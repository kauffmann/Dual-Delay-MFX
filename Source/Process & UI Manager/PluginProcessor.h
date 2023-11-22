/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
 
  
    Author:  Michael Kauffmann

   
 
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


#include "MFXGain.h"

#include "MFXPresetManager.h"

#include "MFXReverb.h"

#include "MFXBitCrusher.h"

#include "MFXMoogFilter.h"
#include "MFXDelayLine.h"

#include "MFXSimpleFilter.h"
#include "MFXStereo.h"
#include "MFXDucking.h"

#include "MFXUsedParameters.h"
#include "MFXHelperFunctions.h"
#include "MFXAudioHelpers.h"

#include "Fifo.h"
#include "MFXEnvelopFollower.h"


#include "ModulationData.h"

#include "MFXChorus.h"


#if JUCE_WINDOWS
static const String directorySeparator = "\\";
#elif JUCE_MAC
static const String directorySeparator = "/";
#elif JUCE_LINUX
static const String directorySeparator = "/";

#endif





class PluginProcessor  : public juce::AudioProcessor /*, public juce::AudioProcessorListener*/
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    // ====================================================================================
    
    AudioProcessorValueTreeState parameters;
    
    
    
    inline MFXPresetManager* getPresetManager()
    {
        return mPresetManager.get();
    }
    
    //A little helper to get the parameter ID
    inline juce::String getParamID(juce::AudioProcessorParameter* param)
    {
        if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
            return paramWithID->paramID;
        
        return param->getName(50);
    }
    
    

    inline void setClearBuffer(bool clear)
    {
        mClearBuffer = clear;
        
        
    }

    
    
    // LevelMeter value that goes into a meter
    float getRmsLevelIn(const int channel);
    
    // LevelMeter value that goes into a meter
    float getRmsLevelOut(const int channel);
    
    
    ActionBroadcaster mPresetNameChangeEvent;
    
    
    
    
    // UI related data that need to keep state regardless if UI gets destroyed(window closed).
    struct UIPersistentData
    {
        bool mIsParallelModeOn = false;
        String mHelperUIText = "";
        bool mIsSliderTouchedByUser = false;
        bool mIsButtonTouchedByUser = false;
        bool mIsComboBoxTouchedByUser = false;
        // use this one when UI is showing delay 1 and a new preset is called. Needed because combobox  listener, setFXStyle in fxpanel is not called.
        // set true in topPanel comboBoxChanged and read/set back to false in FXPanel timer loop - calling setFXStyle. 
        bool mIsPresetLoaded = false;
        
        
        // Array that store GUI parameter values as strings (value + extention, e.g. %, ms, hz) that can be accessed and displayed below UI sliders. See FXPanel(line 175-300). 
        juce::String mParameterValueText[mFXParameter_TotalNumParameters] =
        {
            
            // Names here are just for readerbility, they all get overwritten by createParameterLayout and in FXPanel(line 175-300), that read UI element value and store in here.
            "Input", //  Delay Input Gain
            "Rate", //  Chorus
            "Depth", //  Chorus
            "Chorus", // Chorus
            "Time Left", //  Delay 2
            "Feedback", //  Delay 2
            " ", //  Delay 2
            "Time Right", //  Delay 2
            "Time Mode", //  Delay 2
            "Feedback Right", //  Delay 2
            "Link Time", //  Delay 2
            "Link FB", //  Delay 2
            "Repitch", //  Delay 2
            "Ping pong", //  Delay 2
            "FX Type", //  FX select
            "Output", //  Delay Input Gain
            "Size", //  Reverb
            "Width", // Reverb
            "Damping", // Reverb
            "Dry", //  Reverb
            "Reverb", //  Reverb
            "Freeze", //  Reverb
            /* Moog Filter */
            "Frequency", //  Filter
            "Resonans", //  Filter
            "Drive", //  Filter
            "Filter Type", //  Filter
            
            
            "Time Left", // Crazy delay 1
            "Time Right", //  Crazy delay 1
            "Feedback Left", //  Crazy delay 1
            "Feedback Right", //  Crazy delay 1
            "Link Time", //
            "Link FB", //
            "Repitch", //  delay 1
            //"Rate", //  ModLFO
            //"Amount", //  ModLfo
            "Time Mode", // Delay1
            "Time Mode", // Delay2 MAIN
            //"Reverb", // DelayLine add reverb
            "Stereo Width", // Chorus center delay/ Phase
            "Feedback", // Chorus feedback
            "Rate", // Phaser rate
            "Depth", // Phaser depth
            "Frequency", // Phaser
            "Feedback", // Phaser
            "Phaser", // Phaser
            //"Phaser", // DelayLine add phaser
            "Time Mode", // Delay2 MAIN 2' RIGHT
            "Ping pong", // Delay2 MAIN
            "Noise", // Bit Crusher
            "Downsample", // Bit Crusher
            "Resolution", // Bit Crusher
            "BitCrusher", // Bit Crusher
            //"R+L", // Main DelayLine
            //"L+R", // Main DelayLine
            "All -> FX", // process dry+delay ->FX
            "Dual Parallel", // process  delay in parallel
            "Threshold", // Ducking Threshold 1
            "Amount", // Ducking Amount 1
            "Speed", // Ducking speed 1
            "Release", // Ducking Release 1
            "Offset",  // mFXParameter_Main_Delay_Wide,
            "Echo", //  Global Wet
            "Dry", //  Global Wet
            "Offset", //  Dual delay wide
            "Ducking", //  Dual Ducking Mode Toggle button
            "FX Levels", // Mode Toggle button,
            "Mix", // Mode Toggle button, wet/dry
            "Lowcut", //delay1"
            "Lowcut", //delay2"
            "Highcut", //delay1"
            "Highcut", //delay2"
            "Shaper",
            "Stereo Width",
            "MS",
            "Threshold 2", // Ducking Threshold parallel
            "Amount 2", // Ducking Amount parallel
            "Speed 2", // Ducking Attack parallel
            "Release 2", // Ducking Release parallel
            
            /* Advanced EnvF */
            "Sensitivity",
            "Attack",
            "Release",
            "Target 1",
            "Amount",
            "Target 2",
            "Amount",
            /* Advanced LFO1 */
            "Sync",
            "Rate",
            "Note",
            "Type",
            "Target 1",
            "Amount",
            "Target 2",
            "Amount",
            /* Advanced LFO2 */
            "Sync",
            "Rate",
            "Note",
            "Type",
            "Target 1",
            "Amount",
            "Target 2",
            "Amount",
            /*-------------Delay 1 Fade Slider and related------*/
            "Fade",
            "IsFade",  // not displayed!
            
            /*-------------Delay 2 Fade Slider and related------*/
            "Fade",
            "IsFade", // not displayed!
            
            /*--MIDI Panel ADSR Slider and related------*/
            "Attack",
            "Decay",
            "Sustain",
            "Release",
            "Target 1",
            "Amount", // Target 1
            "Target 2",
            "Amount", // Target 2

            /*--midi panel adsr 2 slider and related------*/
            "attack",
            "decay",
            "sustain",
            "release",
            "target 1",
            "amount", // target 1
            "target 2",
            "amount", // target 2

            /*-------------midi input LFO retrigger, not visible------*/
            "midi_lfo1_retrigger",
            "midi_lfo2_retrigger",
            
        };
        
        
        
    };
    
    inline UIPersistentData& getAccessToUIPersistentData() noexcept
    {
        return mUIData;
    }
    

    //----------------------------------------------------License logic--------------------------------------------------------------------------

    
    // https://discord.com/channels/382895736356077570/382908951324262400/1018738139348619264
    //https://help.gumroad.com/article/76-license-keys
    // https://app.gumroad.com/api#
    //Important: Our license key verification API now requires the product_id parameter instead of product_permalink for all products created on or after Jan 9, 2023.

    inline bool checkLicenseKey(juce::String key)
    {
        juce::StringPairArray responseHeaders;
        int statusCode = 0;

        juce::URL url = juce::URL("https://api.gumroad.com/v2/licenses/verify") 
            .withParameter("license_key", key);

        if (auto stream = url.withPOSTData("Deleted")     //  Deleted product_id from code before uploading to github !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            .createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress) 
                .withConnectionTimeoutMs(10000)
                .withResponseHeaders(&responseHeaders)
                .withStatusCode(&statusCode)))
        {
            if (statusCode != 0)
            {
                auto resultJSON = juce::JSON::fromString(stream->readEntireStreamAsString());
                const auto success = resultJSON["success"]; 

                //DBG("[LICENSE KEY] Successful connection, status code = " + juce::String(statusCode)); // 404 product not exist, 200 all ok.
                
                //DBG("Read1: " << resultJSON["uses"].toString());
                
                if (success.isBool())
                    return (bool)success;
                
              
            }
            
            

        }

        //DBG("[LICENSE KEY] Failed to connect, status code = " + juce::String(statusCode));
        return false;
    }

   


    inline void saveUnlockData(bool isVerified)
    {

        File unlockFile = File(mAppUnlockDataDirectory + directorySeparator + "Data" + directorySeparator + "init" + ".ldata");

        

        if (!unlockFile.exists())
        {
            unlockFile.create();
        }
        else 
        {
            unlockFile.deleteFile();
        }

        MemoryBlock destinationData;

        juce::ValueTree paramTree("Data");
        paramTree.setProperty("Value", static_cast<int>(isVerified), nullptr);
        
       
        juce::ValueTree pluginUnlockState("TimeRazor");
        pluginUnlockState.appendChild(paramTree, nullptr);


        juce::AudioPluginInstance::copyXmlToBinary(*pluginUnlockState.createXml(), destinationData);




        unlockFile.appendData(destinationData.getData(),
                              destinationData.getSize());

        // Then do RSA encryption on data

  
    }


    inline bool readUnlockedStatus()
    {
        File data(mAppUnlockDataDirectory + directorySeparator + "Data" + directorySeparator + "init" + ".ldata");

        if (data.exists())
        {
            // Then do RSA decryption on data

            MemoryBlock dataBinary;

            // check file not empty
            if (data.loadFileAsData(dataBinary)) {

                auto xml = juce::AudioPluginInstance::getXmlFromBinary(dataBinary.getData(), static_cast<int>(dataBinary.getSize()));

                if (xml != nullptr)
                {
                    auto tree = juce::ValueTree::fromXml(*xml);
                    // no need to iterate as there are only one child.
                    auto child = tree.getChildWithName("Data");

                    int unlockValue = child["Value"]; 
                    mIsUnlocked = static_cast<bool>(unlockValue);

                    return static_cast<bool>(unlockValue);
                }
                


            }
            

        }
        
        mIsUnlocked = static_cast<bool>(0);

        return static_cast<bool>(0);
        

    }

    inline void setDemoMode(bool mode)
    {
        mIsDemoMode = mode;
    }

    inline bool getDemoMode()
    {
        return mIsDemoMode;
    }


    //----------------------------------------------------End License logic--------------------------------------------------------------------------


    // used by WrappedRasterAudioProcessorEditor to resize UI
    inline double getResizeFactor()
    {
        return mResizeFactor;
    }

    inline void setResizeFactor(double value)
    {
        mResizeFactor = value;
    }


    inline bool isMidiOn()
    {

        return mData.mIsMidi;
    }


