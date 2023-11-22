/*
  ==============================================================================

    MFXUsedParametersTest.cpp
    
    Author:  Michael Kauffmann
 
    I really wish that I had orgainized /naming better from the start.
    But it is what it is. Class is not for reuse, and since I'm the only one to maintain and extend it should be fine.
    In future projects I can use a unordered_map. I keep MFXParameter enum for unique key(int) and then use different map<int,String> containing ID, labels....
    Then order doesn't matter. Easier to maintain adding new parameters.  

  ==============================================================================
*/

#include "MFXUsedParameters.h"



const juce::String MFXParameterID[] =
{
    "inputgain",
    "chorusrate",
    "chorusdepth",
    "choruswetdry",
    "delaytime", // delay 2
    "delayfeedback",
    "delaywetdry",   
    "delay2_time_right",
    "delay2_time_mode_right",
    "delay2_right_feedback",
    "delay2_link",
    "delay2_link_feedback",
    "delay2_smoothing",
    "delay2_pingpong", // delay 2 end
    "fxtype",
    "outputgain",
    "reverbsize",
    "reverbwidth",
    "reverbdamping",
    "reverbdry",
    "reverbwet",
    "reverbfreeze",
    /* Moogy Filter */
    "filtercutoff",
    "filterresonans",
    "filterDrive",
    "filtertype",
    "dlineleftdelaytime",  // Delay 1
    "dlinerightdelaytime",
    "dlineleftfeedback",
    "dlinerightfeedback",
    "dlinelink",
    "dlinefeedbacklink",
    "dlinesmoothing",
    //"modlforate",
    //"modlfodepth",
    "delaytimemode1",
    "delaytimemode2",
    "chorus_center_delay",
    "chorus_feedback",
    "phaser_rate",
    "phaser_depth",
    "phaser_centerfrequency",
    "phaser_feedback",
    "phaser_drywet",
    "delaytimemode3", // Main Right
    "pingpong",
    "noise_amount",
    "rate_redux",
    "bit_redux",
    "bit_drywet",
    //"delayline_morph_left_to_right",
    //"delayline_morph_right_to_left",
    "all_fx_processing",
    "parallel_mode",
    "ducking_threshold",
    "ducking_amount",
    "ducking_attack",
    "ducking_release",
    "main_delay_wide",
    "global_wet",
    "global_dry",
    "delay_wide",
    "ducking_mode",
    "fx_level_mode",
    "mix_mode",
    "lowcut_delay1",
    "lowcut_delay2",
    "highcut_delay1",
    "highcut_delay2",
    "cut_mode",
    "stereo_width",
    "mid_side_mode",
    "ducking_threshold_parallel",
    "ducking_amount_parallel",
    "ducking_attack_parallel",
    "ducking_release_parallel",
    /* Advanced EnvF */
    "advanced_envf_sensitivity",
    "advanced_envf_attack",
    "advanced_envf_release",
    "advanced_envf_target1",
    "advanced_envf_target1_amount",
    "advanced_envf_target2",
    "advanced_envf_target2_amount",
    /* Advanced LFO1 */
    "advanced_lfo1_sync",
    "advanced_lfo1_rate",
    "advanced_lfo1_note",
    "advanced_lfo1_type",
    "advanced_lfo1_target1",
    "advanced_lfo1_target1_amount",
    "advanced_lfo1_target2",
    "advanced_lfo1_target2_amount",
    /* Advanced LFO2 */
    "advanced_lfo2_sync",
    "advanced_lfo2_rate",
    "advanced_lfo2_note",
    "advanced_lfo2_type",
    "advanced_lfo2_target1",
    "advanced_lfo2_target1_amount",
    "advanced_lfo2_target2",
    "advanced_lfo2_target2_amount",
    /*-------------Fade Slider and related------*/
    "fade_frequency_delay1",
    "fade_is_fade_delay1",
    
    "fade_frequency_delay2",
    "fade_is_fade_delay2",

    /*-------------MIDI Panel ADSR Slider and related------*/
    "midi_adsr_attack",
    "midi_adsr_decay",
    "midi_adsr_sustain",
    "midi_adsr_release",
    "midi_adsr_target1",
    "midi_adsr_target1_amount",
    "midi_adsr_target2",
    "midi_adsr_target2_amount",

    /*-------------MIDI Panel ADSR 2 Slider and related------*/
    "midi_adsr2_attack",
    "midi_adsr2_decay",
    "midi_adsr2_sustain",
    "midi_adsr2_release",
    "midi_adsr2_target1",
    "midi_adsr2_target1_amount",
    "midi_adsr2_target2",
    "midi_adsr2_target2_amount",
    
    /*-------------midi input LFO retrigger------*/
    "midi_lfo1_retrigger",
    "midi_lfo2_retrigger",
        
};


