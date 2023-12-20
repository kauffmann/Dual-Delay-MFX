/*
  ==============================================================================

    MFXUsedParametersTest.h
    
    Author:  Michael Kauffmann
 
    To use global variables, #include "MFXUsedParametersTest.h"
 

  ==============================================================================
*/

#pragma once



#include <JuceHeader.h>

// This is used to unified access index in arrays MFXParameterID,Label..from multiple locations. Using this makes code easier to read/maintain 
enum MFXParameter
{
    /* Gain In/Out ---------*/
    mFXParameter_InputGain = 0,
    mFXParameter_OutputGain,
    
    /* Chorus ---------*/
    mFXParameter_ChorusRate,
    mFXParameter_ChorusDepth,
    mFXParameter_ChorusWetDry,
    mFXParameter_ChorusWidth,
    mFXParameter_ChorusFeedback,
    // -----------------------
    
    /* Delay 2 ---------*/
    mFXParameter_Delay2TimeLeft,
    mFXParameter_Delay2TimeRight,
    mFXParameter_Delay2LeftFeedback,
    mFXParameter_Delay2RightFeedback,
    mFXParameter_Delay2WetDry, 
    mFXParameter_Delay2TimeModeLeft,
    mFXParameter_Delay2TimeModeRight,
    mFXParameter_Delay2LinkTime,
    mFXParameter_Delay2LinkFeedback,
    mFXParameter_Delay2Repitch,
    mFXParameter_FadeFrequency_Delay2,
    mFXParameter_IsFadeMode_Delay2,
    mFXParameter_Delay2_Pingpong,
    mFXParameter_Delay2_Offset,

    
    // Select FX Combobox----------
    mFXParameter_FXType, 
    
    /* Reverb ---------*/
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

    /* Delay 1 ---------*/
    mFXParameter_Delay1TimeLeft, 
    mFXParameter_Delay1TimeRight,
    mFXParameter_Delay1LeftFeedback,
    mFXParameter_Delay1RightFeedback,
    mFXParameter_Delay1TimeModeLeft,
    mFXParameter_Delay1TimeModeRight,
    mFXParameter_Delay1LinkTime,
    mFXParameter_Delay1LinkFeedback,
    mFXParameter_Delay1_Repitch,
    mFXParameter_FadeFrequency_Delay1,
    mFXParameter_IsFadeMode_Delay1,
    mFXParameter_Delay1Pingpong,
    mFXParameter_Delay1_Offset,
    
    
    
    /* Phaser ---------*/
    mFXParameter_PhaserRate,
    mFXParameter_PhaserDepth,
    mFXParameter_PhaserCenterFrequency,
    mFXParameter_PhaserFeedback,
    mFXParameter_PhaserDryWet,


    /* Bit Chrusher ---------*/
    mFXParameter_NoiseAmount,
    mFXParameter_RateRedux,
    mFXParameter_BitRedux,
    mFXParameter_BitDryWet,
    
    /* Center Global Menu ---------*/
    mFXParameter_AllFxProcessing,
    mFXParameter_Parallel_Mode,
    mFXParameter_Ducking_Mode,
    mFXParameter_Mix_Mode,
    mFXParameter_FX_Level_Mode,
    mFXParameter_Cut_Mode,
    

    /* Global Mix ---------*/
    mFXParameter_Global_Wet,
    mFXParameter_Global_Dry,
    mFXParameter_Stereo_Width,
    mFXParameter_Mid_Side_Mode,

    
    /* Ducking single/serial. If parallel mode then delay 1 is controlled ---------*/
    mFXParameter_Ducking_Threshold, // Delay 1/serial all
    mFXParameter_Ducking_Amount,
    mFXParameter_Ducking_Attack,
    mFXParameter_Ducking_Release,
   
    /* Ducking parallel delay 2 is controlled ---------*/
    mFXParameter_Ducking_Threshold_Parallel,
    mFXParameter_Ducking_Amount_Parallel,
    mFXParameter_Ducking_Attack_Parallel,
    mFXParameter_Ducking_Release_Parallel,

    /* Shaper ---------*/
    mFXParameter_Lowcut_Delay1,
    mFXParameter_Lowcut_Delay2,
    mFXParameter_Highcut_Delay1,
    mFXParameter_Highcut_Delay2,     
    
    
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

//extern float MFXParameterDefaultValue [mFXParameter_TotalNumParameters];

// used to set helperUIText.
extern const juce::String MFXParameterHelpText [mFXParameter_TotalNumParameters];

// call/return value (used by PluginProcessor delay Left Right). No need to extern this. Consider put in MFXHelperFunctions instead.
String setValueToNoteText (float value, float& currentTimeModeIndex) noexcept;
    





    
