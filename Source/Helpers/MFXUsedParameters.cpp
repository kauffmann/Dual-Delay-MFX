/*
  ==============================================================================

    MFXUsedParametersTest.cpp
    
    Author:  Michael Kauffmann
 
   

  ==============================================================================
*/

#include "MFXUsedParameters.h"



const juce::String MFXParameterID[] =
{
    /* ----------------------------------------Gain In/Out -----------------------------------------------------*/
    "inputgain",
    "outputgain",
    /* -----------------------------------------Chorus ---------------------------------------------------------*/
    "chorusrate",
    "chorusdepth",
    "choruswetdry",
    "chorus_width",
    "chorus_feedback",

    /* ------------------------------------------Delay 2 --------------------------------------------------------*/
    "delay2_time_left", 
    "delay2_time_right",
    "delay2_feedback_left",
    "delay2_feedback_right",
    "delay2_wetdry",   
    "delay2_time_mode_left",
    "delay2_time_mode_right",
    "delay2_link_time",
    "delay2_link_feedback",
    "delay2_repitch",
    "fade_frequency_delay2",
    "fade_is_fade_delay2",
    "delay2_pingpong",
    "delay2_offset",
    
    /* ------------------------------------------Select FX Combobox----------------------------------------------*/

    "fxtype",
    
    /* --------------------------------------------Reverb -------------------------------------------------------*/

    "reverbsize",
    "reverbwidth",
    "reverbdamping",
    "reverbdry",
    "reverbwet",
    "reverbfreeze",

    /* -----------------------------------------------Moogy Filter -----------------------------------------------*/
    "filtercutoff",
    "filterresonans",
    "filterDrive",
    "filtertype",


    /* ------------------------------------------------Delay 1 (main)---------------------------------------------*/
    "delay1_time_left",  
    "delay1_time_right",
    "delay1_feedback_left",
    "delay1_feedback_right",
    "delay1_time_mode_left",
    "delay1_time_mode_right",
    "delay1_link_time",
    "delay1_link_feedback",
    "delay1_repitch",
    "fade_frequency_delay1",
    "fade_is_fade_delay1",
    "delay1_pingpong",
    "delay1_offset",

    
    /* -------------------------------------------------------Phaser -------------------------------------------------*/
    "phaser_rate",
    "phaser_depth",
    "phaser_centerfrequency",
    "phaser_feedback",
    "phaser_drywet",

    
    
    /* ----------------------------------------------------Bit Chrusher -----------------------------------------------*/
    "noise_amount",
    "rate_redux",
    "bit_redux",
    "bit_drywet",
  
    /* ---------------------------------------------------Center Global Menu -------------------------------------------*/

    "all_fx_processing",
    "parallel_mode",
    "ducking_mode",
    "mix_mode",
    "fx_level_mode",
    "cut_mode",
    
    
    /* ----------------------------------------------------Global Mix ---------------------------------------------------*/

    "global_wet",
    "global_dry",
    "stereo_width",
    "mid_side_mode",

    /* -------------------------------------Ducking single/serial (if parallel mode, delay 1 is controlled)---------------*/
    "ducking_threshold",
    "ducking_amount",
    "ducking_attack",
    "ducking_release",

    /* ----------------------------------------------Ducking parallel control delay 2-------------------------------------*/
    "ducking_threshold_parallel",
    "ducking_amount_parallel",
    "ducking_attack_parallel",
    "ducking_release_parallel",
    
    /* ------------------------------------------------------Shaper LPF/HPF ---------------------------------------------*/
    "lowcut_delay1",
    "lowcut_delay2",
    "highcut_delay1",
    "highcut_delay2",
    
    /* ---------------------------------------------------------Advanced EnvF --------------------------------------------*/
    "advanced_envf_sensitivity",
    "advanced_envf_attack",
    "advanced_envf_release",
    "advanced_envf_target1",
    "advanced_envf_target1_amount",
    "advanced_envf_target2",
    "advanced_envf_target2_amount",
    /* ------------------------------------------------------------Advanced LFO1 -----------------------------------------*/
    "advanced_lfo1_sync",
    "advanced_lfo1_rate",
    "advanced_lfo1_note",
    "advanced_lfo1_type",
    "advanced_lfo1_target1",
    "advanced_lfo1_target1_amount",
    "advanced_lfo1_target2",
    "advanced_lfo1_target2_amount",
    /* ----------------------------------------------------------Advanced LFO2 --------------------------------------------*/
    "advanced_lfo2_sync",
    "advanced_lfo2_rate",
    "advanced_lfo2_note",
    "advanced_lfo2_type",
    "advanced_lfo2_target1",
    "advanced_lfo2_target1_amount",
    "advanced_lfo2_target2",
    "advanced_lfo2_target2_amount",
   
   
   

    /*------------------------------------------------------MIDI Panel ADSR Slider and related------------------------------*/
    "midi_adsr_attack",
    "midi_adsr_decay",
    "midi_adsr_sustain",
    "midi_adsr_release",
    "midi_adsr_target1",
    "midi_adsr_target1_amount",
    "midi_adsr_target2",
    "midi_adsr_target2_amount",

    /*-----------------------------------------------------------MIDI Panel ADSR 2 Slider and related-----------------------*/
    "midi_adsr2_attack",
    "midi_adsr2_decay",
    "midi_adsr2_sustain",
    "midi_adsr2_release",
    "midi_adsr2_target1",
    "midi_adsr2_target1_amount",
    "midi_adsr2_target2",
    "midi_adsr2_target2_amount",
    
    /*-----------------------------------------------------------midi input LFO retrigger-----------------------------------*/
    "midi_lfo1_retrigger",
    "midi_lfo2_retrigger",
        
};


