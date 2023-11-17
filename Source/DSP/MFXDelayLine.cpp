/*
  ==============================================================================

    MFXDelayLine.cpp
    
    Author:  Michael Kauffmann

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
    mSmoothedLeftTime.reset(spec.sampleRate, 0.03);
    mSmoothedWetMix.reset(spec.sampleRate,0.04);
    mSmoothedDryMix.reset(spec.sampleRate,0.04);
    
    mRightFeedbackGainSmooth.reset(spec.sampleRate, 0.04);
    mLeftFeedbackGainSmooth.reset(spec.sampleRate, 0.04);

    mLFO.prepare(spec);
    
}




void MFXDelayLine::process(float& leftCh, float& rightCh,
                           float& leftWetCh, float& rightWetCh)
{
   
    

    
    // Experiment: Linear Fade sync. unlike LFO. Does this assure perfect sync fade values ? -------------------------------------------------------------------
    
     mCountSamples++;
    

    if(mCountSamples <= 7500)
    {
        mA += 0.000133333333;
        
    }
    else if (mCountSamples <= 15000)
    {
        mA -= 0.000133333333;
       
    }


    if (mCountSamples <= 7500)
    {
        mB -= 0.000133333333;
    }
    else if (mCountSamples <= 15000)
    {
        mB += 0.000133333333;
    }

    
    
    if (mCountSamples == 15000)
        mCountSamples = 0;


    //------------------------------------------------------------------------------------------------------------------------------------------



    //Process and remap LFO output. 1: 0 - 1(abs) 2: 1 - 0 (abs value)
  
    double lfoOutputAbs = std::fabs(mLFO.processSample(0.0)); 
   
    
    double lfoOutputAbsReversed = 0.9999999999 - lfoOutputAbs;

    
   //---------------------------------------------------------------------------------------------------------------------------------------------
    


 
    
    if (mPingpong)
    {
        mDelayLine->pushSample(0, ((leftCh + rightCh) / 2.0f) + feedbacked_right);
        mDelayLine->pushSample(1, feedbacked_left);
    }
    else
    {
        // Normal Delay. Write to delayLine
        mDelayLine->pushSample(0, leftCh + feedbacked_left);
        mDelayLine->pushSample(1, rightCh + feedbacked_right);        
        
    }
    
    auto wideInSampleTimeSmoothed = mDelaySmoothedWideSampleTime.getNextValue();
    auto leftFeedbackGainSmooth = mLeftFeedbackGainSmooth.getNextValue();
    auto rightFeedbackGainSmooth =  mRightFeedbackGainSmooth.getNextValue();
    
    
    float delayed_left = 0.0f;
    float delayed_right = 0.0f;
    
    
    // Find point in delayLine to read from, and set referenced wetCh
    if( static_cast<int>(mTimeModeLeft) != mFXDelayTimeMode_ms)
    {
        int valueToIndex = roundToInt(mLeftTime * 7);

        if (valueToIndex == 0)
            valueToIndex = 1;
        

        if (mIsFade == 1.0f) 
        {
            double time = sync(valueToIndex, mTimeModeLeft,
                               mSampleTimeOneBeat - wideInSampleTimeSmoothed); 

            // Make sure time never get larger than the size of delayline (circular buffer)
            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();


            // time multiple of samplerate
            //double factor = round(time / mSampleRate);
            //time = factor * mSampleRate;

            delayed_left = readFadeValue(time, lfoOutputAbs, lfoOutputAbsReversed, mFadeLeft);
            
        }
        else 
        {
            double time = smoothFilter.processSample(0, sync(valueToIndex, mTimeModeLeft,
            mSampleTimeOneBeat - wideInSampleTimeSmoothed)); 
                
            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();
                
           
                
            delayed_left = mDelayLine->popSample(0, time);
        }
        
        if (wetMix > 0 && mChainMode == mChainedDelay_serial)
            leftWetCh = dryMix * leftCh + (wetMix * delayed_left);
        
        else if (wetMix > 0 && mChainMode == mChainedDelay_parallel)  
                 leftWetCh = delayed_left * wetMix;
        
        else leftWetCh = delayed_left; // delay 1 only used
        
        
        mPingpong == true ? feedbacked_left = delayed_left : feedbacked_left = tanh_clip(delayed_left * leftFeedbackGainSmooth);
        
    }
    else {

        if (mIsFade == 1.0f) 
        {
           
            double time = juce::jmax(juce::jmin((mLeftTime * (mSampleRate * 4.0)) - wideInSampleTimeSmoothed, mSampleRate * 4.0), 0.0);
            
            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();
            
            
            

            delayed_left =  readFadeValue(time, lfoOutputAbs, lfoOutputAbsReversed, mFadeLeft); 
            
        }
        else
        {
            double time = smoothFilter.processSample(0, juce::jmax(juce::jmin((mLeftTime * (mSampleRate * 4.0)) - wideInSampleTimeSmoothed, mSampleRate * 4.0), 0.0));

            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();

            
            
            delayed_left = mDelayLine->popSample(0,time);
           
        }
            
        
        
        if (wetMix > 0 && mChainMode == mChainedDelay_serial)
            leftWetCh = dryMix * leftCh + (wetMix * delayed_left);    // add delay signal from delay 1 and delay 2. delay_left contain signal from d1
        
        else if (wetMix > 0 && mChainMode == mChainedDelay_parallel)
            leftWetCh = delayed_left * wetMix;
        
        else leftWetCh = delayed_left;
        
        
        mPingpong == true ? feedbacked_left = delayed_left : feedbacked_left = tanh_clip(delayed_left * leftFeedbackGainSmooth);
        
        

    }
    
    // Find point in delayLine to read from, and set referenced wetCh
    if( static_cast<int>(mTimeModeRight) != mFXDelayTimeMode_ms)
    {   
        int valueToIndex = roundToInt(mRightTime * 7);
        
        if(valueToIndex == 0)
            valueToIndex = 1;
        
        
       
        if (mIsFade == 1.0f)
        { 
            double time = sync(valueToIndex, mTimeModeRight,
                                                     mSampleTimeOneBeat  + wideInSampleTimeSmoothed); 

            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();

            
            delayed_right = readFadeValue(time, lfoOutputAbs, lfoOutputAbsReversed, mFadeRight);                     
        }

        else
        {

            double time = smoothFilter.processSample(1, sync(valueToIndex, mTimeModeRight,
                                              mSampleTimeOneBeat + wideInSampleTimeSmoothed ));

            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();

            delayed_right = mDelayLine->popSample(1, time);
        }


        
        if (wetMix > 0 && mChainMode == mChainedDelay_serial)
            rightWetCh = (rightCh * dryMix) + (delayed_right * wetMix);
        
        else if (wetMix > 0 && mChainMode == mChainedDelay_parallel)
                 rightWetCh = delayed_right * wetMix;

        
        else rightWetCh = delayed_right;
        
        
        feedbacked_right = tanh_clip( delayed_right * rightFeedbackGainSmooth);
        
    }
    
    else
    {
        
        if (mIsFade == 1.0f)
        {
            double time = juce::jmax(juce::jmin((mRightTime* (mSampleRate * 4.0)) + wideInSampleTimeSmoothed , mSampleRate * 4.0), 0.0);

            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();
                      
            delayed_right = readFadeValue(time, lfoOutputAbs, lfoOutputAbsReversed, mFadeRight);       
        }

        else
        {


            // sampletime to read behind dry signal. Do max/min to avoid negative values.
            double time = smoothFilter.processSample(1,
                                              juce::jmax(juce::jmin((mRightTime * (mSampleRate * 4.0)) + wideInSampleTimeSmoothed, mSampleRate * 4.0), 0.0));

            if (time > mDelayLine->getMaximumDelayInSamples())
                time = mDelayLine->getMaximumDelayInSamples();

            
            delayed_right = mDelayLine->popSample(1, time);
        }
        
        

        if (wetMix > 0 && mChainMode == mChainedDelay_serial)
        {
            rightWetCh = (rightCh * dryMix) + (delayed_right * wetMix);
            
        }
        
        
        else if (wetMix > 0 && mChainMode == mChainedDelay_parallel)
        {
            rightWetCh = delayed_right * wetMix;
            
        }
        
        else
        {
            rightWetCh = delayed_right;
            
        }
        
        feedbacked_right = tanh_clip( delayed_right * rightFeedbackGainSmooth);
    }

}


float MFXDelayLine::readFadeValue(const double& delayTime, double& lfoAbsValue, double& lfoReversedAbsValue, const ChannelToFade& channel)
{

    float delayedSampleToSumA = 0.0f;
    float delayedSampleToSumB = 0.0f;

    double thresholdValue = mCurrentLfoFadeFrequency == 2.0 ? 0.000266666667 : 0.003; // 0.03(was)   0.09

    if (mCurrentLfoFadeFrequency == 2.0)
    {
        lfoAbsValue = mA;
        lfoReversedAbsValue = mB;
    }

    
    
   
    // if F = 2Hz then 1 cycle 0-0 is sampleRate / 2. half 0-1 or 1-0  is (sampleRate / 2) / 2 11025 
    // if F = 5Hz then 1 cycle 0-0 is sampleRate / 5. half 0-1 or 1-0  is (sampleRate / 5) / 2  

    // Fade works by continuesly fade/scale up/down new and previous audio sample value. SampleTime values
    // is divided into virtuelly 2 sub channels, A and B, representing new and previous sampleTime values.
    // Scaled A and B sample audio values is finally summed and asigned to a variable that will be the audio that comes out of delay.
    // Scaling is controlled by a LFO (0.3 - 100 Hz). LFO range from 0-1 (A channel) and 1-0 (B channel).
    // Varing the frequency changed the time/speed it fades. 
    // User can experiment with frquency - geting different audible results (slow - more smooth, or drop outs, faster a bit hard transition without clicks).

    // We prepare (read/scale A) to fade new or existing time up while at the same time fade previous time down (read/scale B)
    if (lfoAbsValue < thresholdValue && mFadeA_R) 
    {
          channel == mFadeLeft ? mNewTime_Left = delayTime : mNewTime_Right = delayTime;
        
          // View comment below - LFO can reach....
          if(channel == mFadeRight)
             mFadeA_R = false;
    }

    // When B is scaled close to 0 existing time is silenced. We prepare (read B) to fade new or existing time up
    if (lfoReversedAbsValue < thresholdValue && mFadeB_R)
    {
        channel == mFadeLeft ? mPreviousTime_Left = mNewTime_Left : mPreviousTime_Right = mNewTime_Right;
        
        // View comment below - LFO can reach....
        if (channel == mFadeRight)
            mFadeB_R = false;

    }

    //read A  
    delayedSampleToSumA = channel == mFadeLeft ? mDelayLine->popSample(channel, mNewTime_Left): mDelayLine->popSample(channel, mNewTime_Right);
    
    // This value is used if delay times stop changing - then fading is not nessesary, and only unscaled A value is returned.
    float sampleOutNoFade = delayedSampleToSumA;

    // Scale A
    delayedSampleToSumA *= lfoAbsValue;

    // read B
    delayedSampleToSumB = channel == mFadeLeft ? mDelayLine->popSample(channel, mPreviousTime_Left, false): mDelayLine->popSample(channel, mPreviousTime_Right, false);

    // Scale B
    delayedSampleToSumB *= lfoReversedAbsValue;

    // LFO can reach below threshold and trigger read A/B more than once. This assure trigger only once.
    if (lfoAbsValue > thresholdValue)
        mFadeA_R = true;

    if (lfoReversedAbsValue > thresholdValue)
        mFadeB_R = true;

    

    // Sum and return A + B or just clean out if no change in time. This is to avoid artifacts when high fade freq and user is not modulating time.
    
    if (channel == mFadeLeft && mPreviousTime_Left == delayTime)
    {    
        return sampleOutNoFade;
    }
    else if (channel == mFadeRight && mPreviousTime_Right == delayTime)
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
    mLeftFeedbackGainSmooth.setTargetValue(feedbackGainLeft);
    mRightFeedbackGainSmooth.setTargetValue(feedbackGainRight);
}