private:
    //==============================================================================
    /* Internal */
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    
    
    //AudioListener==================================================================
    /*
    void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override;
    
    void audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details) override;*/
    // ==============================================================================
    
    void initializeDSP();
    
    
    
    void getDawPlayHeadInfo();
    
    void setDelay1Parameters();
    
    void modulateDelay1Parameters(int processLoopIndex);
    
    void setDelay2Parameters();
    
    void modulateDelay2Parameters(int processLoopIndex);
    
    void procesDualFilterParallel();
    
    void procesDualFilterSerial();
    
    void setDuckingParameters();
    
    void setDuckingParallelParameters();
    
    void setGlobalDryWetParameters(int processLoopIndex);
    
    void setChorusParameters();
    
    void setBitcrusherParameters();
    
    void setReverbParameters();
    
    void setPhaserParameters();
    
    void setEnvelopeFollowerParameters();
    
    void setLFO1Parameters();
    
    void setLFO2Parameters();
    
    void updateModulationTargetData();
    
    void setMoogFilterParameters();
    
    void setStereoWidth();
    
    void modulateStereoWidth(int processLoopIndex);
    
    //---LevelMeter value processing stuff ----------------------------------------------------------------------------------------
    void processLevelValue(LinearSmoothedValue<float>&, const float value) const;
    
    
    // In Level
    std::vector<LinearSmoothedValue<float>> rmsLevelsIn;
    Fifo rmsFifoIn;
    AudioBuffer<float> rmsCalculationBufferIn;
    
    // Out Level
    std::vector<LinearSmoothedValue<float>> rmsLevelsOut;
    Fifo rmsFifoOut;
    AudioBuffer<float> rmsCalculationBufferOut;
    
    // used by both in/out
    int rmsWindowSize = 50;
    //double sampleRate = 44'100.0;
    bool isSmoothed = true; 
    //-------------------------------------------------------------------------------------------
    

    

    std::unique_ptr<MFXGain> mInputGain[2];
    
    
    
    std::unique_ptr<MFXBitCrusher> mBitCrusher;
    std::unique_ptr<MFXReverb> mReverb;
    std::unique_ptr<MFXMoogFilter> mMoogFilter;
    std::unique_ptr<MFXSimpleFilter> mLowCutFilters[2];
    std::unique_ptr<MFXSimpleFilter> mHighCutFilters[2];
    std::unique_ptr<MFXDelayLine> mDelayLine1;
    std::unique_ptr<MFXDelayLine> mDelayLine2;
    std::unique_ptr<MFXStereo> mStereo;
    std::unique_ptr<MFXGain> mOutputGain[2];
    std::unique_ptr<MFXChorus> mChorus2;
    

    
    
    
    
    
    
    // Part of Modulation System. System is basically modulation sources(lfo,env) and a struct(tuple) of ModulationTargetData. These are updated by block in this class.
    // The content of this is accessed from dsp code (delay, filter....) and used to modulate params by sample.
    ModulationData mData;
    
    
    
    // Timing stuff. TODO::delete unused
    double mSampleTimeOneBeat{0.0};
    double mRelativePosition {0.0};
    double mBpm{120.0};
    double mCurrentPositionLFO1{0.0};
    double mCurrentPositionLFO2{0.0};
    double mTimeInSamples{0.0};
    bool mIsPlaying = false;
    bool mIsLooping = false;
    bool mResetLFO1PhaseOnStart {false};
    bool mResetLFO2PhaseOnStart {false};
    
    float mLastNoteLfo1 {22.0f};
    float mLastNoteLfo2 {22.0f};
    
    // Use when LFO's is modulated and DAW is not playing. Keep track of free running lfo phase when lfo is reset. TODO: wrap around 0/reset
    float mSumBlokData {0.0f};

    
    //-----------------------------------------------------------------------------------------------------------------------------------
    
    
    
    
    // MIX controls
    std::atomic<float>* mStereoWidth;
    //std::atomic<float>* mMidSide;
    std::atomic<float>* mGlobalDry;
    std::atomic<float>* mGlobalWet;
    
    // Bitcrusher controls
    std::atomic<float>* mResolution;
    std::atomic<float>* mDownsample;
    std::atomic<float>* mNoise;
    std::atomic<float>* mBitCrusherDryWetMix; 
    float mmBitCrusherActiveWet{ 0.0f };


    // Chorus
    float mChorusModulateWet{ 0.0f };

    //Phaser
    float mPhaserModulateWet{ 0.0f };

    // Reverb
    float mReverbModulateWet{ 0.0f };
    
    // Delay1 controls
    std::atomic<float>* mDelay1LeftTime;
    std::atomic<float>* mDelay1RightTime;
    std::atomic<float>* mDelay1LinkTime;
    std::atomic<float>* mDelay1LeftFeedback;
    std::atomic<float>* mDelay1RightFeedback;
    std::atomic<float>* mDelay1Width;
    std::atomic<float>* mSmoothingDelay1;
    std::atomic<float>* mFadeFrequencyDelay1;
    std::atomic<float>* mIsFadeModeDelay1;
    std::atomic<float>* mTimeModeLeftDelay1; 
    std::atomic<float>* mTimeModeRightDelay1;
    std::atomic<float>* mPingPongDelay1; 
    
  
    
    
    
    
    // Delay2 controls
    std::atomic<float>* mDelay2LeftTime;
    std::atomic<float>* mDelay2RightTime;
    std::atomic<float>* mDelay2LinkTime;
    std::atomic<float>* mDelay2LeftFeedback;
    std::atomic<float>* mDelay2RightFeedback;
    std::atomic<float>* mDelay2Width;
    std::atomic<float>* mMixDelay2;
    
    
    std::atomic<float>* mSmoothingDelay2;
    std::atomic<float>* mFadeFrequencyDelay2;
    std::atomic<float>* mIsFadeModeDelay2;
    
    std::atomic<float>* mTimeModeLeftDelay2; 
    std::atomic<float>* mTimeModeRightDelay2;
    std::atomic<float>* mPingPongDelay2; 
    std::atomic<float>* mChainModeDelay2;
    
    
    // MIDI ADSR parameter controls

    std::atomic<float>* mAttack;
    std::atomic<float>* mDecay;
    std::atomic<float>* mSustain;
    std::atomic<float>* mRelease;

    std::atomic<float>* mADSRTarget1;
    std::atomic<float>* mADSRTarget1_Amount;
    std::atomic<float>* mADSRTarget2;
    std::atomic<float>* mADSRTarget2_Amount;


    // MIDI ADSR 2 parameter controls

    std::atomic<float>* mAttack2;
    std::atomic<float>* mDecay2;
    std::atomic<float>* mSustain2;
    std::atomic<float>* mRelease2;

    std::atomic<float>* mADSR2Target1;
    std::atomic<float>* mADSR2Target1_Amount;
    std::atomic<float>* mADSR2Target2;
    std::atomic<float>* mADSR2Target2_Amount;
    

    // MIDI LFO retrigger parameter controls
    std::atomic<float>* mIsLFO1Retrigger;
    std::atomic<float>* mIsLFO2Retrigger;


    
    
    juce::SmoothedValue<double> mSmoothingGlobalDry;
    juce::SmoothedValue<double> mSmoothingGlobalWet;
    
    juce::SmoothedValue<double> mSmoothingDelay2Mix;
    
    
    
    
    
   
    
    
    
    
    //-----------------------------------------------

    std::unique_ptr<MFXPresetManager> mPresetManager;
    
   
    
    
    AudioBuffer<float> mWetBuffer;
    AudioBuffer<float> mCopyBuffer; // used by MS
    
    
    
    // Used to check if incoming audio is mono/stereo 
    float mDifference {0.0f};
    
    
    
    juce::SmoothedValue<double> mChorusSmoothDelayTime;
    
    
    
    
    juce::dsp::Phaser<float> mJPhasor;
    // test
    float mPhaserWetSmooth {0.0f};
    
    
    MFXDucking<double> mDucking;
    MFXDucking<double> mDuckingParallel;
    
    
    bool mClearBuffer {static_cast<bool>(0.0f)};
    
    float mGainMappedWet{1.0f}, mGainMappedDry{1.0f};
    
    
    
   
    
    //test  DELETE
    bool mIsmodulated = false;
    
    
    bool mIsActive {true};
    
    
    // UI related data that need to keep state regardless if UI gets destroyed(window closed).
    UIPersistentData mUIData;

    // License variables
    String mAppUnlockDataDirectory;
    bool mIsUnlocked{false};
    bool mIsDemoMode{ false };
    int mSampleTimeCounter{0};


    // UI resize parameter
    double mResizeFactor{ 1.2 };


   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