const juce::String MFXParameterLabel[] =
{
    "Input", //  Delay Input Gain
    "Rate", //  Chorus
    "Depth", //  Chorus
    "Chorus", // Chorus
    "Time Left", //  Delay 2
    "Feedback Left", //  Delay 2
    "Delay 2", 
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
    "Freq", //  Filter
    "Resonans", //  Filter
    "Drive",
    "Filter Type", //  Filter
    
    
    "Time Left", // delay 1
    "Time Right", //  delay 1
    "Feedback Left", //  delay 1
    "Feedback Right", // delay 1
    "Link Time", //
    "Link FB", //
    "Repitch", // delay 1
    //"Rate", //  ModLFO
    //"Amount", //  ModLfo
    "Time Mode", // Delay1
    "Time Mode", // Delay2 MAIN hmmm
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
    "Parallel", // process  delay in parallel
    "Threshold",
    "Amount",
    "Speed",
    "Release",
    "Offset",  // mFXParameter_Main_Delay_Wide,
    "Echo", //  Global Wet
    "Dry", //  Global Wet
    "Offset", //  Dual delay wide
    "Ducking", //  Dual Ducking Mode Toggle button
    "FX Mix", // Mode Toggle button,
    "Mix", // Mode Toggle button, wet/dry
    "Lowcut", //delay1"
    "Lowcut 2", //delay2"
    "Highcut", //delay1"
    "Highcut 2", //delay2"
    "Shaper",
    "Stereo Width",
    "MS",
    "Threshold 2",
    "Amount 2",
    "Speed 2",
    "Release 2",
    /* Advanced EnvF */
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
    /*-------------Delay 1 Fade Slider and related------*/
    "Fade",  // fade_frequency. 
    "fade_is_fade", // not displayed
    
    /*-------------Delay 2 Fade Slider and related------*/
    "Fade",  //   fade_frequency 
    "fade_is_fade", // not displayed

    /*--MIDI Panel ADSR Slider and related------*/
    "Attack",
    "Decay",
    "Sustain",
    "Release",
    "Target 1",
    "Amount", // Target 1
    "Target 2",
    "Amount", // Target 2

    /*--MIDI Panel ADSR 2 Slider and related------*/
    "Attack",
    "Decay",
    "Sustain",
    "Release",
    "Target 1",
    "Amount", // Target 1
    "Target 2",
    "Amount", // Target 2

    /*-------------midi input LFO retrigger------*/
    "",
    "",
    
    
};

