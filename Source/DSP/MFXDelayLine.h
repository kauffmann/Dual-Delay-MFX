/*
  ==============================================================================

    MFXDelayLine.h
   
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXAudioHelpers.h"

enum ChannelToFade
{
    mFadeLeft = 0,
    mFadeRight
};


class MFXDelayLine
{

public:

    MFXDelayLine();
    ~MFXDelayLine();


    void prepareToProcess(const juce::dsp::ProcessSpec& spec);


   

    inline void setFeedBack(const float inFeedbackLeft, const float inFeedbackRight)
    {
        // Scale between 0.0 - 1.2 to allow self oscillation. Remember tanh_clip (feedbacked_left and feedbacked_left) in process() to avoid sonic explosion.
        feedbackGainLeft = jmap(inFeedbackLeft, 0.0f, 1.0f, 0.0f, 1.2f);
        feedbackGainRight = jmap(inFeedbackRight, 0.0f, 1.0f, 0.0f, 1.2f);


    }

    inline void setDryWet(const float& inWet)
    {
        wetMix = inWet;
        dryMix = 1.0 - wetMix;

    }

    // setDryWet() must be called before this
    inline void modulateDryWetWith(const float& inMod, float currentValue)
    {
        wetMix = (inMod > 0.0f) ? currentValue += ((1.0f - currentValue) * inMod) : currentValue += (currentValue * inMod);
        dryMix = 1.0 - currentValue; 

       
    }


    inline void setSmoothingValue(const float& inValue) noexcept
    {
        // this scales filter freq to get more interesting sound. 
        smoothFilter.setCutoffFrequency(2200.0f / inValue); 
        
        
    }

    inline void setFadeParams(const float& fadeFreqValue, const bool isFade)
    {
        fadeFreqValue == 25.0f ? mLFO.setFrequency(fadeFreqValue - 0.1) : mLFO.setFrequency(fadeFreqValue);
        mIsFade = isFade;
        mCurrentLfoFadeFrequency = fadeFreqValue;
    }


    

    /** @param sampleTimeOneBeat is based on tempo in DAW - how long one beat takes in samples (sample time).
        It is used to calculate note sync. See sync() in MFXAudioHelpers.h
        and process() in this class.
    */
    inline void setParameters(const float& leftTime,
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
        mInitialLeftTime = mLeftTime;
        mInitialRightTime = mRightTime;
        mSampleTimeOneBeat = sampleTimeOneBeat;
        mWideParam = wideParamValue;
        mTimeModeLeft = timeModeLeft;
        mTimeModeRight = timeModeRight;
        mPingpong = pingpong;
        setDryWet(wetValue);
        mChainMode = static_cast<ChainedDelay>(chainMode);
    }

    // setParameters() must be called before below modulation functions

    inline void modulateTimeLeftWith(const float& value, float currentValue)
    {
       
        mLeftTime = (value > 0.0f) ? std::fmin(currentValue + (currentValue * value), 1.0f) :
            currentValue - std::fabs(currentValue * value);

    }

    inline void modulateTimeRightWith(const float& value, float currentValue)
    {

        mRightTime = (value > 0.0f) ? std::fmin(currentValue + (currentValue * value), 1.0f) :
            currentValue - std::fabs(currentValue * value);
    }

    inline void modulateTimeLeftAndRightWith(const float& value, float currentLeftValue, float currentRightValue)
    {
        mLeftTime = (value > 0.0f) ? std::fmin(currentLeftValue + (currentLeftValue * value), 1.0f) :
            currentLeftValue - std::fabs(currentLeftValue * value);

        mRightTime = (value > 0.0f) ? std::fmin(currentRightValue + (currentRightValue * value), 1.0f) :
            currentRightValue - std::fabs(currentRightValue * value);

    }


    inline void modulateFeedbackLeftWith(const float& value, float currentValue)
    {
        feedbackGainLeft = (value > 0.0f) ? currentValue += ((1.2f - currentValue) * value) : currentValue += (currentValue * value);
    }


    inline void modulateFeedbackRightWith(const float& value, float currentValue)
    {
        feedbackGainRight = (value > 0.0f) ? currentValue += ((1.2f - currentValue) * value) : currentValue += (currentValue * value);
    }

    inline void modulateFeedbackLeftRightWith(const float& value, float currentLeftValue, float currentRightValue)
    {
        feedbackGainLeft = (value > 0.0f) ? currentLeftValue += ((1.2f - currentLeftValue) * value) : currentLeftValue += (currentLeftValue * value);
        feedbackGainRight = (value > 0.0f) ? currentRightValue += ((1.2f - currentRightValue) * value) : currentRightValue += (currentRightValue * value);

    }

    inline void modulateWideWith(const float& value, float currentWidthValue)
    {
       
        if (value > 0.0f)
            mWideParam = value > 0.0f ? currentWidthValue += (20.0f - currentWidthValue) * value : currentWidthValue += ((currentWidthValue + 20) * value);
        else
            mWideParam = value > 0.0f ? currentWidthValue += (40.0f + currentWidthValue) * value : currentWidthValue += ((currentWidthValue + 20) * value);

    }




    void process(float& leftCh, float& rightCh, float& leftWetCh, float& rightWetCh);

    float readFadeValue(const double& delayTime, double& lfoAbsValue, double& lfoReversedAbsValue, const ChannelToFade& channel);
    


    void reset() noexcept;

    void setDelayWideSampleTime() noexcept;


    void setFeedbackGainSmooth() noexcept;






    enum ChainedDelay
    {
        mChainedDelay_serial = 0,
        mChainedDelay_parallel,
        mChainedDelay_single

    };



    inline void setIsSync(const bool& isSyncLeft, const bool& isSyncRight) noexcept
    {
        mIsSyncLeft = isSyncLeft;
        mIsSyncRight = isSyncRight;

    }


   


    inline float getLeftTimeModeIndexDelay() noexcept
    {
        return mTimeModeLeft;
    }

    inline float getRightTimeModeIndexDelay() noexcept
    {
        return mTimeModeRight;
    }

    
    inline bool IsMultipleOfSampleRate(double time, double sample_rate) {
        double remainder = fmod(time,sample_rate * 4);
        return fabs(remainder) < 1e-10; // check if remainder is close enough to zero
    }

    inline float getLeftTime()
    {
        return mLeftTime;
    }

    inline float getRightTime()
    {
        return mRightTime;
    }

