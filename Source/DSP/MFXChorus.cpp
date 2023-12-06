/*
  ==============================================================================

    MFXChorus.cpp
    
    Author:  Michael Kauffmann

    Add chorus to incoming audio. To use this class:
    
    prepare must be called when initialize instance, DAW also call this if samplerate or buffer size change.
    Pass a audiobuffer& reference to proces(). 

  ==============================================================================
*/

#include "MFXChorus.h"

MFXChorus::MFXChorus()
{
    mLfoChorus1.initialise( [] (double x)
        {
            x = x / juce::MathConstants<double>::pi;
            return  juce::jmin(2.0 + 2.0 * x, std::abs((x - 0.5) * 2.0) - 1.0); 

        }, 128);


    mLfoChorus2.initialise([](double x)
        {
            x = x / juce::MathConstants<double>::pi;
            return  juce::jmin(2.0 + 2.0 * x, std::abs((x - 0.5) * 2.0) - 1.0); 

        },128 );

    mStereoWidth = std::make_unique<MFXStereo>();    
}

MFXChorus::~MFXChorus(){}

void MFXChorus::prepare(const juce::dsp::ProcessSpec& spec)
{
    
    // set DelayLine size to 15ms + 2 * mDeltaSampleTime safety
    mDelayCLine1 = std::make_unique<dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd>>((spec.sampleRate * 0.015) + (mDeltaSampleTime * 2));
    mDelayCLine1->prepare(spec);

    mDelayCLine2 = std::make_unique<dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd>>((spec.sampleRate * 0.015) + (mDeltaSampleTime * 2));
    mDelayCLine2->prepare(spec);

    mLfoChorus1.prepare(spec);
    mLfoChorus2.prepare(spec);
    
    mCurrentSampleRate = spec.sampleRate;
}

void MFXChorus::process(juce::AudioBuffer<float>& buffer) noexcept
{   

    float* bufferWriter[2] = { buffer.getWritePointer(0), buffer.getWritePointer(1) };

    float delaytimeSampleOut1Ch0 = 0.0f;
    float delaytimeSampleOut1Ch1 = 0.0f;
    float delaytimeSampleOut2Ch0 = 0.0f;
    float delaytimeSampleOut2Ch1 = 0.0f;
    float delaytimeSampleOutSumCh0 = 0.0f;
    float delaytimeSampleOutSumCh1 = 0.0f;

    float feedbackValue1Ch0 = 0.0f;
    float feedbackValue1Ch1 = 0.0f;
    float feedbackValue2Ch0 = 0.0f;
    float feedbackValue2Ch1 = 0.0f;

    for (size_t i = 0; i < buffer.getNumSamples(); i++)
    {
        double readhead1 = getReadHead(mDelayLine1);
        double readhead2 = getReadHead(mDelayLine2);
        
        for (size_t channel = 0;  channel < 2;  channel++)
        {
            if(channel == 0)
            {
                writeSampleToDelayLine(bufferWriter[channel][i], channel, feedbackValue1Ch0, feedbackValue2Ch0);
                delaytimeSampleOut1Ch0 = readFromDelayLine(readhead1, channel, mDelayLine1);
                delaytimeSampleOut2Ch0 = readFromDelayLine(readhead2, channel, mDelayLine2);
            }

            if (channel == 1)
            {
                writeSampleToDelayLine(bufferWriter[channel][i], channel, feedbackValue1Ch1, feedbackValue2Ch1);
                delaytimeSampleOut1Ch1 = readFromDelayLine(readhead1, channel, mDelayLine1);
                delaytimeSampleOut2Ch1 = readFromDelayLine(readhead2, channel, mDelayLine2);
            }
                
        }

        // Calculate feedback value to put in delayline (write func)
        feedbackValue1Ch0 = tanh_clip(delaytimeSampleOut1Ch0 * mFeedBackGainParameter);
        feedbackValue1Ch1 = tanh_clip(delaytimeSampleOut1Ch1 * mFeedBackGainParameter);
        feedbackValue2Ch0 = tanh_clip(delaytimeSampleOut2Ch0 * mFeedBackGainParameter);
        feedbackValue2Ch1 = tanh_clip(delaytimeSampleOut2Ch1 * mFeedBackGainParameter);


        // Samples from all delaylines are summed
        delaytimeSampleOutSumCh0 = (delaytimeSampleOut1Ch0 + delaytimeSampleOut2Ch0); 
        delaytimeSampleOutSumCh1 = (delaytimeSampleOut1Ch1 + delaytimeSampleOut2Ch1); 
        
        mStereoWidth->processWidth(delaytimeSampleOutSumCh0, delaytimeSampleOutSumCh1);

        bufferWriter[0][i] = (bufferWriter[0][i] * (1 - mDryParameter)) + (delaytimeSampleOutSumCh0 * mWetParameter);
        bufferWriter[1][i] = (bufferWriter[1][i] * (1 - mDryParameter)) + (delaytimeSampleOutSumCh1 * mWetParameter);
        
    }

}

void MFXChorus::reset() noexcept
{

    if (mDelayCLine1 != nullptr)
        mDelayCLine1->reset();

    if (mDelayCLine2 != nullptr)
        mDelayCLine2->reset();
}

void MFXChorus::writeSampleToDelayLine(float& sample, int channel, float& feedbackValue1, float& feedbackValue2)
{
    mDelayCLine1->pushSample(channel, sample + feedbackValue1);
    mDelayCLine2->pushSample(channel, sample + feedbackValue2);
}



float MFXChorus::readFromDelayLine(double readSampleTimeIndex, int channel, DelayLine lineToReadFrom) const noexcept
{
    switch (lineToReadFrom)
    {
    case mDelayLine1:
        if (channel == 0)
            return mDelayCLine1->popSample(channel, readSampleTimeIndex);   
        else
            return mDelayCLine1->popSample(channel, readSampleTimeIndex + mDeltaSampleTime); // add sample time to get stereo 
        break;
    case mDelayLine2:
        if (channel == 0)
            return mDelayCLine2->popSample(channel, readSampleTimeIndex + mDeltaSampleTime); // add sample time to get stereo 
        else
            return mDelayCLine2->popSample(channel, readSampleTimeIndex);
        break;
  
    default: jassertfalse; // should not be here
        return 0.02f;
        
        break;
    }

    
}

double MFXChorus::getReadHead(DelayLine lineToFeedLFO) noexcept
{
    switch (lineToFeedLFO)
    {
    case mDelayLine1: // curly brackets is needed when declaring variables. https://stackoverflow.com/questions/92396/why-cant-variables-be-declared-in-a-switch-statement
    {
        double lfoOut1 = mLfoChorus1.processSample(0.0f) * mDepthParameter;

        // get Time in samples. Time sweeps whithin 5 - 10ms. 0.005f
        double readHead_1 = jmap<double>(lfoOut1, -1.0, 1.0, 0.005, 0.010) * mCurrentSampleRate; 
        
        
        return readHead_1;
    }
        break;
    case mDelayLine2:
    {
        double lfoOut2 = mLfoChorus2.processSample(0.0f) * mDepthParameter;

        // get Time in samples. Time sweeps whithin 5 - 10ms.
        double readHead_2 = jmap<double>(lfoOut2, -1.0, 1.0, 0.005, 0.010) * mCurrentSampleRate;
        

        return readHead_2;
    }
        break;
    
    default: jassertfalse; // should not be here
        return 0.0;
        break;
    }


  }