float MFXParameterDefaultValue[] =
{
    0.0f, //  inGain
    0.3f,   //  chorus rate
    0.4f,  //  chorus depth
    0.0f, // chorus dryWet
    0.2f,    // delay 2 left time
    0.5f,   // delay feedback
    0.0f,  // delay drywet  
    0.2f, //  Delay right 2
    0, //  Delay 2 time mode right  
    0.5f, //  Delay 2
    0.0f, //  Delay 2
    0.0f, //  Delay 2
    200.0f, //  Delay 2  Transition smoothing
    0.0f, //  Delay 2
    0.0f, // fxtype
    0.0f,  //  outGain
    0.9f,      //  size Reverb
    0.6f,     // width
    0.2f,    // damp
    0.6f,   // dry  was 0.6
    0.0f,  // wet
    0.0f, // freeze
    
    /* Moog Filter */
    15000.0f, //  filter cutoff
    0.1f,   //  filter resonans
    0.0f,  // filter drive
    0.0f,  //  filter type
    
    0.2f,     //  DelayLine left time
    0.2f,    //  DelayLine right time
    0.5f,       //  DelayLine left feedback
    0.5f,      //  DelayLine left feedback
    0.0f,    //  DelayLine link
    0.0f,   //  DelayLine Link Feedback
    200.0f, //  SmoothingDelayLine Transition smoothing
    //0.2f,   //  Filter Rate LFO
    //0.0f,  //  Filter Depth LFO
    0, // delay1 time mode    TODO: int ?
    0, // delay2 time mode Delay2 MAIN 2' RIGHT
    0.25f, // chorus center delay/ phase
    0.0f, // chorus feedback
    0.4f, // Phaser rate
    0.4f, // Phaser depth
    1500.0f, // Phaser centerfreq
    0.0f, // Phaser feedback
    0.0f, // Phaser dryWet
    //0.0f, // DelayLine add chorus Phaser
    0, // Delay2 Time Mode 2' RIGHT
    0.0f, // Delay2 MAIN Bool
    48.5f, // Bit Crusher noise
    39.0f, // Bit Crusher rate
    4.0f, // Bit Crusher bit
    0.0f, // Bit Crusher drywet
    //0.0f, // Morph L->R DelayLine
    //0.0f, //Morph R->L DelayLine
    0.0f, // Main (delay)UI FX Routing
    0.0f, // Main (delay)UI DUAL serial/parallel Routing
    0.0f, // Ducking Threshold
    5.0f, // Ducking Amount
    0.2f, // Ducking Attack
    700.0f, // Ducking Release
    0.0f, // mFXParameter_Main_Delay_Wide,
    1.0f,     //  Global Wet
    1.0f,     //  Global Dry
    0.0f,     //  Dual delay wide
    0.0f, // Dual ducking
    0.0f, // FX level mode
    0.0f, // Mix mode
    20.0f, // lowcut delay1 (highpass filter)"
    20.0f, //delay2"
    20000.0f, // highcut delay1" (lowpass filter)
    20000.0f, //delay2"
    0.0f, // cut mode
    0.5f, // Stereo wide
    0.0f, // MS
    0.0f, // Ducking Threshold parallel
    5.0f, // Ducking Amount parallel
    0.2f, // Ducking Attack parallel
    700.0f, // Ducking Release parallel
    /* Advanced EnvF */
    5.0f,    //Sensitivity
    0.1f,    //Attack
    280.0f,    //Release
    0.0f,    //Target 1
    0.0f,    //Amount
    0.0f,    //Target 2
    0.0f,    //Amount
    /* Advanced LFO1 */
    0.0f,    //Sync
    0.0f,    //Rate
    0.0f,    // Note
    0.0f,    //Type
    0.0f,    //Target 1
    0.0f,    //Amount
    0.0f,    //Target 2
    0.0f,    //Amount
    /* Advanced LFO2 */
    0.0f,    //Sync
    0.0f,    //Rate
    0.0f,    // Note
    0.0f,    //Type
    0.0f,    //Target 1
    0.0f,    //Amount
    0.0f,    //Target 2
    0.0f,    //Amount
    /*-------------Delay 1 Fade Slider and related------*/
    2.0, // fade freq
    0.0f, // isFadeMode false
    
    /*-------------Delay 2 Fade Slider and related------*/
    2.0, // fade freq
    0.0f, // isFadeMode false

    /*-------------MIDI Panel ADSR Slider and related------*/
    0.1f, // midi_adsr_attack
    0.3f, //   "midi_adsr_decay"
    0.4f, //   "midi_adsr_Sustain",
    0.2f,  //  "midi_adsr_release",
    0.0f,  //  "midi_adsr_target1",
    0.0f,  //  "midi_adsr_target1_amount",
    0.0f,  //  "midi_adsr_target2",
    0.0f,  //  "midi_adsr_target2_amount",

    /*-------------MIDI Panel ADSR 2 Slider and related------*/
    0.1f, // midi_adsr_attack
    0.3f, //   "midi_adsr_decay"
    0.4f, //   "midi_adsr_Sustain",
    0.2f,  //  "midi_adsr_release",
    0.0f,  //  "midi_adsr_target1",
    0.0f,  //  "midi_adsr_target1_amount",
    0.0f,  //  "midi_adsr_target2",
    0.0f,  //  "midi_adsr_target2_amount",

    /*-------------midi input LFO retrigger------*/
    0.0f, //   "mitodi_lfo1_retrigger"
    0.0f, //   "midi_lfo2_retrigger"
    

};





