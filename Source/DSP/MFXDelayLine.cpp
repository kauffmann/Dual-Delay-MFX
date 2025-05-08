/*
  ==============================================================================

    MFXDelayLine.cpp
    
    Author:  Michael Kauffmann

    To use this class:
    PrepareToProcess must be called when initialize and whenever samplerate or buffer size change in DAW.
    In PluginProcesser::procesblok() loop audiobuffer calling process() passing a bufferWriterPointer to unprocessed audiobuffer and another (wet)audiobuffer to put processed audio samples.
    You will need two audiobuffers, the one recieved from DAW(unprocessed audio) and one you create to hold processd audio (wet).
    In processBlok you must add (wet)audiobuffer samples to unprocessed audiobuffer.




    
  ==============================================================================
*/

#include "MFXDelayLine.h"



MFXDelayLine::MFXDelayLine()
{
    smoothFilter.setType (dsp::FirstOrderTPTFilterType::lowpass);
    
    mLFO.initialise([](double x) { return std::sin(-x); }, 128); 
    mLFO.setFrequency(2.0);
}
MFXDelayLine::~MFXDelayLine(){}


void MFXDelayLine::prepareToProcess(const juce::dsp::ProcessSpec& spec)
{
    
    
    
        // set DelayLine size to 4 seconds (add  maximumblockSize for safety)
        mDelayLine = std::make_unique<dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd>>((spec.sampleRate + spec.maximumBlockSize) * 4.0);
        mDelayLine->prepare(spec);

        smoothFilter.prepare(spec);

        mSampleRate = spec.sampleRate;

        mDelaySmoothedWideSampleTime.reset(spec.sampleRate, 0.03);
       

        mRightFeedbackGainSmooth.reset(spec.sampleRate, 0.04);
        mLeftFeedbackGainSmooth.reset(spec.sampleRate, 0.04);

        mLFO.prepare(spec);
    
}


void MFXDelayLine::process(float& leftCh, float& rightCh,
                           float& leftWetCh, float& rightWetCh)
{
   
    double lfoOutputAbs = std::fabs(mLFO.processSample(0.0));
    double lfoOutputAbsReversed = 0.9999999999 - lfoOutputAbs;

    if (mPingpong)
    {
        mDelayLine->pushSample(0, ((leftCh + rightCh) / 2.0f) + mFeedback_right);
        mDelayLine->pushSample(1, mFeedback_left);
    }
    else
    {
        mDelayLine->pushSample(0, leftCh + mFeedback_left);
        mDelayLine->pushSample(1, rightCh + mFeedback_right);
    }

    // Process left and right channels using the helper function
    processChannel(leftCh, leftWetCh, mLeftTime,mNewTime_Left,mPreviousTime_Left, mTimeModeLeft, mFadeLeft, mFeedback_left, lfoOutputAbs, lfoOutputAbsReversed, 0);
    processChannel(rightCh, rightWetCh, mRightTime,mNewTime_Right,mPreviousTime_Right, mTimeModeRight, mFadeRight, mFeedback_right, lfoOutputAbs, lfoOutputAbsReversed, 1);
}





void MFXDelayLine::processChannel(float& dryCh, float& wetCh, float time, double& newTime, double& previousTime, int timeMode,
    ChannelToFade fadeChannel, float& feedbacked, double lfoOutputAbs,
    double lfoOutputAbsReversed, int channelIndex)
{
    auto wideInSampleTimeSmoothed = mDelaySmoothedWideSampleTime.getNextValue();
    auto feedbackGainSmooth = (fadeChannel == mFadeLeft) ? mLeftFeedbackGainSmooth.getNextValue() : mRightFeedbackGainSmooth.getNextValue();

    float delayed = 0.0f;

    if (static_cast<int>(timeMode) != mFXDelayTimeMode_ms)
    {
        int valueToIndex = roundToInt(time * 7);
        valueToIndex = (valueToIndex == 0) ? 1 : valueToIndex;

        double delayTime = (mIsFade == 1.0f)
            ? sync(valueToIndex, timeMode, mSampleTimeOneBeat + (channelIndex == 0 ? -wideInSampleTimeSmoothed : wideInSampleTimeSmoothed))
            : smoothFilter.processSample(channelIndex, sync(valueToIndex, timeMode, mSampleTimeOneBeat + (channelIndex == 0 ? -wideInSampleTimeSmoothed : wideInSampleTimeSmoothed)));

        delayTime = std::min(delayTime, static_cast<double>(mDelayLine->getMaximumDelayInSamples()));

        delayed = (mIsFade == 1.0f)
            ? readFadeValue(delayTime,newTime,previousTime, lfoOutputAbs, lfoOutputAbsReversed, fadeChannel)
            : mDelayLine->popSample(channelIndex, delayTime);
    }
    else
    {
        double delayTime = (mIsFade == 1.0f)
            ? juce::jmax(juce::jmin((time * (mSampleRate * 4.0)) + (channelIndex == 0 ? -wideInSampleTimeSmoothed : wideInSampleTimeSmoothed), mSampleRate * 4.0), 0.0)
            : smoothFilter.processSample(channelIndex, juce::jmax(juce::jmin((time * (mSampleRate * 4.0)) + (channelIndex == 0 ? -wideInSampleTimeSmoothed : wideInSampleTimeSmoothed), mSampleRate * 4.0), 0.0));

        delayTime = std::min(delayTime, static_cast<double>(mDelayLine->getMaximumDelayInSamples()));


        delayed = (mIsFade == 1.0f)
            ? readFadeValue(delayTime,newTime, previousTime, lfoOutputAbs, lfoOutputAbsReversed, fadeChannel)
            : mDelayLine->popSample(channelIndex, delayTime);
    }

    if (mWetMix > 0 && mChainMode == mChainedDelay_serial)
        wetCh = mDryMix * dryCh + (mWetMix * delayed);
    else if (mWetMix > 0 && mChainMode == mChainedDelay_parallel)
        wetCh = delayed * mWetMix;
    else
        wetCh = delayed;

    feedbacked = tanh_clip(delayed * feedbackGainSmooth);
}






