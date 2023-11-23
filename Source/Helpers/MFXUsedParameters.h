/*
  ==============================================================================

    MFXUsedParametersTest.h
    
    Author:  Michael Kauffmann
 
    To use global variables, #include "MFXUsedParametersTest.h"
 
    I really wish that I had orgainized /naming better from the start. A hard reminder - PLAN your work. But not always easy as changes/new functions are added to project during development.
    But it is what it is. Class is not for reuse, and since I'm the only one to maintain and extend it should be fine. But never again this mess!
    If I change order in enum all arrays (ID,Label, DefaultValue...) must match this order. But as long as this order match there will be no problems in my code elsewhere.
    It is time consuming and risk of error to do this, but can be done.

    In future projects I can use a unordered_map. I keep MFXParameter enum for unique key(int) and then use different map<int,String>
    containing ID, labels....all share same unique key. E.g. mapID(mFXParameter_ChorusRate, "name"), mapLabel(mFXParameter_ChorusRate, "label"). Search/access are O(1).
    Then order doesn't matter. Easier to maintain adding new parameters. If I want to change key names in enum I can rename this here and all reference will resonate change.
    And I can simply re-order the keys here, maps will still work. 
    
    TODO::Consider refactor this. I hate look at this.

    Reason having arrays that can be accessed by enum is to eliminate errors. You should not write the name of the ID or label... each time you need to use it in code - high risk of error.
    In this project I use ID, labels.... multiple times, so this is a great solution.  

  ==============================================================================
*/

#pragma once



#include <JuceHeader.h>

    // This is used to unified access index in arrays MFXParameterID,Label...
enum MFXParameter
{
    mFXParameter_InputGain = 0,
    mFXParameter_ChorusRate,
    mFXParameter_ChorusDepth,
    mFXParameter_ChorusWetDry,
    // -----------------------
    
    mFXParameter_DelayTime, // delay 2 rename to Delay2TimeLeft
    mFXParameter_DelayFeedback, // rename to mFXParameter_Delay2LeftFeedback
    mFXParameter_DelayWetDry, 
    
    
    mFXParameter_Delay2TimeRight,
    
    
    mFXParameter_Delay2TimeModeRight,
    
    mFXParameter_Delay2RightFeedback,
    mFXParameter_Delay2Link,
    mFXParameter_Delay2LinkFeedback,
    mFXParameter_Delay2Smoothing,
    
    mFXParameter_Delay2_Pingpong,
    
    //----------
    mFXParameter_FXType, 
    mFXParameter_OutputGain,
    mFXParameter_ReverbSize,
    mFXParameter_ReverbWidth,
    mFXParameter_ReverbDamping,
    mFXParameter_ReverbDry,
    mFXParameter_ReverbWet,
    mFXParameter_ReverbFreeze,
    /* Moog Filter*/
    mFXParameter_FilterCutoff,
    mFXParameter_FilterResonans,
    mFXParameter_FilterDrive,
    mFXParameter_FilterType,

    mFXParameter_Delay1LeftDelayTime, // delay 1 mFXParameter_DlineLeftDelayTime
    mFXParameter_Delay1RightDelayTime, // mFXParameter_DlineRightDelayTime
    mFXParameter_DlineLeftFeedback,
    mFXParameter_DlineRightFeedback,
    mFXParameter_DlineLink,
    mFXParameter_DlineLinkFeedback,
    mFXParameter_DlineSmoothing,
    //mFXParameter_ModLfoRate, // delete
    //mFXParameter_ModLfoDepth, // delete
    mFXParameter_DelayTimeMode1, // delay 2 left mode
    mFXParameter_DelayTimeMode2,
    mFXParameter_ChorusCenterDelay,
    mFXParameter_ChorusFeedback,
    mFXParameter_PhaserRate,
    mFXParameter_PhaserDepth,
    mFXParameter_PhaserCenterFrequency,
    mFXParameter_PhaserFeedback,
    mFXParameter_PhaserDryWet,
    mFXParameter_DelayTimeMode3, // delay 1 right mode
    mFXParameter_Pingpong,
    mFXParameter_NoiseAmount,
    mFXParameter_RateRedux,
    mFXParameter_BitRedux,
    mFXParameter_BitDryWet,
    //mFXParameter_DelayLineMorph_L_To_R, // delete
    //mFXParameter_DelayLineMorph_R_To_L, // delete
    mFXParameter_AllFxProcessing,
    mFXParameter_Parallel_Mode,
    mFXParameter_Ducking_Threshold, // Delay 1/serial all
    mFXParameter_Ducking_Amount,
    mFXParameter_Ducking_Attack,
    mFXParameter_Ducking_Release,
    mFXParameter_Main_Delay_Wide, // delay1
    mFXParameter_Global_Wet,
    mFXParameter_Global_Dry,
    mFXParameter_Delay_Wide,  // delay2
    mFXParameter_Ducking_Mode,
    mFXParameter_FX_Level_Mode,
    mFXParameter_Mix_Mode,
    mFXParameter_Lowcut_Delay1,
    mFXParameter_Lowcut_Delay2,
    mFXParameter_Highcut_Delay1,
    mFXParameter_Highcut_Delay2,
    mFXParameter_Cut_Mode,
    mFXParameter_Stereo_Width,
    mFXParameter_Mid_Side_Mode,
    mFXParameter_Ducking_Threshold_Parallel,
    mFXParameter_Ducking_Amount_Parallel,
    mFXParameter_Ducking_Attack_Parallel,
    mFXParameter_Ducking_Release_Parallel,
    /* -------- Advanced Panel------------- */
    mFXParameter_Advanced_EnvF_Sensitivity,
    mFXParameter_Advanced_EnvF_Attack,
    mFXParameter_Advanced_EnvF_Release,
    mFXParameter_Advanced_EnvF_Target1,
    mFXParameter_Advanced_EnvF_Target1_Amount,
    mFXParameter_Advanced_EnvF_Target2,
    mFXParameter_Advanced_EnvF_Target2_Amount,
    
    
    mFXParameter_Advanced_LFO1_Sync,
    mFXParameter_Advanced_LFO1_Rate,
    mFXParameter_Advanced_LFO1_Note,
    mFXParameter_Advanced_LFO1_Type,
    mFXParameter_Advanced_LFO1_Target1,
    mFXParameter_Advanced_LFO1_Target1_Amount,
    mFXParameter_Advanced_LFO1_Target2,
    mFXParameter_Advanced_LFO1_Target2_Amount,
    
    
    mFXParameter_Advanced_LFO2_Sync,
    mFXParameter_Advanced_LFO2_Rate,
    mFXParameter_Advanced_LFO2_Note,
    mFXParameter_Advanced_LFO2_Type,
    mFXParameter_Advanced_LFO2_Target1,
    mFXParameter_Advanced_LFO2_Target1_Amount,
    mFXParameter_Advanced_LFO2_Target2,
    mFXParameter_Advanced_LFO2_Target2_Amount,