const juce::String MFXParameterHelpText [mFXParameter_TotalNumParameters] =
{
    
    "input gain: controls incoming audio level. High levels can change sonic harmonics in your sound",
    /* Chorus */
    "Chorus rate: controls frequency it sweeps delay time",
    "Depth: sets range in ms delay time sweeps within.",
    "Chorus: sets the amount chorus added", // Chorus
    
    /* Delay 2 */

    #if JUCE_WINDOWS
	"Time Left: hold down ctrl key and mouse drag for smaller values",
	#elif JUCE_MAC
    "Time Left: hold down command key and mouse drag for smaller values",
	#endif

    "Feedback left: amount of delay line 2 output audio reinjected into the delay line 2. Have overload protection, but be careful anyway", //  Delay 2
    "Dual level balance: delays will be processed serial by default. 0-100 % will decrease delay 1 and increase delay 2 level. Parallel mode set delay 2 level", //  Delay 2

    #if JUCE_WINDOWS
    "Time Right: hold down ctrl key and mouse drag for smaller values",
    #elif JUCE_MAC
    "Time Right: hold down command key and mouse drag for smaller values",
    #endif

	
    "Time Mode", //  Delay 2
    "Feedback right: amount of delay line 2 output audio reinjected into the delay line 2. Have overload protection, but be careful anyway", //  Delay 2
    "Link: sync left and right delay time sliders.Can be used creatively if left/right delay time is different - switching from non-link to link mode", //  Delay 2
    "Link: sync left and right feedback sliders", //  Delay 2
    "Repitch mode: control sonic repitch behavior when changing delay time.", //  Delay 2 control sonic behavior when changing delay time param manually/by modulator
    "Ping pong: add classic ping pong effect to delay line 2. From mix menu, use stereo width to control it", //  Delay 2
    "FX Type", //  FX select
    "Output gain: controls audio out level", //  Delay Input Gain
    "Size", //  Reverb
    "Width: sets mono to full scale stereo", // Reverb
    "Damping: attenuate high frequencies", // Reverb
    "Dry", //  Reverb
    "Reverb: sets the amount of reverb added", //  Reverb
    "Freeze", //  Reverb
    /* Moog Filter */
    "Frequency: controls cutoff frequency. Multimode filter is always active. Is placed before reverb FX in signal chain", //  Filter
    "Resonans", //  Filter
    "Drive", //  Filter
    "Filter Type", //  Filter
    
    /* Delay 1 */

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

    
    
    "Feedback left: amount of delay line 1 output audio reinjected into the delay line 1. Have overload protection, but be careful anyway", //  Crazy delay
    "Feedback right: amount of delay line 1 output audio reinjected into the delay line 1. Have overload protection, but be careful anyway", //  Crazy delay
    "Link: sync left and right delay time sliders.  Can be used creatively if left/right times is different - switching from non-link to link mode", //
    "Link: sync left and right feedback sliders", //
    "Repitch mode: control sonic repitch behavior when changing delay time.", //  Crazy delay
    
    //"Rate", //  ModLFO
    //"Amount", //  ModLfo
    
    "Time Mode", // Delay1
    "Time Mode", // Delay2 MAIN
    "Stereo width: set mono -> stereo width", // Chorus Phase
    "FeedBack: amount of audio reinjected into the chorus. Makes sound more resonant, changing sonic character", // Chorus feedback
    "Rate: controls frequency it sweeps delay time", // Phaser rate
    "Depth: sets percent of full range delay time sweeps within", // Phaser depth
    "Frequency: sets the centre frequency (in Hz). If you can't hear any phaser effect try higher or lower frequency", // Phaser
    "Feedback: negative and positive values can be used to make sound more resonant, changing sonic character", // Phaser      ########Begin here again to edit##############################
    "Phaser: sets the amount of phaser added", // Phaser
    //"Phaser: Sets the amount of wet signal in the output of the phaser", // DelayLine add phaser
    "Time Mode", // Delay2 MAIN 2' RIGHT
    "Ping pong: add classic ping pong effect to delay line 1. From mix menu, use stereo width to control it", // Delay2 MAIN to
    "Noise: can boost bit resolution effect. Try turn up noise(e.g. 70 percent), then decrease bit resolution until noise is not audible, then play a note", // Bit Crusher
    "Downsample: levels above 1 simulate reduced sample rate", // Bit Crusher
    "Bit resolution: simulate changing bit depth", // Bit Crusher
    "Bit Crusher: sets the amount of bit crusher added", // add Bit crusher
    //"R+L", // Main DelayLine   no use !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //"L+R", // Main DelayLine
    "All -> FX: will apply FX, multimode filter to both wet(echo) and dry audio", // process dry+delay ->FX
    "Parallel: this will proces delay line 1 and 2 in parallel. Delay line 2 must be activated in FX-level menu", // Parallel option button
    "Threshold: control audio level ducking react to. Only levels above threshold is affected. For basic ducking there is no need to set the other parameters", // Ducking Threshold 1
    "Amount: control how much the audio above threshold is affected", // Ducking Amount 1
    "Speed: time before ducking hit audio. Normaly you want small time values", // Ducking speed 1
    "Release: time it takes to release ducking applied to audio. Values below 200 ms might cause distortion. This is normal, as it quickly return to full level", // Ducking Release 1
    "Offset: set a small gap between left and right time, adding stereo width if initial time levels are identical. If modulated, repitch/fade affect sound",  // mFXParameter_Main_Delay_Wide,
    "Echo: scale output level of delayed audio (delay 1 + delay 2)", //  Global Wet
    "Dry: scale output level of dry audio.", //  Global Dry
    "Offset: set a small gap between left and right time, adding stereo width if initial time levels are identical. If modulated, repitch/fade affect sound", //  Dual delay wide
    "Ducking: apply ducking to delayed audio. In Parallel each delay line has unique control. Fade up one delay line after the other (set release time)", //  Dual Ducking Mode Toggle button
    "Control FX levels: audio is routed in this order, and multimode filter comes before reverb. A FX is activated by setting FX level > 0", // FX Level Toggle button,
    "Mix: balance output of dry and wet(echo) audio. Control stereo width of wet(echo) audio. Control mid/side of wet(echo) audio", // Mode Toggle button, wet/dry
    "lowcut", //delay1"
    "lowcut 2", //delay2"
    "highcut", //delay1"
    "highcut 2", //delay2"
    "Shaper: control lowpass and a highpass filter applied to delayed audio. If Parallel is selected each delay line has unique control",
    "Stereo Width: control the stereo width of the delayed audio. 0 % is mono, 100 % is normal stereo and 100+ enhance stereo further.",
    "Mid/Side: only works when audio input source is stereo. You can use stereo width to shape stereo image further",
    "Threshold: control audio level ducking react to. Only level above threshold is affected. For basic ducking there is no need to set the other parameters", // Ducking Threshold parallel
    "Amount: control how much the audio above threshold is affected", // Ducking Amount parallel
    "Speed: time before ducking hit audio. Normaly you want small time values", // Ducking Attack parallel
    "Release: time it takes to release ducking applied to audio. Values below 200 ms might cause distortion. This is normal, as it quickly return to full level", // Ducking Release parallel
    
    /* Advanced EnvF */
    "Sensitivity: sets sensitivity for reacting to incoming audio. Think of it as threshold where 5.0 correspond to -60 dB",
    "Attack: control how fast target param will react to incoming modulation",
    "Release: control when to release/fade out modulation on target param.",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    /* Advanced LFO1 */
    "Sync",
    "Rate: Control the frequency of the LFO",
    "Note: sync to DAW tempo",
    "Type",
    "Target 1",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    "Target 2",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    /* Advanced LFO2 */
    "Sync",
    "Rate: control the frequency of the LFO",
    "Note: sync to DAW tempo",
    "Type",
    "Target 1",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    "Target 2",
    "Amount: values higher than 0 scale how much target param is modulated. If target is a FX you must activate it (FX-level menu), except multimode filter",
    /*-------------Delay 1 Fade Slider and related------*/
    "Fade mode: control fade mode behavior when changing delay time (crossfade between old and new delay times). It can sound soft, harder or destructive", // fade freq
    "nothing",
   
    /*-------------Delay 2 Fade Slider and related------*/
    "Fade mode: control fade mode behavior when changing delay time (crossfade between old and new delay times). It can sound soft, harder or destructive", // fade freq
    "Nothing",
    
    /* Advanced MIDI ADSR */
    "Attack: control how fast it will reach max level (1.0)",
    "Decay: control how fast it will fall from max level to sustain level",
    "Sustain: control the sustain level to hold",
    "Release: control how fast level will fall to 0.0",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",

    /* Advanced MIDI ADSR 2 */
    "Attack: control how fast it will reach max level (1.0)",
    "Decay: control how fast it will fall from max level to sustain level",
    "Sustain: control the sustain level to hold",
    "Release: control how fast level will fall to 0.0",
    "Target 1",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    "Target 2",
    "Amount: less or higher than 0 control target param up/down direction. If target is a FX you must activate it (use FX-level menu), except multimode filter",
    
    /*-------------midi input LFO retrigger------*/
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