const juce::String MFXParameterLabel[] =
{
    /* ---------------------------Gain In/Out ------------------------------------------------------------------------------*/
    "Input", 
    "Output", 

    /* ----------------------------------------------Chorus ----------------------------------------------------------------*/
    "Rate", 
    "Depth", 
    "Chorus", // wet balance
    "Stereo Width",
    "Feedback",

    /* -----------------------------------------------Delay 2 --------------------------------------------------------------*/
    "Time Left", 
    "Time Right", 
    "Feedback Left", 
    "Feedback Right", 
    "Delay 2", // wet
    "Time Mode", //  Delay 2 left
    "Time Mode", //  Delay 2 right
    "Link Time",
    "Link Feedback",
    "Repitch",
    "Fade", 
    "fade_is_fade", // not displayed
    "Ping pong",
    "Offset",


    /* ------------------------------------------Select FX Combobox--------------------------------------------------------*/

    "FX Type", //  FX select

    /* ----------------------------------------------------Reverb----------------------------------------------------------*/
    
    "Size",
    "Width", 
    "Damping", 
    "Dry", //  Reverb dry
    "Reverb", //  Reverb wet
    "Freeze",
    
    /* ------------------------------------------------------Moog Filter --------------------------------------------------*/
    "Freq", 
    "Resonans", 
    "Drive",
    "Filter Type",
    
    /* -------------------------------------------------------Delay 1 (Main) -----------------------------------------------*/

    "Time Left", 
    "Time Right", 
    "Feedback Left", 
    "Feedback Right", 
    "Time Mode",
    "Time Mode",
    "Link Time",
    "Link Feedback",
    "Repitch",
    "Fade", 
    "fade_is_fade",
    "Ping pong",
    "Offset",


    /* -----------------------------------------------------------------Phaser --------------------------------------------*/

    "Rate",
    "Depth",
    "Frequency",
    "Feedback",
    "Phaser", // Phaser wet
    
    /* ---------------------------------------------------------Bit Chrusher -----------------------------------------------*/

    "Noise",
    "Downsample", 
    "Resolution", 
    "BitCrusher", // Bit Crusher wet
    
    /* -------------------------------------------------------------Center Global Menu -------------------------------------*/

    "All -> FX", 
    "Parallel",
    "Ducking",
    "Mix",
    "FX Mix",
    "Shaper",

    /* -------------------------------------Global Mix (dry wet(delay) audio)------------------------------------------------*/

    "Echo",
    "Dry", 
    "Stereo Width",
    "MS",

   
    /*---------------------------------Ducking single/serial (if parallel mode, delay 1 is controlled)-----------------------*/

    "Threshold",
    "Amount",
    "Speed",
    "Release",  

    /* -----------------------------Ducking parallel mode, delay 2 is controlled)--------------------------------------------*/

    "Threshold 2",
    "Amount 2",
    "Speed 2",
    "Release 2",

    /* ------------------------------------------------------Shaper----------------------------------------------------------*/
    "Lowcut", //delay1"
    "Lowcut 2", //delay2 (if parallel)"
    "Highcut", //delay1"
    "Highcut 2", //delay2 (if parallel) 
   

    /* ------------------------------------------- Advanced Panel-------------------------------------------------------------- */

    /* ----Advanced EnvF --*/
    "Sensitivity",
    "Attack",
    "Release",
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
    /* Advanced LFO1 */
    "Sync",
    "Rate",
    "Note",
    "Type",
    "Target 1",
    "Amount",
    "Target 2",
    "Amount",
   
   
    /*-----------------------------------------------------MIDI Panel ADSR Slider and related---------------------------------*/
    "Attack",
    "Decay",
    "Sustain",
    "Release",
    "Target 1",
    "Amount", // Target 1
    "Target 2",
    "Amount", // Target 2

    /*------------------------------------------------------MIDI Panel ADSR 2 Slider and related-------------------------------*/
    "Attack",
    "Decay",
    "Sustain",
    "Release",
    "Target 1",
    "Amount", // Target 1
    "Target 2",
    "Amount", // Target 2

    /*-----------------------------------------------------------------MIDI input LFO retrigger--------------------------------*/
    "",
    "",
    
    
};