float MFXDelayLine::readFadeValue(const double& delayTime, double& newTime, double& previousTime, double& lfoAbsValue, double& lfoReversedAbsValue, const ChannelToFade& channel)
{

    float delayedSampleToSumA = 0.0f;
    float delayedSampleToSumB = 0.0f;

   
    // if F = 2Hz then 1 cycle 0-0 is sampleRate / 2. half 0-1 or 1-0  is (sampleRate / 2) / 2 11025 
    // if F = 5Hz then 1 cycle 0-0 is sampleRate / 5. half 0-1 or 1-0  is (sampleRate / 5) / 2  

    // Fade works by continuesly fade/scale up/down new and previous audio sample value. SampleTime values
    // is divided into virtuelly 2 sub channels, A and B, representing new and previous sampleTime values.
    // Scaled A and B sample audio values is finally summed and asigned to a variable that will be the audio that comes out of delay.
    // Scaling is controlled by a LFO (0.3 - 100 Hz). LFO range from 0-1 (A channel) and 1-0 (B channel).
    // Varing the frequency changed the time/speed it fades. 
    // User can experiment with frquency - geting different audible results (slow - more smooth, or drop outs, faster a bit hard transition without clicks).

    // We prepare (read/scale A) to fade new or existing time up while at the same time fade previous time down (read/scale B)
    if (lfoAbsValue < mFadeThreshold && mFadeA_R)
    {
          newTime = delayTime;
        
          // View comment below - LFO can reach....
          if(channel == mFadeRight)
             mFadeA_R = false;
    }

    // When B is scaled close to 0 existing time is silenced. We prepare (read B) to fade new or existing time up
    if (lfoReversedAbsValue < mFadeThreshold && mFadeB_R)
    {
        previousTime = newTime;
        
        // View comment below - LFO can reach....
        if (channel == mFadeRight)
            mFadeB_R = false;

    }

    //read A                       
    delayedSampleToSumA = mDelayLine->popSample(channel, newTime);
    
    // This value is used if delay times stop changing - then fading is not nessesary, and only unscaled A value is returned.
    float sampleOutNoFade = delayedSampleToSumA;

    // Scale A
    delayedSampleToSumA *= lfoAbsValue;

    // read B
    delayedSampleToSumB = mDelayLine->popSample(channel, previousTime, false);

    // Scale B
    delayedSampleToSumB *= lfoReversedAbsValue;

    // LFO can reach below threshold and trigger read A/B more than once. This assure trigger only once.
    if (lfoAbsValue > mFadeThreshold)
        mFadeA_R = true;

    if (lfoReversedAbsValue > mFadeThreshold)
        mFadeB_R = true;

    

    // Sum and return A + B or just clean out if no change in time. This is to avoid artifacts when high fade freq and user is not modulating time.
    
    if (channel == mFadeLeft && previousTime == delayTime)
    {    
        return sampleOutNoFade;
    }
    else if (channel == mFadeRight && previousTime == delayTime)
    {   
        return sampleOutNoFade;
    }
    
    else
    {
        return delayedSampleToSumA + delayedSampleToSumB;
    }
    
}





void MFXDelayLine::reset() noexcept
{
    if (mDelayLine != nullptr)
        mDelayLine->reset();
    
}

void MFXDelayLine::setDelayWideSampleTime() noexcept
{
    double wideInSampleTime = ((mSampleRate / 4000.0 ) * mWideParam ); 
    mDelaySmoothedWideSampleTime.setTargetValue(wideInSampleTime);
}



void MFXDelayLine::setFeedbackGainSmooth() noexcept
{
    mLeftFeedbackGainSmooth.setTargetValue(mFeedbackGainLeft);
    mRightFeedbackGainSmooth.setTargetValue(mFeedbackGainRight);
}