    /*-------------Fade Slider and related------*/
    mFXParameter_FadeFrequency_Delay1,
    mFXParameter_IsFadeMode_Delay1,
    mFXParameter_FadeFrequency_Delay2,
    mFXParameter_IsFadeMode_Delay2,
    

    /*-------------MIDI Panel ADSR Slider and related------*/
    mFXParameter_Midi_ADSR_Attack, 
    mFXParameter_Midi_ADSR_Decay,
    mFXParameter_Midi_ADSR_Sustain,
    mFXParameter_Midi_ADSR_Release,
    mFXParameter_Midi_ADSR_Target1,
    mFXParameter_Midi_ADSR_Target1_Amount,
    mFXParameter_Midi_ADSR_Target2,
    mFXParameter_Midi_ADSR_Target2_Amount,

    /*-------------midi panel adsr 2 slider and related------*/
    mFXParameter_Midi_ADSR2_Attack, 
    mFXParameter_Midi_ADSR2_Decay,
    mFXParameter_Midi_ADSR2_Sustain,
    mFXParameter_Midi_ADSR2_Release,
    mFXParameter_Midi_ADSR2_Target1,
    mFXParameter_Midi_ADSR2_Target1_Amount,
    mFXParameter_Midi_ADSR2_Target2,
    mFXParameter_Midi_ADSR2_Target2_Amount,

    /*-------------midi input LFO retrigger------*/
    mFXParameter_Midi_LFO1_Retrigger,
    mFXParameter_Midi_LFO2_Retrigger,


    mFXParameter_TotalNumParameters
    
};

enum MFXDelayTimeMode
{
    mFXDelayTimeMode_ms = 0,
    mFXDelayTimeMode_Straight,
    mFXDelayTimeMode_Dotted,
    mFXDelayTimeMode_Triplet,
};

// Use to identify modulation source various places in code
enum Modulators
{
    LFO1 = 0,
    LFO2,
    EnvF,
    adsr,
    adsr2,
    DRY
};

// Use to identify modulation targets various places in code doing modulation to avoid string comparison in loop numSamples.
// Must match String array audioparameterChoice in pluginProcessor/createLayout() and LFO, enf targets (work around with wildcard...) 

enum ModulationTargetIndex
{
    None = 0,
    Delay1_Left,
    Delay1_Right,
    Delay1_LR,
    Delay1_Offset,
    Feedback1_Left,
    Feedback1_Right,
    Feedback1_LR,
    Delay2_Left,
    Delay2_Right,
    Delay2_LR,
    Delay2_Offset,
    Feedback2_Left,
    Feedback2_Right,
    Feedback2_LR,
    FilterCutoff,
    FilterResonans,
    FilterDrive,
    WetOut,
    DryOut,
    StereoWidth,
    Delay2_Wet,
    ChorusWet,
    PhaserWet,
    BitCrusherWet,
    BitCrusherBit,
    BitCrusherRate,
    BitCrusherNoise,
    ReverbWet,
    ReverbSize,
    ReverbWidth,
    LFO1_Rate,
    EnvelopeSensitivity,
    Wildcard // 33
};


// http://cabanasdonlalo.com/yb5kjb/global-variable-without-extern.html
// When multiple instances is loaded in DAW - they interfere. But this is read only and call/return function, so ok

extern const juce::String MFXParameterID [mFXParameter_TotalNumParameters];

extern const juce::String MFXParameterLabel [mFXParameter_TotalNumParameters];

extern float MFXParameterDefaultValue [mFXParameter_TotalNumParameters];

// used to set helperUIText.
extern const juce::String MFXParameterHelpText [mFXParameter_TotalNumParameters];

// call/return value (used by PluginProcessor delay Left Right). No need to extern this. Consider put in MFXHelperFunctions instead.
String setValueToNoteText (float value, float& currentTimeModeIndex) noexcept;
    





    
