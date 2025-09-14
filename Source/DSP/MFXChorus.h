/*
  ==============================================================================

    MFXChorus.h
    
    Author:  Michael kauffmann

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>
#include "MFXStereo.h"
#include "MFXAudioHelpers.h"
#include "MFXJuce_Oscillator.h"





class MFXChorus
{

public:
    //=============================================================================

    MFXChorus();
    
    ~MFXChorus();
        
    enum DelayLine
    {
        mDelayLine1 = 0,
        mDelayLine2
    };

    //=============================================================================
    void prepare(const juce::dsp::ProcessSpec& spec);

    void process(juce::AudioBuffer<float>& buffer) noexcept;

    void reset() noexcept;

  
    // Delayline Buffer operations ==============================================================================================

    void writeSampleToDelayLine(float& sample, int channel, float& feedbackValue1, float& feedbackValue2);
    
    float readFromDelayLine(double readSampleTimeIndex, int channel, DelayLine lineToReadFrom) const noexcept;
    
    double getReadHead(DelayLine lineToFeedLFO) noexcept;

    

   
    // Set states of FX parameter (called from AudioProcessor class) =========================================================

    void setDryWetParameter(const float& dryWet) noexcept
    {
        
        mWetParameter = dryWet;


        mDryParameter = dryWet; 
    }
    


    void setDepthParameter(const float& depth) noexcept
    {
        mDepthParameter = depth;
    }


    void setRateParameter(const float& rate) noexcept
    {
        mLfoChorus1.setFrequency(rate);
        mLfoChorus2.setFrequency(rate * 3.1f);
    }
    


    void setStereoWidthParameter(const float& stereoAmount) noexcept
    {
        mStereoWidth->setWidth(stereoAmount);
        mStereoAmountParameter = stereoAmount;
    }


    void setFeedbackGainParameter(const float& feedbackGain) noexcept
    {
        mFeedBackGainParameter = feedbackGain;
    }

    

   
private:
    //===============================================================================

   

    double mCurrentSampleRate{44100.0};

    // This is where audio data is stored from buffer. 
    std::unique_ptr<juce::dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd> > mDelayCLine1;
    std::unique_ptr<juce::dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd> > mDelayCLine2;

   
    // LFO data
    juce::dsp::Oscillator_Redesign <double> mLfoChorus1;
    juce::dsp::Oscillator_Redesign<double> mLfoChorus2;
    
    


    // Parameters
    float mWetParameter{0.0f};
    float mDryParameter{ 0.0f };

    float mFeedBackGainParameter{ 0.0f };
    

    // Depth: how wide (in ms) the delay time sweeps.
    float mDepthParameter{ 0.0f };
    // Rate: how quickly the delay time sweeps, within the range set by depth
    float mRateParameter{ 0.0f };
    
    float mStereoAmountParameter{ 0.0f };

    std::unique_ptr<MFXStereo> mStereoWidth;

    // add sample time difference to to one of the channels to get stereo 
    double mDeltaSampleTime{25.0};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXChorus);

};