private:

    std::unique_ptr<juce::dsp::DelayLine<float, dsp::DelayLineInterpolationTypes::Lagrange3rd> > mDelayLine;
    

    dsp::FirstOrderTPTFilter<double> smoothFilter;
    

    // TODO: rename variables - to prepend m before name. Makes clear its a member variable
    float feedbackGainLeft = 0.5f;
    float feedbackGainRight = 0.5f;
    float feedbacked_left = 0.0f; 
    float feedbacked_right = 0.0f;
    float wetMix = 0.0f; //1.0f;
    float dryMix = 0.0f; //1.0-wetMix;
    float mTimeSmoothedLeft{ 0.0f };
    float mTimeSmoothedRight{ 0.0f };
    double mSampleRate{ 0.0 };
    bool isPrepared{ false };
    float mRightTime{ 0.0f }, mLeftTime{ 0.0f };
    double mSampleTimeOneBeat{ 0.0 };
    float mTimeModeLeft{ 0.0f }, mTimeModeRight{ 0.0f };
    float mWideParam{ 0.0f };
    bool mPingpong{ false };
    bool mIsSyncLeft{ false };
    bool mIsSyncRight{ false };

   
    

    float mInitialLeftTime{ 0.0f };
    float mInitialRightTime{ 0.0f };


    juce::SmoothedValue<double> mDelaySmoothedWideSampleTime;
    juce::SmoothedValue<double> mSmoothedRightTime;
    juce::SmoothedValue<double> mSmoothedLeftTime;
    juce::SmoothedValue<double> mRightFeedbackGainSmooth;
    juce::SmoothedValue<double> mLeftFeedbackGainSmooth;

    juce::SmoothedValue<double> mSmoothedWetMix;
    juce::SmoothedValue<double> mSmoothedDryMix;

    ChainedDelay mChainMode{ mChainedDelay_serial };

    

    // use to control when to read a delay time value and scale/fade poped sample value.
    //Do std::fabs(output) .0-.1 and jmap(output, 0.0f, 1.0f, 1.0f, 0.0f);
    juce::dsp::Oscillator<double> mLFO;
    double mCurrentLfoFadeFrequency{2.0};
    bool mIsFade{ false };
    double mNewTime_Left{ 0.0 };
    double mNewTime_Right{ 0.0 };
    double mPreviousTime_Left{ 0.0 };
    double mPreviousTime_Right{ 0.0 };


    bool mFadeA_R{ true };
    bool mFadeB_R{ true };

    int mCountSamples = { 0 };

    double mA = { 0.0 };
    double mB = { 1.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXDelayLine);

};

    

    
    

    

