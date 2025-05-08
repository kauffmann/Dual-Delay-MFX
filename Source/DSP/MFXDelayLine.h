/*
  ==============================================================================

    MFXDelayLine.h
   
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXAudioHelpers.h"
#include "MFXJuce_Oscillator.h"





class MFXDelayLine
{

public:

    MFXDelayLine();
    ~MFXDelayLine();

    enum ChannelToFade
    {
        mFadeLeft = 0,
        mFadeRight
    };

    void prepareToProcess(const juce::dsp::ProcessSpec& spec);


   

    void setFeedBack(const float inFeedbackLeft, const float inFeedbackRight)
    {
        // Scale between 0.0 - 1.2 to allow self oscillation. Remember tanh_clip (feedbacked_left and feedbacked_left) in process() to avoid sonic explosion.
        mFeedbackGainLeft = jmap(inFeedbackLeft, 0.0f, 1.0f, 0.0f, 1.2f);
        mFeedbackGainRight = jmap(inFeedbackRight, 0.0f, 1.0f, 0.0f, 1.2f);


    }

    void setDryWet(const float& inWet)
    {
        mWetMix = inWet;
        mDryMix = 1.0 - mWetMix;

    }

    // setDryWet() must be called before this
    void modulateDryWetWith(const float& inMod, float currentValue)
    {
        mWetMix = (inMod > 0.0f) ? currentValue += ((1.0f - currentValue) * inMod) : currentValue += (currentValue * inMod);
        mDryMix = 1.0 - currentValue; 

       
    }


    void setSmoothingValue(const float& inValue) noexcept
    {
        // this scales filter freq to get more interesting sound. 
        smoothFilter.setCutoffFrequency(2200.0f / inValue); 
        
        
    }

    void setFadeParams(const float& fadeFreqValue, const bool isFade)
    {
        fadeFreqValue == 25.0f ? mLFO.setFrequency(fadeFreqValue - 0.1) : mLFO.setFrequency(fadeFreqValue);
        mIsFade = isFade;
        mCurrentLfoFadeFrequency = fadeFreqValue;
    }


    

    /** @param sampleTimeOneBeat is based on tempo in DAW - how long one beat takes in samples (sample time).
        It is used to calculate note sync. See sync() in MFXAudioHelpers.h
        and process() in this class.
    */
    void setParameters(const float& leftTime,
                              const float& rightTime,
                              const double& sampleTimeOneBeat,
                              const float& wideParamValue,
                              const float& timeModeLeft,
                              const float& timeModeRight,
                              const bool& pingpong,
                              const float& wetValue = 0.0f,
                              const int& chainMode = mChainedDelay_single) noexcept
    {
        mRightTime = rightTime;
        mLeftTime = leftTime;
        mSampleTimeOneBeat = sampleTimeOneBeat;
        mWideParam = wideParamValue;
        mTimeModeLeft = timeModeLeft;
        mTimeModeRight = timeModeRight;
        mPingpong = pingpong;
        setDryWet(wetValue);
        mChainMode = static_cast<ChainedDelay>(chainMode);
    }

    // setParameters() must be called before below modulation functions

    void modulateTimeLeftWith(const float& value, float currentValue)
    {
       
        mLeftTime = (value > 0.0f) ? std::fmin(currentValue + (currentValue * value), 1.0f) :
            currentValue - std::fabs(currentValue * value);

    }

    void modulateTimeRightWith(const float& value, float currentValue)
    {

        mRightTime = (value > 0.0f) ? std::fmin(currentValue + (currentValue * value), 1.0f) :
            currentValue - std::fabs(currentValue * value);
    }

    void modulateTimeLeftAndRightWith(const float& value, float currentLeftValue, float currentRightValue)
    {
        mLeftTime = (value > 0.0f) ? std::fmin(currentLeftValue + (currentLeftValue * value), 1.0f) :
            currentLeftValue - std::fabs(currentLeftValue * value);

        mRightTime = (value > 0.0f) ? std::fmin(currentRightValue + (currentRightValue * value), 1.0f) :
            currentRightValue - std::fabs(currentRightValue * value);

    }


    void modulateFeedbackLeftWith(const float& value, float currentValue)
    {
        mFeedbackGainLeft = (value > 0.0f) ? currentValue += ((1.2f - currentValue) * value) : currentValue += (currentValue * value);
    }


    void modulateFeedbackRightWith(const float& value, float currentValue)
    {
        mFeedbackGainRight = (value > 0.0f) ? currentValue += ((1.2f - currentValue) * value) : currentValue += (currentValue * value);
    }

    void modulateFeedbackLeftRightWith(const float& value, float currentLeftValue, float currentRightValue)
    {
        mFeedbackGainLeft = (value > 0.0f) ? currentLeftValue += ((1.2f - currentLeftValue) * value) : currentLeftValue += (currentLeftValue * value);
        mFeedbackGainRight = (value > 0.0f) ? currentRightValue += ((1.2f - currentRightValue) * value) : currentRightValue += (currentRightValue * value);

    }

    void modulateWideWith(const float& value, float currentWidthValue)
    {
       
        if (value > 0.0f)
            mWideParam = value > 0.0f ? currentWidthValue += (20.0f - currentWidthValue) * value : currentWidthValue += ((currentWidthValue + 20) * value);
        else
            mWideParam = value > 0.0f ? currentWidthValue += (40.0f + currentWidthValue) * value : currentWidthValue += ((currentWidthValue + 20) * value);

    }




    void process(float& leftCh, float& rightCh, float& leftWetCh, float& rightWetCh);

    void processChannel(float& dryCh, float& wetCh, float time,double& newTime, double& previousTime, int timeMode,
                        ChannelToFade fadeChannel, float& feedbacked, double lfoOutputAbs,
                        double lfoOutputAbsReversed, int channelIndex);

    float readFadeValue(const double& delayTime, double& newTime, double& previousTime,double& lfoAbsValue, double& lfoReversedAbsValue, const ChannelToFade& channel);
    


    void reset() noexcept;

    void setDelayWideSampleTime() noexcept;


    void setFeedbackGainSmooth() noexcept;






    enum ChainedDelay
    {
        mChainedDelay_serial = 0,
        mChainedDelay_parallel,
        mChainedDelay_single

    };



    void setIsSync(const bool& isSyncLeft, const bool& isSyncRight) noexcept
    {
        mIsSyncLeft = isSyncLeft;
        mIsSyncRight = isSyncRight;

    }


   


    float getLeftTimeModeIndexDelay() noexcept
    {
        return mTimeModeLeft;
    }

    float getRightTimeModeIndexDelay() noexcept
    {
        return mTimeModeRight;
    }

    
    bool IsMultipleOfSampleRate(double time, double sample_rate) {
        double remainder = fmod(time,sample_rate * 4);
        return fabs(remainder) < 1e-10; // check if remainder is close enough to zero
    }

    float getLeftTime()
    {
        return mLeftTime;
    }

    float getRightTime()
    {
        return mRightTime;
    }