const juce::String MFXParameterHelpText [mFXParameter_TotalNumParameters] =
{
    /* ---------------------------------------Gain In/Out ----------------------------------------------------------------------*/

    "input gain: controls incoming audio level. High levels can change sonic harmonics in your sound",
    "Output gain: controls audio out level",

    /* --------------------------------------Chorus ----------------------------------------------------------------------------*/

    "Chorus rate: controls frequency it sweeps delay time",
    "Depth: sets range in ms delay time sweeps within.",
    "Chorus: sets the amount chorus added", // wet
    "Stereo width: set mono -> stereo width", 
    "FeedBack: amount of audio reinjected into the chorus. Makes sound more resonant, changing sonic character",
    
    /* --------------------------------------Delay 2 ---------------------------------------------------------------------------*/

    #if JUCE_WINDOWS
	"Time Left: hold down ctrl key and mouse drag for smaller values",
	#elif JUCE_MAC
    "Time Left: hold down command key and mouse drag for smaller values",
	#endif

    #if JUCE_WINDOWS
    "Time Right: hold down ctrl key and mouse drag for smaller values",
    #elif JUCE_MAC
    "Time Right: hold down command key and mouse drag for smaller values",
    #endif

    "Feedback left: amount of delay line 2 output audio reinjected into the delay line 2. Have overload protection, but be careful anyway", 
    "Feedback right: amount of delay line 2 output audio reinjected into the delay line 2. Have overload protection, but be careful anyway", 
    "Dual level balance: delays will be processed serial by default. 0-100 % will decrease delay 1 and increase delay 2 level. Parallel mode set delay 2 level", 
    "Time Mode",
    "Time Mode", 
    "Link: sync left and right delay time sliders.Can be used creatively if left/right delay time is different - switching from non-link to link mode",
    "Link: sync left and right feedback sliders", 
    "Repitch mode: control sonic repitch behavior when changing delay time.",
    "Fade mode: control fade mode behavior when changing delay time (crossfade between old and new delay times). It can sound soft, harder or destructive",
    "Nothing",
    "Ping pong: add classic ping pong effect to delay line 2. From mix menu, use stereo width to control it",
    "Offset: set a small gap between left and right time, adding stereo width if initial time levels are identical. If modulated, repitch/fade affect sound",


    /* --------------------------------------FX select combo box --------------------------------------------------------------*/
    "FX Type", 

    /* --------------------------------------Reverb ---------------------------------------------------------------------------*/

    "Size",
    "Width: sets mono to full scale stereo", 
    "Damping: attenuate high frequencies", 
    "Dry",
    "Reverb: sets the amount of reverb added", //  Reverb wet
    "Freeze",

    /* -------------------------------------------Moog Filter -----------------------------------------------------------------*/

    "Frequency: controls cutoff frequency. Multimode filter is always active. Is placed before reverb FX in signal chain", 
    "Resonans", 
    "Drive",
    "Filter Type", 
    
    /* --------------------------------------Delay 1 (Main) --------------------------------------------------------------------*/


     #if JUCE_WINDOWS
    "Time Left: hold down ctrl key for smaller values",
    #elif JUCE_MAC
    "Time Left: hold down command key for smaller values",
    #endif

    #if JUCE_WINDOWS
    "Time Right: hold down ctrl key for smaller values",
    #elif JUCE_MAC
    "Time Right: hold down command key for smaller values",
    #endif

    
    
    "Feedback left: amount of delay line 1 output audio reinjected into the delay line 1. Have overload protection, but be careful anyway", 
    "Feedback right: amount of delay line 1 output audio reinjected into the delay line 1. Have overload protection, but be careful anyway", 
    "Time Mode",
    "Time Mode",
    "Link: sync left and right delay time sliders.  Can be used creatively if left/right times is different - switching from non-link to link mode", 
    "Link: sync left and right feedback sliders", 
    "Repitch mode: control sonic repitch behavior when changing delay time.",
    "Fade mode: control fade mode behavior when changing delay time (crossfade between old and new delay times). It can sound soft, harder or destructive",
    "nothing",
    "Ping pong: add classic ping pong effect to delay line 1. From mix menu, use stereo width to control it", 
    "Offset: set a small gap between left and right time, adding stereo width if initial time levels are identical. If modulated, repitch/fade affect sound", 
    
    /* --------------------------------------Phaser ----------------------------------------------------------------------------*/

    "Rate: controls frequency it sweeps delay time", 
    "Depth: sets percent of full range delay time sweeps within", 
    "Frequency: sets the centre frequency (in Hz). If you can't hear any phaser effect try higher or lower frequency", 
    "Feedback: negative and positive values can be used to make sound more resonant, changing sonic character", 
    "Phaser: sets the amount of phaser added", 

    /* --------------------------------------Bit Chrusher ----------------------------------------------------------------------*/
    
    "Noise: can boost bit resolution effect. Try turn up noise(e.g. 70 percent), then decrease bit resolution until noise is not audible, then play a note", 
    "Downsample: levels above 1 simulate reduced sample rate", 
    "Bit resolution: simulate changing bit depth",
    "Bit Crusher: sets the amount of bit crusher added", 

    /* -------------------------------------- Center Global Menu ---------------------------------------------------------------*/

    "All -> FX: will apply FX, multimode filter to both wet(echo) and dry audio", 
    "Parallel: this will proces delay line 1 and 2 in parallel. Delay line 2 must be activated in FX-level menu", 
    "Ducking: apply ducking to delayed audio. In Parallel each delay line has unique control. Fade up one delay line after the other (set release time)",
    "Mix: balance output of dry and wet(echo) audio. Control stereo width of wet(echo) audio. Control mid/side of wet(echo) audio", 
    "Control FX levels: audio is routed in this order, and multimode filter comes before reverb. A FX is activated by setting FX level > 0",
    "Shaper: control lowpass and a highpass filter applied to delayed audio. If Parallel is selected each delay line has unique control",

    /* -------------------------------------------------Global Mix --------------------------------------------------------------*/

    "Echo: scale output level of delayed audio (delay 1 + delay 2)", //  Global Wet
    "Dry: scale output level of dry audio.", //  Global Dry
    "Stereo Width: control the stereo width of the delayed audio. 0 % is mono, 100 % is normal stereo and 100+ enhance stereo further.",
    "Mid/Side: only works when audio input source is stereo. You can use stereo width to shape stereo image further",
  


    /* -------------------------------------------------Ducking -----------------------------------------------------------------*/

    "Threshold: control audio level ducking react to. Only levels above threshold is affected. For basic ducking there is no need to set the other parameters", // Ducking Threshold 1
    "Amount: control how much the audio above threshold is affected", // Ducking Amount 1
    "Speed: time before ducking hit audio. Normaly you want small time values", // Ducking speed 1
    "Release: time it takes to release ducking applied to audio. Values below 200 ms might cause distortion. This is normal, as it quickly return to full level", // Ducking Release 1
    
    /* -------------------------------------------------Ducking Parallel --------------------------------------------------------*/


    "Threshold: control audio level ducking react to. Only level above threshold is affected. For basic ducking there is no need to set the other parameters", // Ducking Threshold parallel
    "Amount: control how much the audio above threshold is affected", // Ducking Amount parallel
    "Speed: time before ducking hit audio. Normaly you want small time values", // Ducking Attack parallel
    "Release: time it takes to release ducking applied to audio. Values below 200 ms might cause distortion. This is normal, as it quickly return to full level", // Ducking Release parallel    
    

    /* -------------------------------------------------Shaper -----------------------------------------------------------------*/
    
    "lowcut", //delay1"
    "lowcut 2", //delay2"
    "highcut", //delay1"
    "highcut 2", //delay2"
    
    
    
    
    /* -------------------------------------------------------Advanced EnvF ----------------------------------------------------*/
    "Sensitivity: sets sensitivity for reacting to incoming audio. Think of it as threshold where 5.0 correspond to -60 dB",
    "Attack: control how fast target param will react to incoming modulation",
    "Release: control when to release/fade out modulation on target param.",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    
    /* ----------------------------------------------------------Advanced LFO1 -------------------------------------------------*/
    "Sync",
    "Rate: Control the frequency of the LFO",
    "Note: sync to DAW tempo",
    "Type",
    "Target 1",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    "Target 2",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    
    /* ----------------------------------------------------------------Advanced LFO2 -------------------------------------------*/
    "Sync",
    "Rate: control the frequency of the LFO",
    "Note: sync to DAW tempo",
    "Type",
    "Target 1",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    "Target 2",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    
    
    
    /* ----------------------------------------------------------------Advanced MIDI ADSR -------------------------------------*/
    "Attack: control how fast it will reach max level (1.0)",
    "Decay: control how fast it will fall from max level to sustain level",
    "Sustain: control the sustain level to hold",
    "Release: control how fast level will fall to 0.0",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",

    /* ----------------------------------------------------------------Advanced MIDI ADSR 2 -----------------------------------*/
    "Attack: control how fast it will reach max level (1.0)",
    "Decay: control how fast it will fall from max level to sustain level",
    "Sustain: control the sustain level to hold",
    "Release: control how fast level will fall to 0.0",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    
    /*-------------------------------------------------------------------midi input LFO retrigger------------------------------*/
    "If midi input is received you can retrigger LFO 1. If retrigger is on and midi is not received, then LFO is free running.",
    "If midi input is received you can retrigger LFO 2. If retrigger is on and midi is not received, then LFO is free running.",
    
};






String setValueToNoteText (float value, float& currentTimeModeIndex) noexcept
{
   int index = roundToInt(value *= 7);
   if (index == 0)
       index = 1;
    
    
    switch (index)
    {
        case 1: // 1/1
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 2: // 1/2
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/2";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/2.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/2T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 3: // 1/4
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/4";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/4.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/4T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 4: // 1/8
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/8";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/8.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/8T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 5: // 1/16
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/16";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/16.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/16T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 6: // 1/32
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/32";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/32.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/32T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
        case 7: // 1/64
            if (currentTimeModeIndex == mFXDelayTimeMode_Straight)
            {
                return "1/64";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Dotted)
            {
                return "1/64.";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_Triplet)
            {
                return "1/64T";
            }
            
            else if (currentTimeModeIndex == mFXDelayTimeMode_ms)
            {
                return " ms";
            }
            break;
            
        default:
            jassert(currentTimeModeIndex == mFXDelayTimeMode_ms);
            // should't be here
            return "Error";
            
    }
    return "Error"; // why I am I doing this
}

bool isParallelModeOn = false;


bool isSliderTouchedByUser = false;
bool isButtonTouchedByUser = false;
bool isComboBoxTouchedByUser = false;

String helperUIText = "";