private:

    std::unique_ptr<juce::dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd> > mDelayLine;
    

    dsp::FirstOrderTPTFilter<double> smoothFilter;
    

    
    float mFeedbackGainLeft = 0.5f;
    float mFeedbackGainRight = 0.5f;
    float mFeedback_left = 0.0f; 
    float mFeedback_right = 0.0f;
    float mWetMix = 0.0f; //1.0f;
    float mDryMix = 0.0f; //1.0-wetMix;
    double mSampleRate{ 0.0 };
    float mRightTime{ 0.0f }, mLeftTime{ 0.0f };
    double mSampleTimeOneBeat{ 0.0 };
    float mTimeModeLeft{ 0.0f }, mTimeModeRight{ 0.0f };
    float mWideParam{ 0.0f };
    bool mPingpong{ false };
    bool mIsSyncLeft{ false };
    bool mIsSyncRight{ false };

   
    

    juce::SmoothedValue<double> mDelaySmoothedWideSampleTime;
    juce::SmoothedValue<double> mRightFeedbackGainSmooth;
    juce::SmoothedValue<double> mLeftFeedbackGainSmooth;

    
    ChainedDelay mChainMode{ mChainedDelay_serial };

    

    // use to control when to read a delay time value and scale/fade poped sample value.
    //Do std::fabs(output) .0-.1 and jmap(output, 0.0f, 1.0f, 1.0f, 0.0f);
    juce::dsp::Oscillator_Redesign<double> mLFO;
    double mCurrentLfoFadeFrequency{2.0};
    bool mIsFade{ false };
    double mNewTime_Left{ 0.0 };
    double mNewTime_Right{ 0.0 };
    double mPreviousTime_Left{ 0.0 };
    double mPreviousTime_Right{ 0.0 };

	double mFadeThreshold{ 0.003 };
    bool mFadeA_R{ true };
    bool mFadeB_R{ true };

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXDelayLine);

};

    

    
    

    

