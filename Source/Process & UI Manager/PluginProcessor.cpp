/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"






//==============================================================================
PluginProcessor::PluginProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("MFX"), createParameterLayout())

#endif

{
    
    initializeDSP();

    mPresetManager = std::make_unique<MFXPresetManager>(this, parameters);

    //addListener(this);

    // StereoWidth parameter
    mStereoWidth = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Stereo_Width]);
    // Bit Crusher parameter
    mResolution = parameters.getRawParameterValue(MFXParameterID[mFXParameter_BitRedux]);
    mDownsample = parameters.getRawParameterValue(MFXParameterID[mFXParameter_RateRedux]);
    mNoise = parameters.getRawParameterValue(MFXParameterID[mFXParameter_NoiseAmount]);
    mBitCrusherDryWetMix = parameters.getRawParameterValue(MFXParameterID[mFXParameter_BitDryWet]);

    // Delay 1 parameters
    mDelay1LeftTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeLeft]);
    mDelay1RightTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeRight]);
    mDelay1LeftFeedback = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1LeftFeedback]);;
    mDelay1RightFeedback = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1RightFeedback]);
    mDelay1Width = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1_Offset]);
    mSmoothingDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1_Repitch]);
    mFadeFrequencyDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_FadeFrequency_Delay1]);
    mIsFadeModeDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_IsFadeMode_Delay1]);


    mTimeModeLeftDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeModeLeft]);
    mTimeModeRightDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeModeRight]);
    mPingPongDelay1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1Pingpong]);
    mDelay1LinkTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1LinkTime]);


    // Delay 2 parameters
    mDelay2LeftTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeLeft]);
    mDelay2RightTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeRight]);
    mDelay2LeftFeedback = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2LeftFeedback]);
    mDelay2RightFeedback = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2RightFeedback]);
    mDelay2Width = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2_Offset]);
    
    /* in serial mode @param wetLevel (DUAL delay) sets balance of Dual delay (full at 1)
     and main delay (main delay full at 0, gone at 1). 0.5 is equal balanced.
     in parallel mode @param wetLevel (DUAL delay) only sets dry/wet of Dual delay, and is not connected with main delay */
    mMixDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2WetDry]);
    
    mSmoothingDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2Repitch]);
    mFadeFrequencyDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_FadeFrequency_Delay2]);
    mIsFadeModeDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_IsFadeMode_Delay2]);

    mTimeModeLeftDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeModeLeft]);
    mTimeModeRightDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeModeRight]);
    mPingPongDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2_Pingpong]);
    mChainModeDelay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Parallel_Mode]);
    mDelay2LinkTime = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2LinkTime]);

    // Global Dry/wet level
    mGlobalDry = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Global_Dry]);
    mGlobalWet = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Global_Wet]);



    // MIDI ADSR

    mAttack = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Attack]);
    mDecay = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Decay]);
    mSustain = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Sustain]);
    mRelease = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Release]);

    mADSRTarget1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target1]);
    mADSRTarget1_Amount = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target1_Amount]);
    mADSRTarget2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target2]);
    mADSRTarget2_Amount = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target2_Amount]);


    // MIDI ADSR 2

    mAttack2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Attack]);
    mDecay2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Decay]);
    mSustain2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Sustain]);
    mRelease2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Release]);

    mADSR2Target1 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target1]);
    mADSR2Target1_Amount = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target1_Amount]);
    mADSR2Target2 = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target2]);
    mADSR2Target2_Amount = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target2_Amount]);

    // Midi LFO

    mIsLFO1Retrigger = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_LFO1_Retrigger]);
    mIsLFO2Retrigger = parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_LFO2_Retrigger]);



    //  Removed mAppUnlockDataDirectory init ____________________________________________________________________________________________
    
    


}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginProcessor::getProgramName (int index)
{
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = uint32 (samplesPerBlock);
    const int numInputChannel = getTotalNumInputChannels();
    spec.numChannels = uint32 (numInputChannel);
    
    for (int i = 0; i < 2; i++)
    {
        mLowCutFilters[i]->prepare(spec);
        mHighCutFilters[i]->prepare(spec);
    }
    
    mData.prepare(spec);
    
    
    mChorusSmoothDelayTime.reset(sampleRate, 0.04); 
    

    mChorus2->prepare(spec);
    
    mJPhasor.prepare(spec);
    
    mBitCrusher->prepareToPlay(spec);

    
  
    
    mWetBuffer.setSize(4, samplesPerBlock); 
    mWetBuffer.clear();
    
    // use this one for stereo/mono check
    mCopyBuffer.setSize(2, samplesPerBlock);
    mCopyBuffer.clear();
    
   

    
    mReverb->prepareToProces(sampleRate, samplesPerBlock, getTotalNumInputChannels());
    
    
    mMoogFilter->prepareToProces(spec);
    
    mDelayLine1->prepareToProcess(spec);
    
    
    mDelayLine2->prepareToProcess(spec);

  
    
    mDucking.prepare(spec);
    mDuckingParallel.prepare(spec);
    
    
    mSmoothingGlobalDry.reset(sampleRate, 0.04); // play with settings
    mSmoothingGlobalWet.reset(sampleRate, 0.04);
    mSmoothingDelay2Mix.reset(sampleRate, 0.04);
    
    
    
    
    //----------New Meter stuff----------------------------------------------
    rmsLevelsIn.clear();
    for (auto i = 0; i < numInputChannel; i++)
    {
        LinearSmoothedValue<float> rms{ -100.f };
        rms.reset(sampleRate, 0.5);
        rmsLevelsIn.emplace_back(std::move(rms));
    }
    
    rmsFifoIn.reset(numInputChannel, static_cast<int>(sampleRate) + 1);
    rmsCalculationBufferIn.clear();
    rmsCalculationBufferIn.setSize(numInputChannel, static_cast<int>(sampleRate) + 1);
    
    
    rmsLevelsOut.clear();
    for (auto i = 0; i < numInputChannel; i++)
    {
        LinearSmoothedValue<float> rms{ -100.f };
        rms.reset(sampleRate, 0.5);
        rmsLevelsOut.emplace_back(std::move(rms));
    }
    
    rmsFifoOut.reset(numInputChannel, static_cast<int>(sampleRate) + 1);
    rmsCalculationBufferOut.clear();
    rmsCalculationBufferOut.setSize(numInputChannel, static_cast<int>(sampleRate) + 1);
    
    
    
    // time it takes meter to fall 
    rmsWindowSize = static_cast<int>(sampleRate * 60) / 1000; 
    isSmoothed = true;
    
    
    
    
    // update mIsUnlocked var
    readUnlockedStatus();    
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    for (int i = 0; i < 2; i++)
    {
        mLowCutFilters[i]->reset();
        mHighCutFilters[i]->reset();
    }            
    
    mCopyBuffer.clear();
    mReverb->reset();
    mMoogFilter->reset();
    mDelayLine1->reset();
    mDelayLine2->reset(); 
    mDucking.reset();
    mDuckingParallel.reset();
    mData.mEnvelopeFollower->reset();
    mData.mLfoModulator1->reset();
    mData.mLfoModulator2->reset();
    mWetBuffer.clear();
    mChorus2->reset(); 
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
//  #if JucePlugin_IsMidiEffect
//    juce::ignoreUnused (layouts);
//    return true;
//  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
//    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
//     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//        return false;


    // Stereo only fix plugin validation issues in Logic
    
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
        return true;
     else
        return false;
    
    
    // This checks if the input layout matches the output layout
//   #if ! JucePlugin_IsSynth
//    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//        return false;
//   #endif

    //return true;
  //#endif
}
#endif



void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    /*
     The maximumExpectedSamplesPerBlock value is a strong hint about the maximum
     number of samples that will be provided in each block. You may want to use
     this value to resize internal buffers. You should program defensively in
     case a buggy host exceeds this value. The actual block sizes that the host
     uses may be different each time the callback happens: completely variable
     block sizes can be expected from some hosts.*/
    
    /*Because the buffer size from the host could come in small chunks.
      For example if your maximum buffer is 256 samples, you might get two calls with 128, or one call with 255 and one call with 1.*/
    
    // We do this to avoid potential error in eg. Ducking if in/out block numSamples don't match due to DAW that send a buffer that is smaller than maximum size
    if (buffer.getNumSamples() != mWetBuffer.getNumSamples())
    {
        mWetBuffer.setSize(4, buffer.getNumSamples());
        mWetBuffer.clear();
        
        jassert(buffer.getNumSamples() == mWetBuffer.getNumSamples()); // this is to alert if buffers still don't match     
    }
     
    juce::ScopedNoDenormals noDenormals;
    
    juce::ignoreUnused(midiMessages);
    
    
    // to access read/write main Buffer
    float* bufferWriterPointer[2] = { buffer.getWritePointer (0), buffer.getWritePointer (1) } ;
    
    // to access read/write wetBuffer
    float* wetBufferWriterPointer[4] = { mWetBuffer.getWritePointer (0), mWetBuffer.getWritePointer (1),
                                         mWetBuffer.getWritePointer (2), mWetBuffer.getWritePointer (3) };
    
      
    auto numSamplesToRender = buffer.getNumSamples();
    
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //    // In case we have more outputs than inputs, this code clears any output
    //    // channels that didn't contain input data, (because these aren't
    //    // guaranteed to be empty - they may contain garbage).
    //    // This is here to avoid people getting screaming feedback
    //    // when they first compile a plugin, but obviously you don't need to keep
    //    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
         buffer.clear (i, 0, buffer.getNumSamples());    
    }

    
    
   
    
    // InGain --------------------------------------------------------------------------------------------------
    for (int channel = 0; channel < totalNumInputChannels; channel++)
    { 
        auto& inputGain = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_InputGain]);
        mInputGain[channel]->process(bufferWriterPointer[channel],
                                     inputGain, numSamplesToRender);
    }
    
    
    
    
    
    
    
    // LevelMeter -----------------------------------------------------------------------------------------------
    
    
    
    for (auto& rmsLevel : rmsLevelsIn)
         rmsLevel.skip(numSamplesToRender);
    
    rmsFifoIn.push(buffer);
    
    
    
    // Get playhead info ----------------------------------------------------------------------------------------
    setDawPlayHeadInfo();
    
    

    
    
    // Modulation----------------------------------------------------------------------------------------------------
    
    updateModulationTargetData();
    setEnvelopeFollowerParameters();
    setLFO1Parameters(); // set param from UI and set phase before process LFO
    setLFO2Parameters();
    mData.mADSR->setParams(mAttack->load(), mDecay->load(), mSustain->load(), mRelease->load());
    mData.mADSR2->setParams(mAttack2->load(), mDecay2->load(), mSustain2->load(), mRelease2->load());

    mData.processModulationOutput(getPlayHead() ,bufferWriterPointer[0], buffer.getNumSamples(), getSampleRate(), midiMessages);
    
    //------------------------------------------------------------------------------------------------------------------
    
    
    
    
    
    // Main Delay 1 ------------------------------------------------------------------------------------------------
  
    for(int i = 0; i < buffer.getNumSamples(); i++)
    {
        setDelay1Parameters();
        modulateDelay1Parameters(i);
        mDelayLine1->process(bufferWriterPointer[0][i], bufferWriterPointer[1][i],
                             wetBufferWriterPointer[0][i], wetBufferWriterPointer[1][i]);
        
    }

   
    
    
    // Delay 2 DUAL --------------------------------------------------------------------------------------------------
    
    auto& addDelay = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2WetDry]);
    auto& parallelMode = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Parallel_Mode]);
    
    
    if (addDelay > 0.0f)
    {      
        if (!parallelMode)
        {       
            for(int i = 0; i < buffer.getNumSamples(); i++)
            {
                setDelay2Parameters();
                modulateDelay2Parameters(i);
                mDelayLine2->process(wetBufferWriterPointer[0][i], wetBufferWriterPointer[1][i],
                                     wetBufferWriterPointer[0][i], wetBufferWriterPointer[1][i]);       
            }
        }
        else
        {
            for(int i = 0; i < buffer.getNumSamples(); i++)
            {
                setDelay2Parameters();
                modulateDelay2Parameters(i);
                mDelayLine2->process(bufferWriterPointer[0][i], bufferWriterPointer[1][i],
                                     wetBufferWriterPointer[2][i], wetBufferWriterPointer[3][i]);       
            }
        }
    }
    
    
   
    
    
  
    
    
    

// Parallel Filter Mode, And sum mWetBuffer channels--------------------------------------------------------------------------------------
    
    if (parallelMode)  
    {
        procesDualFilterParallel();
        
        auto& wet = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2WetDry]);
        
        // Parallel ducking on each delayline
        setDuckingParameters(); // Delay 1
        
        constexpr int numChannels = 2;
        auto tempBufferDelay1 = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
        
        juce::dsp::AudioBlock<float> blockWet { tempBufferDelay1 };
        juce::dsp::AudioBlock<float> blockDry { buffer };
        mDucking.process (juce::dsp::ProcessContextReplacing<float> (blockDry), juce::dsp::ProcessContextReplacing<float> (blockWet));
        
        
        setDuckingParallelParameters(); // Delay 2
        constexpr int startChannel = 2;
        constexpr int numChannelMid = 2;
        auto tempBufferDelay2 = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers()
                                                         + startChannel, numChannelMid, mWetBuffer.getNumSamples());
        juce::dsp::AudioBlock<float> blockWet2 { tempBufferDelay2 };
        mDuckingParallel.process (juce::dsp::ProcessContextReplacing<float> (blockDry), juce::dsp::ProcessContextReplacing<float> (blockWet2));

        
        
        
        
        mWetBuffer.addFrom(0, 0, mWetBuffer, 2, 0, mWetBuffer.getNumSamples());
        mWetBuffer.addFrom(1, 0, mWetBuffer, 3, 0, mWetBuffer.getNumSamples());
        
        
        mWetBuffer.clear(2, 0, mWetBuffer.getNumSamples());
        mWetBuffer.clear(3, 0, mWetBuffer.getNumSamples());
        
        
    }
    
    
    
    
    
    
    

    
    // Serial Filter Mode--------------------------------------------------------------------------------------------------------------
    
    if (!parallelMode)
    {
        procesDualFilterSerial();
    }
    
    
    
    
    
    
    // M/S --------Stereo input to Mid Side decoding-----------------------------------------------------------------------------------
    
    auto& midside = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Mid_Side_Mode]);
    
    
    // Check for incoming stereo or mono signal ---------------------------------------
    // mDifference > 0 means that L/R channel are different and therefor is stereo.
    
    // Buffer signal must be > 0 Otherwise 0 value buffer gets copied and M/S is not processed because it think its mono(mDifference = 0).
    if (buffer.getSample(0, 0) > 0 && buffer.getSample(1, 0) > 0) // hmmm valid signal can also be negative towards -1??
    {
        // Make copy because buffer signal gets destoyed(channel level subtraction), makes clicks/crunch when summmed later with wet signal.
        mCopyBuffer.makeCopyOf(buffer);
        // -1.0f is the same as subtracting. If mDifference = 0 that means its mono and != 0 is stereo signal
        mCopyBuffer.addFrom (0, 0, mCopyBuffer.getReadPointer (1), mCopyBuffer.getNumSamples(), -1.0f);
        mDifference = mCopyBuffer.getMagnitude (0, 0, mCopyBuffer.getNumSamples());
    }
    
    // Do MS processing if stereo----------------------------------------------------------------
    // Only process if stereo. 
    if (midside && mDifference > 0)
    {
        for(int i = 0; i < mWetBuffer.getNumSamples(); i++)
        {
            mStereo->processMS(wetBufferWriterPointer[0][i], wetBufferWriterPointer[1][i]);
        }
    }
    
    
    

    

    
    // StereoWidth --------If M/S then Mid Side to stereo decoding--------------------------------------------------------------------
    
    setStereoWidth();
    
    for(int i = 0; i < mWetBuffer.getNumSamples(); i++)
    {
        modulateStereoWidth(i);
        mStereo->processWidth(wetBufferWriterPointer[0][i], wetBufferWriterPointer[1][i]);
    }
    
    
    
    
    
    
    
    
// Ducking effect Serial Mode---------------------------------------------------------------------------------------------------------
    
    
    if (!parallelMode)
    {
        setDuckingParameters();
        
        constexpr int numChannel = 2;
        auto tempBuffer2 = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannel, mWetBuffer.getNumSamples());
        juce::dsp::AudioBlock<float> blockWet { tempBuffer2 };
        juce::dsp::AudioBlock<float> blockDry { buffer };
        mDucking.process (juce::dsp::ProcessContextReplacing<float> (blockDry), juce::dsp::ProcessContextReplacing<float> (blockWet));
    }
    

    
    
    
    

    
// Clear audio. Handy if user want to kill feedback delay tail --------------------------------------------------------------------

    if(mClearBuffer)
    {
        releaseResources();
        mClearBuffer = false;
    }
    
//----------------------------------------------------------------------------------------------------------------------------------------

    
    
    
   
    // Sum buffers if both dry and wet signal should have FX -------------------------------------------------------------------------------

    auto& allFX = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_AllFxProcessing]);
    
    if(allFX)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            setGlobalDryWetParameters(i);
            const auto& dry = mSmoothingGlobalDry.getNextValue();
            const auto& wet = mSmoothingGlobalWet.getNextValue();
            buffer.setSample(0, i, buffer.getSample(0, i) * dry + mWetBuffer.getSample(0, i) * wet );
            buffer.setSample(1, i, buffer.getSample(1, i) * dry + mWetBuffer.getSample(1, i) * wet );  
        }
    }
    
    
    
    // BitCrusher ----------------------------------------------------------------------------------------------
    
    
    
    auto& addBitCrusher = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_BitDryWet]);  // why not atomic load() in if()

    if (addBitCrusher > 0.0f)
    {
        setBitcrusherParameters();
        
        if(allFX)
        {
            mBitCrusher->process(buffer); 
        }
        else
        {
            constexpr int numChannels = 2;
            auto tempBuffer = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
            mBitCrusher->process(tempBuffer);
        }   
    }
    
    if (allFX)
    {
        setMoogFilterParameters();

        for (int i = 0; i < buffer.getNumSamples(); i++)
        {

            mMoogFilter->modulateParameters(i);
            mMoogFilter->process(bufferWriterPointer[0][i], 0);
            mMoogFilter->process(bufferWriterPointer[1][i], 1);
        }
    }
    
    
    if(!allFX)
    {
        // do filter here on mWetbuffer
        
        setMoogFilterParameters();
        
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            
            mMoogFilter->modulateParameters(i);
            mMoogFilter->process(wetBufferWriterPointer[0][i], 0);
            mMoogFilter->process(wetBufferWriterPointer[1][i], 1);    
        }
    }
    
    
    // JChorus ------------------------------------------------------------------------------------------
    const auto& addChorus = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusWetDry]);
    
    if (addChorus > 0.0f)
    {
        setChorusParameters();

        if(allFX)
        {
            mChorus2->process(buffer);
        }
        else
        {  
            constexpr int numChannels = 2;   
            auto tempBuffer = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
            mChorus2->process(tempBuffer);
        }
    }
    
    
    
    
    
    
    
    // Phaser -----------------------------------------------------------------------------------------------
    auto& addPhaser = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserDryWet]);
   
    if (addPhaser > 0.0f)
    {
        setPhaserParameters();
        
        if(allFX)
        {
            juce::dsp::AudioBlock<float> block { buffer };
            mJPhasor.process (juce::dsp::ProcessContextReplacing<float> (block));
        }
        else
        {
            constexpr int numChannels = 2;
            auto tempBuffer = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
            juce::dsp::AudioBlock<float> block { tempBuffer };
            mJPhasor.process (juce::dsp::ProcessContextReplacing<float> (block));
        }
    }
    
    
    
    
    
    
    
    

    
    
    
    // Reverb -------------------------------------------------------------------------------------------------

    auto& addReverb = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ReverbWet]);
    
  
    if(addReverb > 0)
    {
        setReverbParameters();
        
        if(allFX)
        {
            mReverb->process(buffer);
        }
        else
        {
            constexpr int numChannels = 2;
            auto tempBuffer = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
            mReverb->process(tempBuffer);
        }
    }
    
  
    
   
     
    
    
    
    // Sum buffers if wet signal only is added FX -----------------------------------------------------------------
    
    if(!allFX)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            setGlobalDryWetParameters(i);
            const auto& dry = mSmoothingGlobalDry.getNextValue();
            const auto& wet = mSmoothingGlobalWet.getNextValue();   
            buffer.setSample(0, i, buffer.getSample(0, i) * dry + mWetBuffer.getSample(0, i) * wet );
            buffer.setSample(1, i, buffer.getSample(1, i) * dry + mWetBuffer.getSample(1, i) * wet );
        }
    }
    
    
    

    
  
    
    // OutputGain -------------------------------------------------------------------------------------------------
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
    
        auto& outputGain = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_OutputGain]);
        mOutputGain[channel]->process(bufferWriterPointer[channel],      
                              outputGain,
                              numSamplesToRender);
    }
    
    
   
    
    // LevelMeter ------------------------------------------------------------------------------------------------
    
    for (auto& rmsLevel : rmsLevelsOut)
        rmsLevel.skip(numSamplesToRender);
    
    rmsFifoOut.push(buffer);

    
    
    // Use when LFO's is modulated and DAW is not playing. Keep track of free running sample time. Used in setLFO1/2Parameters() to set lfo phase. 
    // What happens if mSumBlokData gets really big - risk of overflow. TODO: wrap around 0/reset mSumBlokData. Can I refactor this out completely, finding aother solution.
    // work around when not having transport timeInSamples
    mSumBlokData += buffer.getNumSamples();

    
    // This protect plugin from audio boost in DAW if something goes wrong TODO: not sure about boost level 20.0f 
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        if (buffer.getMagnitude(channel, 0, buffer.getNumSamples()) > 20.0f) 
        {
            buffer.clear(channel, 0, buffer.getNumSamples());    
        }
    }


    // Product/License protection -------------------------------------------------------------------------------------

    if (!mIsUnlocked) 
    {
        // Count samples contained in block
        for (size_t i = 0; i < buffer.getNumSamples(); i++)
        {
            if (mSampleTimeCounter < getSampleRate() * 50)
                mSampleTimeCounter++;
            else
                mSampleTimeCounter = 0;
        }

        
        
        
        
        // exceeds 45s, then audio silence 5s 
        if (mSampleTimeCounter > getSampleRate() * 45 && mSampleTimeCounter < getSampleRate() * 50)
        {
            //DBG("Should'nt be here if unlocked");
            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                 buffer.clear(channel, 0, buffer.getNumSamples());
            }
        }
    }
    
    

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}



//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    
    // return new PluginEditor (*this);   
    return new WrappedRasterAudioProcessorEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{

    //Serializes parameters, and any other potential data into an XML:

    
    if (readUnlockedStatus()) 
    {
        juce::ValueTree params("Params");

        for (auto& param : getParameters())
        {
            juce::ValueTree paramTree(getParamID(param));
            paramTree.setProperty("Value", param->getValue(), nullptr);
            params.appendChild(paramTree, nullptr);
        }

        juce::ValueTree pluginPreset("TimeRazor");
        pluginPreset.appendChild(params, nullptr);

        //add non-parameters to preset
        juce::ValueTree text("LastPresetName");
        text.setProperty("presetname", mPresetManager->getCurrentPresetName().contains("*") ? mPresetManager->getCurrentPresetName() :
            mPresetManager->getCurrentPresetName() + "*", nullptr);

        pluginPreset.appendChild(text, nullptr);

        copyXmlToBinary(*pluginPreset.createXml(), destData);

    }
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    //Loads parameters, and any other potential data from an XML:
    
    if (readUnlockedStatus())
    {

        auto xml = getXmlFromBinary(data, sizeInBytes);

        if (xml != nullptr)
        {
            auto preset = juce::ValueTree::fromXml(*xml);
            auto params = preset.getChildWithName("Params");

            for (auto& param : getParameters())
            {
                auto paramTree = params.getChildWithName(getParamID(param));

                if (paramTree.isValid())
                    param->setValueNotifyingHost(paramTree["Value"]);
            }

            //Load non-parameter data now
            params = preset.getChildWithName("LastPresetName");

            mPresetManager->setCurrentPresetName(params.getPropertyAsValue("presetname", nullptr).toString());
        }

        mPresetNameChangeEvent.sendActionMessage("Update");
    }
}


    
// I really wish that I had orgainized order better from the start. A hard reminder - PLAN your work. But not always easy as eksperimenting and changes/new functions are added to project during development.
// Refactor order will not affect APVTS, it will work just fine. It is time consuming, but if time I will refactor this.

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    
    
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    
    /*The version hint supplied to this constructor is used in Audio Unit plugins to aid ordering parameter identifiers when JUCE_FORCE_USE_LEGACY_PARAM_IDS is not enabled.

    When adding a parameter that is not present in a previous version of the Audio Unit, you must ensure that the version hint supplied is a number higher than that of any parameter in any previous plugin version.

    For example, in the first release of a plugin, every parameter was created with "1" as a version hint. If you add some parameters in the second release of the plugin, all of the new parameters should have "2" as a version hint. Additional parameters added in subsequent plugin versions should have "3", "4", and so forth, increasing monotonically. */
    
    int version1 = 1;
    // if more params is added after release build, then version2 = 2 is added to new params
    

    /*--------------------------------Gain In/Out-----------------------------------------------------------*/

    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_InputGain], version1},
                                                           MFXParameterLabel[mFXParameter_InputGain],
                                                           NormalisableRange<float>(-70.0f,12.0f, 0.001f, 1.7f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = (value > -70.0f) ?
                                                               String (value, 1 ) + " dB" : "-inf dB";
                                                               
                                                               
                                                               return valueToText;
                                                           }
                                                           ));

    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_OutputGain],version1 },
                                                           MFXParameterLabel[mFXParameter_OutputGain],
                                                           NormalisableRange<float>(-70.0f, 12.0f, 0.001f, 1.7f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = (value > -70.0f) ?
                                                               String(value, 1) + " dB" : "-inf dB";

                                                               return valueToText;
                                                           }
                                                           ));
    
    /*--------------------------------Chorus-----------------------------------------------------------*/

    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ChorusRate], version1},
                                                           MFXParameterLabel[mFXParameter_ChorusRate],
                                                           NormalisableRange<float>(0.01f,5.0f, 0.001f, 0.45f),0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value, 1) + " Hz";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ChorusDepth], version1},
                                                           MFXParameterLabel[mFXParameter_ChorusDepth],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (static_cast<int>(value * 10)) + " ms";
                                                               
                                                               
                                                               
                                                               return valueToText;
                                                           }
                                                           ));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ChorusWetDry], version1},
                                                           MFXParameterLabel[mFXParameter_ChorusWetDry],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value * 100,1) + " %";
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_ChorusWidth], version1},
                                                           MFXParameterLabel[mFXParameter_ChorusWidth],
                                                           NormalisableRange<float>(0.0f, 0.5f, 0.001f, 0.45f), 0.25f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value * 2,1);                                         
                                                               
                                                               return valueToText;
                                                           }
                                                           ));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ChorusFeedback], version1},
                                                           MFXParameterLabel[mFXParameter_ChorusFeedback],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value * 100,1) + " %";                                                                                                                        
                                                               
                                                               return valueToText;
                                                           }
                                                           ));
   
    /* -------------------------------------------------Delay 2-----------------------------------------------------------------------------------------*/
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Delay2TimeModeLeft], version1},
                                                                   MFXParameterLabel[mFXParameter_Delay2TimeModeLeft],
                                                                   juce::StringArray { "Time: ms", "Straight", "Dotted", "Triplet"},0.0f));
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Delay2TimeModeRight], version1},
                                                                   MFXParameterLabel[mFXParameter_Delay2TimeModeRight],
                                                                   juce::StringArray { "Time: ms", "Straight", "Dotted", "Triplet"},0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2TimeLeft], version1},
                                                           MFXParameterLabel[mFXParameter_Delay2TimeLeft],
                                                           NormalisableRange<float>(0.00025f, 1.0f, 0.000001f, 0.8f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [&] (float value, int) -> String
                                                           {
//                                                               
                                                               
                                                               return mUIData.mParameterValueText[mFXParameter_Delay2TimeLeft]; //valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2TimeRight],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2TimeRight],
                                                           NormalisableRange<float>(0.00025f,1.0f, 0.000001f, 0.8f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [&] (float value, int) -> String
                                                           {
//                                                              
                                                               
                                                               return mUIData.mParameterValueText[mFXParameter_Delay2TimeRight]; //valueToText;
                                                           }));
    
   
    

    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2LeftFeedback],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2LeftFeedback],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int)
                                                           {
                                                               String valueToText = String (static_cast<int>(jmap(value, 0.0f, 1.0f, 0.0f, 1.2f)
                                                                                                             * 100)) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
 
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2RightFeedback],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2RightFeedback],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int)
                                                           {
                                                               String valueToText = String (static_cast<int>(jmap(value, 0.0f, 1.0f, 0.0f, 1.2f)
                                                                                                             * 100)) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Delay2LinkTime],version1},
                                                          MFXParameterLabel[mFXParameter_Delay2LinkTime], static_cast<bool>(0.0f)));
    
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID { MFXParameterID[mFXParameter_Delay2LinkFeedback],version1},
                                                          MFXParameterLabel[mFXParameter_Delay2LinkFeedback], static_cast<bool>(0.0f)));
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Delay2_Pingpong],version1},
                                                          MFXParameterLabel[mFXParameter_Delay2_Pingpong], static_cast<bool>(0.0f)));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2Repitch],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2Repitch],
                                                           NormalisableRange<float>(0.1f,20000.0f,0.001f), 
                                                           200.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String ( roundToInt(value/200) ) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_FadeFrequency_Delay2],version1 },
                                                           MFXParameterLabel[mFXParameter_FadeFrequency_Delay2],
                                                           NormalisableRange<float>(0.3f, 25.0f, 0.001f, 0.3f),2.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value, 2) + " ";

                                                               return valueToText;
                                                            }));

   

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_IsFadeMode_Delay2],version1 },
                                                          MFXParameterLabel[mFXParameter_IsFadeMode_Delay2], static_cast<bool>(0.0f)));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2_Offset],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2_Offset],
                                                           NormalisableRange<float>(-20.0f, 20.0f, 0.0001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value/20,1);
                                                               
                                                               
                                                           
                                                               
                                                               return valueToText;
                                                           }));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay2WetDry],version1},
                                                           MFXParameterLabel[mFXParameter_Delay2WetDry],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    

    /*-----------------------------------------------Select FX combobox------------------------------------------------------------------------*/
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_FXType],version1},
                                                           MFXParameterLabel[mFXParameter_FXType],
                                                           NormalisableRange<float>(0.0f,6.0f), 0.0f));
    
    
    
    /*-----------------------------------------------Reverb------------------------------------------------------------------------*/
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_ReverbSize],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbSize],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f),0.9f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " ";;                                     
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ReverbWidth],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbWidth],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.6f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ReverbDamping],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbDamping],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID { MFXParameterID[mFXParameter_ReverbDry],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbDry],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.6f,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_ReverbWet],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbWet],
                                                           NormalisableRange<float>(0.0f,0.35f, 0.001f), 0.0f,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 285.714286,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_ReverbFreeze],version1},
                                                           MFXParameterLabel[mFXParameter_ReverbFreeze], static_cast<bool>(0.0f)));

    /*-----------------------------------------------Ladder Filter------------------------------------------------------------------------*/


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_FilterCutoff],version1},
                                                           MFXParameterLabel[mFXParameter_FilterCutoff],
                                                           NormalisableRange<float>(20.0f,15000.0f, 0.01, 0.3),15000.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               (value < 1000.0f) ? String (value,1) + " Hz" : String (value/1000,1) + " KHz";
                                                               
                                                               
                                                               return valueToText;
                                                           }));



    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_FilterResonans],version1},
                                                           MFXParameterLabel[mFXParameter_FilterResonans],
                                                           NormalisableRange<float>(0.1f,1.0f,0.001f), 0.1f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1);
                                                               
                                                               return valueToText;
                                                           }));
       
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_FilterDrive],version1},
                                                           MFXParameterLabel[mFXParameter_FilterDrive],
                                                           NormalisableRange<float>(1.0f,20.0f,0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1);
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID { MFXParameterID[mFXParameter_FilterType],version1},
                                                                   MFXParameterLabel[mFXParameter_FilterType],
                                                                   juce::StringArray { "LPF12", "LPF24", "BPF12", "BPF24", "HPF12", "HPF24" },0.0f));
    


    /*--------------------------------------------------------Delay 1----------------------------------------------------------------------------------------------*/


    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Delay1TimeModeLeft],version1},
                                                                   MFXParameterLabel[mFXParameter_Delay1TimeModeLeft],
                                                                   juce::StringArray { "Time: ms", "Straight", "Dotted", "Triplet"},0.0f));
    
    
    
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Delay1TimeModeRight],version1},
                                                                   MFXParameterLabel[mFXParameter_Delay1TimeModeRight],
                                                                   juce::StringArray { "Time: ms", "Straight", "Dotted", "Triplet"},0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay1TimeLeft],version1},
                                                           MFXParameterLabel[mFXParameter_Delay1TimeLeft],
                                                           NormalisableRange<float>(0.00025f, 1.0f, 0.000001f,0.8f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [&] (float value, int) -> String
                                                           {
                                                               
                                                               return mUIData.mParameterValueText[mFXParameter_Delay1TimeLeft];
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay1TimeRight],version1},
                                                           MFXParameterLabel[mFXParameter_Delay1TimeRight],
                                                           NormalisableRange<float>(0.00025f, 1.0f, 0.000001f, 0.8f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [&] (float value, int) -> String
                                                           {
                                                        
                                                               return mUIData.mParameterValueText[mFXParameter_Delay1TimeRight]; //valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay1LeftFeedback],version1},
                                                           MFXParameterLabel[mFXParameter_Delay1LeftFeedback],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               
                                                               String valueToText = String ( jmap(value, 0.0f, 1.0f, 0.0f, 1.2f)
                                                                                              * 100, 1 ) + " %";
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay1RightFeedback],version1},
                                                           MFXParameterLabel[mFXParameter_Delay1RightFeedback],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f),0.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               
                                                               String valueToText = String ( jmap(value, 0.0f, 1.0f, 0.0f, 1.2f)
                                                                                            * 100, 1 ) + " %";
                                                               
                                                               return valueToText;
                                                           }));
    
    
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Delay1LinkTime],version1},
                                                          MFXParameterLabel[mFXParameter_Delay1LinkTime], static_cast<bool>(0.0f)));

    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Delay1LinkFeedback],version1},
                                                          MFXParameterLabel[mFXParameter_Delay1LinkFeedback], static_cast<bool>(0.0f)));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Delay1_Repitch],version1},
                                                           MFXParameterLabel[mFXParameter_Delay1_Repitch],
                                                           NormalisableRange<float>(0.1f,20000.0f,0.001f), 200.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String ( roundToInt(value/200) ) + " %";
                                                               
                                                               return valueToText;
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_FadeFrequency_Delay1],version1 },
                                                           MFXParameterLabel[mFXParameter_FadeFrequency_Delay1],
                                                           NormalisableRange<float>(0.3f, 25.0f, 0.001f, 0.3f), 0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value, 2) + " ";

                                                               return valueToText;
                                                           }));



    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_IsFadeMode_Delay1],version1 },
                                                          MFXParameterLabel[mFXParameter_IsFadeMode_Delay1], static_cast<bool>(0.0f)));

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_Delay1Pingpong],version1 },
                                                          MFXParameterLabel[mFXParameter_Delay1Pingpong], static_cast<bool>(0.0f)));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_Delay1_Offset],version1 },
                                                           MFXParameterLabel[mFXParameter_Delay1_Offset],
                                                           NormalisableRange<float>(-20.0f, 20.0f, 0.0001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value / 20, 1);

                                                               return valueToText;
                                                           }));



    /*--------------------------------------------------------Phaser----------------------------------------------------------------------------------------------*/

    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_PhaserRate],version1},
                                                           MFXParameterLabel[mFXParameter_PhaserRate],
                                                           NormalisableRange<float>(0.0f,99.0f, 0.001f, 0.45f),0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value, 1) + " Hz";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_PhaserDepth],version1},
                                                           MFXParameterLabel[mFXParameter_PhaserDepth],
                                                           NormalisableRange<float>(0.0f,1.0f, 0.001f), 0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value * 100, 1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_PhaserCenterFrequency],version1},
                                                           MFXParameterLabel[mFXParameter_PhaserCenterFrequency],
                                                           NormalisableRange<float>(20.0f,20000.0f, 0.01f, 0.45f),1500.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value, 1) + " Hz";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_PhaserFeedback],version1},
                                                           MFXParameterLabel[mFXParameter_PhaserFeedback],
                                                           NormalisableRange<float>(-1.0f, 1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value * 100,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_PhaserDryWet],version1},
                                                           MFXParameterLabel[mFXParameter_PhaserDryWet],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    

    
    /*--------------------------------------------------------BitChrusher----------------------------------------------------------------------------------------------*/

    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_NoiseAmount],version1},
                                                           MFXParameterLabel[mFXParameter_NoiseAmount],
                                                           NormalisableRange<float>(0.0f, 100.0f, 0.001f, 0.9f),48.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_RateRedux],version1},
                                                           MFXParameterLabel[mFXParameter_RateRedux],
                                                           NormalisableRange<float>(1.0f, 50.0f, 0.001f),39.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (static_cast<int>(value)) + " ";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_BitRedux],version1},
                                                           MFXParameterLabel[mFXParameter_BitRedux],
                                                           NormalisableRange<float>(2.0f, 16.0f, 0.001f),4.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (static_cast<int>(value)) + " bit";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_BitDryWet],version1},
                                                           MFXParameterLabel[mFXParameter_BitDryWet],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value * 100,1) + " %";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
   
    /* -----------------------------------------------Center Global Menu -----------------------------------------------------------------------------------------*/

    

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_AllFxProcessing],version1},
                                                          MFXParameterLabel[mFXParameter_AllFxProcessing], static_cast<bool>(0.0f)));
    
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Parallel_Mode],version1},
                                                          MFXParameterLabel[mFXParameter_Parallel_Mode], static_cast<bool>(0.0f)));

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_Ducking_Mode],version1 },
                                                          MFXParameterLabel[mFXParameter_Ducking_Mode], static_cast<bool>(0.0f)));


    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_Mix_Mode],version1 },
                                                                       MFXParameterLabel[mFXParameter_Mix_Mode], static_cast<bool>(0.0f)));


    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_FX_Level_Mode],version1 },
                                                                       MFXParameterLabel[mFXParameter_FX_Level_Mode], static_cast<bool>(0.0f)));

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_Cut_Mode],version1 }, 
                                                                       MFXParameterLabel[mFXParameter_Cut_Mode], static_cast<bool>(0.0f)));

    

    /* -----------------------------------------------Global Mix -----------------------------------------------------------------------------------------*/



    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_Global_Wet],version1 },
                                                           MFXParameterLabel[mFXParameter_Global_Wet],
                                                           NormalisableRange<float>(0.0f, 2.0f, 0.001f),1.0f,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value * 100, 1) + " %";

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_Global_Dry],version1 },
                                                           MFXParameterLabel[mFXParameter_Global_Dry],
                                                           NormalisableRange<float>(0.0f, 2.0f, 0.001f),1.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value * 100, 1) + " %";

                                                               return valueToText;
                                                           }));



 
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID{ MFXParameterID[mFXParameter_Stereo_Width],version1 },
                                                           MFXParameterLabel[mFXParameter_Stereo_Width],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.01), 0.5f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value * 200, 1) + " %";

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterBool>(ParameterID{ MFXParameterID[mFXParameter_Mid_Side_Mode],version1 },
                                                          MFXParameterLabel[mFXParameter_Mid_Side_Mode], static_cast<bool>(0.0f)));





    /* ----------------------------------------------------------Ducking single/serial. If parallel mode then delay 1 is controlled ---------*/




  
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Threshold],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Threshold],
                                                           NormalisableRange<float>(-60.0f, 0.0f, 0.01f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " dB";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Amount],
                                                           NormalisableRange<float>(1.0f, 5.0f, 0.01f),5.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1);
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Attack],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Attack],
                                                           NormalisableRange<float>(0.01f, 3000.0f, 0.01f, 0.3f),0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Release],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Release],
                                                           NormalisableRange<float>(10.0f, 3000.0f, 0.01f, 0.3f), 700.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    

    /* ------------------------------------------------Ducking parallel. If parallel mode then delay 1 is controlled ------------------------------------------------*/


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Threshold_Parallel],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Threshold_Parallel],
                                                           NormalisableRange<float>(-60.0f, 0.0f, 0.01f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " dB";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Amount_Parallel],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Amount_Parallel],
                                                           NormalisableRange<float>(1.0f, 5.0f, 0.01f),5.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1);
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Attack_Parallel],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Attack_Parallel],
                                                           NormalisableRange<float>(0.01f, 3000.0f, 0.01f, 0.3f), 0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Ducking_Release_Parallel],version1},
                                                           MFXParameterLabel[mFXParameter_Ducking_Release_Parallel],
                                                           NormalisableRange<float>(10.0f, 3000.0f, 0.01f, 0.3f), 700.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    

    
    
    
    /* -------------------------------------------------------------------------Shaper--------------------------------------------------------------*/

    
    
    /* Shaper Panel Simple Filters Params*/
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Lowcut_Delay1],version1},
                                                           MFXParameterLabel[mFXParameter_Lowcut_Delay1],
                                                           NormalisableRange<float>(20.0f,20000.0f, 0.01, 0.3),20.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               (value < 1000.0f) ? String (value,1) + " Hz" : String (value/1000,1) + " KHz";
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Lowcut_Delay2],version1},
                                                           MFXParameterLabel[mFXParameter_Lowcut_Delay2],
                                                           NormalisableRange<float>(20.0f,20000.0f, 0.01, 0.3),20.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               (value < 1000.0f) ? String (value,1) + " Hz" : String (value/1000,1) + " KHz";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Highcut_Delay1],version1},
                                                           MFXParameterLabel[mFXParameter_Highcut_Delay1],
                                                           NormalisableRange<float>(20.0f,20000.0f, 0.01, 0.3),20000.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               (value < 1000.0f) ? String (value,1) + " Hz" : String (value/1000,1) + " KHz";
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Highcut_Delay2],version1},
                                                           MFXParameterLabel[mFXParameter_Highcut_Delay2],
                                                           NormalisableRange<float>(20.0f,20000.0f, 0.01, 0.3),20000.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               (value < 1000.0f) ? String (value,1) + " Hz" : String (value/1000,1) + " KHz";
                                                               
                                                               return valueToText;
                                                           }));
    
    
    
    
    
    
    /* -------------------------------------------------------Advanced Panel Params Envelope follower------------------------------------------------------------------------------------*/
    
    

    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Sensitivity],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_EnvF_Sensitivity],
                                                           NormalisableRange<float>(1.0f, 5.0f, 0.01f), 5.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1);
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Attack],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_EnvF_Attack],
                                                           NormalisableRange<float>(0.01f, 25000.0f, 0.01f, 0.3f),0.1f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value / 10.0f ,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Release],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_EnvF_Release],
                                                           NormalisableRange<float>(10.0f, 6000.0f, 0.01f, 0.3f),280.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText = String (value,1) + " ms";
                                                               
                                                               
                                                               return valueToText;
                                                           }));
    
    // String array Must match enum ModulationTargetIndex (in MFXUsedParameters)
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Target1],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Target1],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                       "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 1 Rate", "LFO 2 Rate"},0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Target1_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_EnvF_Target1_Amount],
                                                           NormalisableRange<float>( -1.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,1);
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Target2],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Target2],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                       "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 1 Rate", "LFO 2 Rate"},0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_EnvF_Target2_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_EnvF_Target2_Amount],
                                                           NormalisableRange<float>( -1.0f, 1.0f, 0.001f),0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,1);
                                                               
                                                               return valueToText;
                                                           }));
    
    
    /* -----------------------------------------------------------------Advanced Panel Params LFO1-----------------------------------------------------------------------------------------*/
    
        
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Sync],version1},
                                                          MFXParameterLabel[mFXParameter_Advanced_LFO1_Sync], static_cast<bool>(0.0f)));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Rate],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO1_Rate],
                                                           NormalisableRange<float>(0.01f, 20.0f, 0.0001f, 0.6f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2) + " Hz";
                                                               
                                                               return valueToText;
                                                           }));
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Note],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Note],
                                                                   juce::StringArray { "1", "1.", "1T", "1/2", "1/2.", "1/2T","1/4", "1/4.", "1/4T",
                                                                       "1/8", "1/8.", "1/8T", "1/16", "1/16.", "1/16T",
                                                                       "1/32", "1/32.", "1/32T", "1/64", "1/64.", "1/64T"}, 0.0f));

    
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Type],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Type],
                                                                   juce::StringArray { "Sin", "U-Ramp", "Square", "Triangle", "D-Ramp"}, 0.0f));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Target1],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Target1],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution",
                                                                       "Bit Crusher Downsample", "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 2 Rate", "Envelope Sensitivity"}, 0.0f));

    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Target1_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO1_Target1_Amount],
                                                           NormalisableRange<float>( 0.0f, 1.0f, 0.00001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2);
                                                               
                                                               return valueToText;
                                                           }));

    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Target2],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Target2],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution",
                                                                       "Bit Crusher Downsample", "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 2 Rate", "Envelope Sensitivity"}, 0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO1_Target2_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO1_Target2_Amount],
                                                           NormalisableRange<float>( 0.0f, 1.0f, 0.00001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2);
                                                               
                                                               return valueToText;
                                                           }));

    
    
    /* ------------------------------------------------------Advanced Panel Params LFO2----------------------------------------------------------------------*/
    
    
    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Sync],version1},
                                                          MFXParameterLabel[mFXParameter_Advanced_LFO2_Sync], static_cast<bool>(0.0f)));
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Rate],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO2_Rate],
                                                           NormalisableRange<float>(0.01f, 20.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2) + " Hz";
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Note],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Note],
                                                                   juce::StringArray { "1", "1.", "1T", "1/2", "1/2.", "1/2T","1/4", "1/4.", "1/4T",
                                                                       "1/8", "1/8.", "1/8T", "1/16", "1/16.", "1/16T",
                                                                       "1/32", "1/32.", "1/32T", "1/64", "1/64.", "1/64T"}, 0.0f));
    
    
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Type],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Type],
                                                                   juce::StringArray { "Sin", "U-Ramp", "Square", "Triangle", "D-Ramp"}, 0.0f));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Target1],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Target1],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution",
                                                                       "Bit Crusher Downsample", "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 1 Rate", "Envelope Sensitivity"}, 0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Target1_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO2_Target1_Amount],
                                                           NormalisableRange<float>( 0.0f, 1.0f, 0.00001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2);
                                                               
                                                               return valueToText;
                                                           }));
    
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Target2],version1},
                                                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Target2],
                                                                   juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                       "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                       "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                       "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                       "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                       "Echo/Out", "Dry/Out", "Stereo Width",
                                                                       "Delay 2 Wet","Chorus Wet","Phaser Wet", "Bit Crusher Wet","Bit Crusher Resolution",
                                                                       "Bit Crusher Downsample", "Bit Crusher Noise", "Reverb Wet","Reverb Size","Reverb Width",
                                                                       "LFO 1 Rate", "Envelope Sensitivity"}, 0.0f));
    
    
    
    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Advanced_LFO2_Target2_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Advanced_LFO2_Target2_Amount],
                                                           NormalisableRange<float>( 0.0f, 1.0f, 0.00001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [] (float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String (value,2);
                                                               
                                                               return valueToText;
                                                           }));
    
    

  

     /* -------------------------------------------------------------MIDI Panel ADSR ----------------------------------------------------------------------------------------------------*/




    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Attack],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Attack],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f), 0.1f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";


                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Decay],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Decay],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f), 0.3f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";

                                                                return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Sustain],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Sustain],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value, 1);

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Release],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Release],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f), 0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";

                                                               return valueToText;
                                                           }));

    // String array Must match enum ModulationTargetIndex (in MFXUsedParameters)
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Target1],version1},
                                                                  MFXParameterLabel[mFXParameter_Midi_ADSR_Target1],
                                                                  juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                  "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                  "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                  "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                  "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                  "Echo/Out", "Dry/Out", "Stereo Width",
                                                                  "Delay 2 Wet", "Chorus Wet", "Phaser Wet", "Bit Crusher Wet", "Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                  "Bit Crusher Noise", "Reverb Wet", "Reverb Size", "Reverb Width",
                                                                  "LFO 1 Rate", "LFO 2 Rate"}, 0.0f));



    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Target1_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Target1_Amount],
                                                           NormalisableRange<float>(-1.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String(value, 1);

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Target2],version1},
                                                                  MFXParameterLabel[mFXParameter_Midi_ADSR_Target2],
                                                                  juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                  "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                  "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                  "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                  "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                  "Echo/Out", "Dry/Out", "Stereo Width",
                                                                  "Delay 2 Wet", "Chorus Wet", "Phaser Wet", "Bit Crusher Wet", "Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                  "Bit Crusher Noise", "Reverb Wet", "Reverb Size", "Reverb Width",
                                                                  "LFO 1 Rate", "LFO 2 Rate"}, 0.0f));



    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR_Target2_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR_Target2_Amount],
                                                           NormalisableRange<float>(-1.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String(value, 1);

                                                               return valueToText;
                                                           }));


    /* -----------------------------------------------------------MIDI Panel ADSR 2 ----------------------------------------------------------------------------*/




    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Attack],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Attack],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f),0.1f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";


                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Decay],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Decay],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f), 0.3f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";


                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Sustain],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Sustain],
                                                           NormalisableRange<float>(0.0f, 1.0f, 0.001f),0.4f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = String(value, 1);

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Release],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Release],
                                                           NormalisableRange<float>(0.0f, 20.0f, 0.001f, 0.3f), 0.2f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText = value < 1.0f ? String(value * 1000, 1) + " ms" : String(value, 1) + " s";


                                                               return valueToText;
                                                            }));

    // String array Must match enum ModulationTargetIndex (in MFXUsedParameters)
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Target1],version1},
                                                                  MFXParameterLabel[mFXParameter_Midi_ADSR2_Target1],
                                                                  juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                  "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                  "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                  "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                  "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                  "Echo/Out", "Dry/Out", "Stereo Width",
                                                                  "Delay 2 Wet", "Chorus Wet", "Phaser Wet", "Bit Crusher Wet", "Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                  "Bit Crusher Noise", "Reverb Wet", "Reverb Size", "Reverb Width",
                                                                  "LFO 1 Rate", "LFO 2 Rate"}, 0.0f));



    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Target1_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Target1_Amount],
                                                           NormalisableRange<float>(-1.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                               String valueToText =
                                                               String(value, 1);

                                                               return valueToText;
                                                           }));


    params.push_back(std::make_unique<juce::AudioParameterChoice>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Target2],version1},
                                                                  MFXParameterLabel[mFXParameter_Midi_ADSR2_Target2],
                                                                  juce::StringArray { "None", "Delay 1 Left", "Delay 1 Right", "Delay 1 L+R", "Delay 1 Offset",
                                                                  "Feedback 1 Left", "Feedback 1 Right", "Feedback 1 L+R",
                                                                  "Delay 2 Left", "Delay 2 Right", "Delay 2 L+R", "Delay 2 Offset",
                                                                  "Feedback 2 Left", "Feedback 2 Right", "Feedback 2 L+R",
                                                                  "Filter Cutoff", "Filter Resonans", "Filter Drive",
                                                                  "Echo/Out", "Dry/Out", "Stereo Width",
                                                                  "Delay 2 Wet", "Chorus Wet", "Phaser Wet", "Bit Crusher Wet", "Bit Crusher Resolution", "Bit Crusher Downsample",
                                                                  "Bit Crusher Noise", "Reverb Wet", "Reverb Size", "Reverb Width",
                                                                  "LFO 1 Rate", "LFO 2 Rate"}, 0.0f));



    params.push_back(std::make_unique<AudioParameterFloat>(ParameterID {MFXParameterID[mFXParameter_Midi_ADSR2_Target2_Amount],version1},
                                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Target2_Amount],
                                                           NormalisableRange<float>(-1.0f, 1.0f, 0.001f), 0.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, int) -> String
                                                           {
                                                                String valueToText =
                                                                String(value, 1);

                                                                return valueToText;
                                                           }));


    /* ---------------------------------------------------------Midi in LFO retrigger ----------------------------------------------------------------*/

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Midi_LFO1_Retrigger],version1},
                                                          MFXParameterLabel[mFXParameter_Midi_LFO1_Retrigger], static_cast<bool>(0.0f)));

    params.push_back(std::make_unique<AudioParameterBool>(ParameterID {MFXParameterID[mFXParameter_Midi_LFO2_Retrigger],version1},
                                                          MFXParameterLabel[mFXParameter_Midi_LFO2_Retrigger], static_cast<bool>( 0.0f)));








    
    return { params.begin(), params.end() };
}





float PluginProcessor::getRmsLevelIn(const int channel)
{
    //jassert(channel >= 0 && channel < rmsCalculationBufferIn.getNumChannels());
    if (rmsCalculationBufferIn.getNumSamples() != 0)
    {
        rmsFifoIn.pull(rmsCalculationBufferIn.getWritePointer(channel), channel, rmsWindowSize);

        processLevelValue(rmsLevelsIn[channel], Decibels::gainToDecibels(rmsCalculationBufferIn.getRMSLevel(channel, 0, rmsWindowSize)));
        return rmsLevelsIn[channel].getCurrentValue();
    }
    else 
    {
        return 0.0f;
    }
}

float PluginProcessor::getRmsLevelOut(const int channel)

{
    //jassert(channel >= 0 && channel < rmsCalculationBufferOut.getNumChannels());
    if (rmsCalculationBufferOut.getNumSamples() != 0)
    {
        rmsFifoOut.pull(rmsCalculationBufferOut.getWritePointer(channel), channel, rmsWindowSize);

        processLevelValue(rmsLevelsOut[channel], Decibels::gainToDecibels(rmsCalculationBufferOut.getRMSLevel(channel, 0, rmsWindowSize)));
        return rmsLevelsOut[channel].getCurrentValue();
    }

    else 
    {
        return 0.0f;
    }
    
}




void PluginProcessor::processLevelValue(LinearSmoothedValue<float>& smoothedValue, const float value) const
{
    if (isSmoothed)
    {
        if (value < smoothedValue.getCurrentValue())
        {
            smoothedValue.setTargetValue(value);
            return;
        }
    }
    
    smoothedValue.setCurrentAndTargetValue(value);
}


//AudioListener==================================================================
// The idea of having them is to reduce reading reading UI user controlled params by blok to when param actually change - more cpu friendly.
// But this is not a big problem - so, a nice to have TODO thing. Se Viator project for this.
//
//void PluginProcessor::audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue)
//{
//
//      //  parameters.getParameter(MFXParameterID[parameterIndex])->beginChangeGesture();
//    
//    //DBG("Hello parameter changed"); // WORKS - Need further reseach - where to put BEGIN/END change!
//    
//    //parameters.getParameter(MFXParameterID[parameterIndex])->endChangeGesture();
//
//}
//
//void PluginProcessor::audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details) 
//{
//    // nothing to add
//}
//




void PluginProcessor::initializeDSP()
{
    for(int i = 0; i < 2; i++)
    {
        mInputGain[i] = std::make_unique<MFXGain>();
        mOutputGain[i] = std::make_unique<MFXGain>();
        mLowCutFilters[i] = std::make_unique<MFXSimpleFilter>();
        mHighCutFilters[i] = std::make_unique<MFXSimpleFilter>();
    }
    
    mReverb = std::make_unique<MFXReverb>(mData);
    mBitCrusher = std::make_unique<MFXBitCrusher>();
    mMoogFilter = std::make_unique<MFXMoogFilter>(mData);
    mDelayLine1 = std::make_unique<MFXDelayLine>();
    mDelayLine2 = std::make_unique<MFXDelayLine>();
    mStereo = std::make_unique<MFXStereo>();
    mChorus2 = std::make_unique <MFXChorus>();
}

void PluginProcessor::setDawPlayHeadInfo()
{
    // This only gives info about start of audio blok!
    
    // could encapsulate in new class that has a proces(AudioPlayHead playHead), instatiated and called from this class.
    if (AudioPlayHead* playHead = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo playposinfo; 
        /* details about the transport's
        position at the start of the current processing block - the first sample accuracy only. */
        auto playinfo = playHead->getPosition();
        
        // we track Bpm from DAW and use that to set DelayTimesInSamples, based on note
        double beatPrSecond = *playinfo->getBpm() / 60.0;
        mSampleTimeOneBeat = getSampleRate() / beatPrSecond;
        
        mBpm = *playinfo->getBpm();
      
        mIsPlaying = playinfo->getIsPlaying();
        
        mTimeInSamples = *playinfo->getTimeInSamples();
        
        mIsLooping = playinfo->getIsLooping();
    }
}



void PluginProcessor::setDelay1Parameters()
{
    
    /* Remember to call getDawPlayHeadInfo() before this function! Otherwise it will not get correct @param mSampleTimeOneBeat */
    

    
    mDelayLine1->setFeedBack(mDelay1LeftFeedback->load(), mDelay1RightFeedback->load());
    mDelayLine1->setSmoothingValue(mSmoothingDelay1->load());


    // If L/R time is linked and identical (ms, straight...) time consistency is vital - avoid phase like artefact when modulate time. 
    // Since L/R time value is not allways read equal (why?) - periodic small difference - only Left value is used in delayLine as parameter
    if (mTimeModeLeftDelay1->load() == mTimeModeRightDelay1->load() && mDelay1LinkTime->load() == 1.0f)
    {
        mDelayLine1->setParameters(mDelay1LeftTime->load(), mDelay1LeftTime->load(),
        mSampleTimeOneBeat, mDelay1Width->load(), mTimeModeLeftDelay1->load(),
        mTimeModeRightDelay1->load(), mPingPongDelay1->load());
        
    }
    else
    {
        
        mDelayLine1->setParameters(mDelay1LeftTime->load(), mDelay1RightTime->load(),
                                   mSampleTimeOneBeat, mDelay1Width->load(), mTimeModeLeftDelay1->load(),
                                   mTimeModeRightDelay1->load(), mPingPongDelay1->load());
    }

    
    mDelayLine1->setFadeParams(mFadeFrequencyDelay1->load(), mIsFadeModeDelay1->load());
    mDelayLine1->setDelayWideSampleTime(); // is also called in modulateDelay1Parameters
    
}


void PluginProcessor::modulateDelay1Parameters(int processLoopIndex)
{
    auto modSumLeftTime = 0.0f;
    auto modSumRightTime = 0.0f;
    auto modSumLeftRightTime = 0.0f;
    auto modSumLeftFeedback = 0.0f;
    auto modSumRightFeedback = 0.0f;
    auto modSumLeftRightFeedback = 0.0f;
    auto modSumOffset = 0.0f;
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    
    for (auto& set :mData.mModulationTargetData)
    {
        if (std::get<1>(set) == Delay1_Left)
        {
            if(std::get<0>(set) == EnvF)
                modSumLeftTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // * 10 will increase effect
            
            else
                modSumLeftTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);   // try (std::get<2>(set) * 10)
            
        }
        
        
        else if (std::get<1>(set) == Delay1_Right)
        {
            if(std::get<0>(set) == EnvF)
                modSumRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // will increase effect
            
            else
                modSumRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Delay1_LR)
        {
            if(std::get<0>(set) == EnvF)
                
                modSumLeftRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // will increase effect
            
            else
                modSumLeftRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        
        else if (std::get<1>(set) == Feedback1_Left)
        {
            modSumLeftFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Feedback1_Right)
        {
            modSumRightFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Feedback1_LR)
        {
            modSumLeftRightFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Delay1_Offset)
        {
            modSumOffset +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        
    }
    
    
    
    if ( !juce::approximatelyEqual<float>(modSumLeftTime, 0.0f) )
    {
        mDelayLine1->modulateTimeLeftWith(modSumLeftTime, mDelay1LeftTime->load());
        
    }
    if ( !juce::approximatelyEqual<float>(modSumRightTime, 0.0f) )
    {
        mDelayLine1->modulateTimeRightWith(modSumRightTime, mDelay1RightTime->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumLeftRightTime, 0.0f) )
    {
        mDelayLine1->modulateTimeLeftAndRightWith(modSumLeftRightTime, mDelay1LeftTime->load(), mDelay1RightTime->load() );
    }

    if ( !juce::approximatelyEqual<float>(modSumLeftFeedback, 0.0f) )
    {
        mDelayLine1->modulateFeedbackLeftWith(modSumLeftFeedback, mDelay1LeftFeedback->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumRightFeedback, 0.0f))
    {
        mDelayLine1->modulateFeedbackRightWith(modSumRightFeedback, mDelay1RightFeedback->load());
        
    }
    if ( !juce::approximatelyEqual<float>(modSumLeftRightFeedback, 0.0f) )
    {
        mDelayLine1->modulateFeedbackLeftRightWith(modSumLeftRightFeedback,mDelay1LeftFeedback->load(),  mDelay1RightFeedback->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumOffset, 0.0f ))
    {
        mDelayLine1->modulateWideWith(modSumOffset, mDelay1Width->load());
    }
    
    
    mDelayLine1->setDelayWideSampleTime();
    mDelayLine1->setFeedbackGainSmooth();  
}


void PluginProcessor::setDelay2Parameters()
{
    
    /* Remember to call getDawPlayHeadInfo() before this function! Otherwise it will not get correct @param mSampleTimeOneBeat */
    

    /* in serial mode @param wetLevel (DUAL delay) sets balance of Dual delay (full at 1)
    and main delay (main delay full at 0, gone at 1). 0.5 is equal balanced.
    in parallel mode @param wetLevel (DUAL delay) only sets dry/wet of Dual delay, and is not connected with main delay */

    
    mDelayLine2->setFeedBack(mDelay2LeftFeedback->load(), mDelay2RightFeedback->load());
    mDelayLine2->setSmoothingValue(mSmoothingDelay2->load());
    mDelayLine2->setFadeParams(mFadeFrequencyDelay2->load(), mIsFadeModeDelay2->load());
    
    
    // If L/R time is linked and identical (ms, straight...) time consistency is vital - avoid phase like artefact when modulate time. 
    // Since L/R time value is not allways read equal (why?) - periodic small difference - only Left value is used in delayLine as parameter
    if (mTimeModeLeftDelay2->load() == mTimeModeRightDelay2->load() && mDelay2LinkTime->load() == 1.0f)
    {
        mDelayLine2->setParameters(mDelay2LeftTime->load(), mDelay2LeftTime->load(),
                                   mSampleTimeOneBeat, mDelay2Width->load(),
                                   mTimeModeLeftDelay2->load(),
                                   mTimeModeRightDelay2->load(),
                                   mPingPongDelay2->load(), mMixDelay2->load(),
                                   (mChainModeDelay2->load() == mDelayLine2->mChainedDelay_serial) ? mDelayLine2->mChainedDelay_serial
                                                                                                   : mDelayLine2->mChainedDelay_parallel);


    }
    else
    {
        mDelayLine2->setParameters(mDelay2LeftTime->load(), mDelay2RightTime->load(),
                                   mSampleTimeOneBeat, mDelay2Width->load(),
                                   mTimeModeLeftDelay2->load(),
                                   mTimeModeRightDelay2->load(),
                                   mPingPongDelay2->load(), mMixDelay2->load(),
                                   (mChainModeDelay2->load() == mDelayLine2->mChainedDelay_serial) ? mDelayLine2->mChainedDelay_serial
                                                                                                   : mDelayLine2->mChainedDelay_parallel);
        
    }


    mDelayLine2->setDelayWideSampleTime();
    
}


void PluginProcessor::modulateDelay2Parameters(int processLoopIndex)
{
    auto modSumLeftTime = 0.0f;
    auto modSumRightTime = 0.0f;
    auto modSumLeftRightTime = 0.0f;
    auto modSumLeftFeedback = 0.0f;
    auto modSumRightFeedback = 0.0f;
    auto modSumLeftRightFeedback = 0.0f;
    auto modSumOffset = 0.0f;
    auto modSumWet = 0.0f;
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    
    for (auto& set :mData.mModulationTargetData)
    {
        if (std::get<1>(set) == Delay2_Left)
        {
            if(std::get<0>(set) == EnvF)
                modSumLeftTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // will increase effect
            
            else
                modSumLeftTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        
        else if (std::get<1>(set) == Delay2_Right)
        {
            if(std::get<0>(set) == EnvF)
                modSumRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // will increase effect
            
            else
                modSumRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Delay2_LR)
        {
            if(std::get<0>(set) == EnvF)
                
                modSumLeftRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set); // will increase effect
            
            else
                modSumLeftRightTime +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        
        else if (std::get<1>(set) == Feedback2_Left)
        {
            modSumLeftFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Feedback2_Right)
        {
            modSumRightFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Feedback2_LR)
        {
            modSumLeftRightFeedback +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == Delay2_Offset)
        {
            modSumOffset +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        else if (std::get<1>(set) == Delay2_Wet)
        {
            modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        
    }
    
    
    if ( !juce::approximatelyEqual<float>(modSumLeftTime, 0.0f) )
    {
        mDelayLine2->modulateTimeLeftWith(modSumLeftTime, mDelay2LeftTime->load());
        
        
    }
    if ( !juce::approximatelyEqual<float>(modSumRightTime, 0.0f) )
    {
        mDelayLine2->modulateTimeRightWith(modSumRightTime, mDelay2RightTime->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumLeftRightTime, 0.0f) )
    {
        mDelayLine2->modulateTimeLeftAndRightWith(modSumLeftRightTime, mDelay2LeftTime->load(), mDelay2RightTime->load() );
    }
    
    if ( !juce::approximatelyEqual<float>(modSumLeftFeedback, 0.0f) )
    {
        mDelayLine2->modulateFeedbackLeftWith(modSumLeftFeedback, mDelay2LeftFeedback->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumRightFeedback, 0.0f))
    {
        mDelayLine2->modulateFeedbackRightWith(modSumRightFeedback, mDelay2RightFeedback->load());
        
    }
    if ( !juce::approximatelyEqual<float>(modSumLeftRightFeedback, 0.0f) )
    {
        mDelayLine2->modulateFeedbackLeftRightWith(modSumLeftRightFeedback,mDelay2LeftFeedback->load(),  mDelay2RightFeedback->load());
    }
    if ( !juce::approximatelyEqual<float>(modSumOffset, 0.0f ))
    {
        mDelayLine2->modulateWideWith(modSumOffset, mDelay2Width->load());
    }
  
    if ( !juce::approximatelyEqual<float>(modSumWet, 0.0f) )
    {
        mDelayLine2->modulateDryWetWith(modSumWet, mMixDelay2->load());
        //mDelay2ModulateWet = modSumWet;
    }
    
    
    mDelayLine2->setDelayWideSampleTime();
    mDelayLine2->setFeedbackGainSmooth();
    

}

void PluginProcessor::procesDualFilterParallel()
{
    auto& highCutDelay1 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Highcut_Delay1]);
    auto& lowCutDelay1 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Lowcut_Delay1]);
    auto& highCutDelay2 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Highcut_Delay2]);
    auto& lowCutDelay2 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Lowcut_Delay2]);
    
    constexpr int numChannels = 2;
    auto tempBufferDelay1 = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
    
    
    mHighCutFilters[0]->updateParameters(highCutDelay1, mFilterType_LowPass);
    mHighCutFilters[0]->process(tempBufferDelay1);
    
    mLowCutFilters[0]->updateParameters(lowCutDelay1, mFilterType_HighPass);
    mLowCutFilters[0]->process(tempBufferDelay1);
    
    constexpr int startChannel = 2;
    constexpr int numChannelMid = 2;
    auto tempBufferDelay2 = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers() + startChannel, numChannelMid, mWetBuffer.getNumSamples());
    
    
    mHighCutFilters[1]->updateParameters(highCutDelay2, mFilterType_LowPass);
    mHighCutFilters[1]->process(tempBufferDelay2);
    
    mLowCutFilters[1]->updateParameters(lowCutDelay2, mFilterType_HighPass);
    mLowCutFilters[1]->process(tempBufferDelay2);
}


void PluginProcessor::procesDualFilterSerial()
{
    
    // We create a tempBuffer only containing the channels we need, and because filter requires a 2 channel only audioBuffer
    constexpr int numChannels = 2;
    auto tempBuffer = juce::AudioBuffer<float>(mWetBuffer.getArrayOfWritePointers(), numChannels, mWetBuffer.getNumSamples());
    
    
    auto& highCutDelay1 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Highcut_Delay1]);
   
    auto& lowCutDelay1 = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Lowcut_Delay1]);
    
    
    
    
    mLowCutFilters[0]->updateParameters(lowCutDelay1, mFilterType_HighPass);
    mLowCutFilters[0]->process(tempBuffer);
    
    mHighCutFilters[0]->updateParameters(highCutDelay1, mFilterType_LowPass);
    mHighCutFilters[0]->process(tempBuffer);
}


void PluginProcessor::setDuckingParameters()
{
    auto& threshold = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Threshold]);
    auto& amount = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Amount]);
    auto& attack = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Attack]);
    auto& release = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Release]);
    
    
    mDucking.setThreshold(threshold);
    mDucking.setRatio(amount);
    mDucking.setAttack(attack);
    mDucking.setRelease(release);
}


void PluginProcessor::setDuckingParallelParameters()
{
    auto& threshold = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Threshold_Parallel]);
    auto& amount = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Amount_Parallel]);
    auto& attack = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Attack_Parallel]);
    auto& release = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_Ducking_Release_Parallel]);
    
    
    mDuckingParallel.setThreshold(threshold);
    mDuckingParallel.setRatio(amount);
    mDuckingParallel.setAttack(attack);
    mDuckingParallel.setRelease(release);
}


void PluginProcessor::setGlobalDryWetParameters(int processLoopIndex)
{
    
    
    const auto& wet = mGlobalWet->load();
    const auto& dry = mGlobalDry->load();
    
    
    auto modSumWet = 0.0f;
    auto modSumDry = 0.0f;
    
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                           mData.mModulatorOutput[LFO2].getWritePointer(0),
                                           mData.mModulatorOutput[EnvF].getWritePointer(0),
                                           mData.mModulatorOutput[adsr].getWritePointer(0),
                                           mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    for (auto set : mData.mModulationTargetData)
    {
        if (std::get<1>(set) == WetOut)
        {
            modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
            
        }
        else if (std::get<1>(set) == DryOut)
        {
            modSumDry +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
    }
    
    
    if (!juce::approximatelyEqual<float>(modSumWet, 0.0f))
    {
        auto modWet = modSumWet > 0.0f ? wet + ((2.0f - wet) * modSumWet) : wet + (wet * modSumWet);
        mGainMappedWet = jmap(modWet, 0.0f, 2.0f, -24.0f, 24.0f);
        mGainMappedWet = Decibels::decibelsToGain(mGainMappedWet, -24.0f);
        
    }
    else
    {
        mGainMappedWet = jmap(wet, 0.0f, 2.0f, -24.0f, 24.0f); // was -12/12
        mGainMappedWet = Decibels::decibelsToGain(mGainMappedWet, -24.0f); // was -12
    }
    
    
    mSmoothingGlobalWet.setTargetValue(mGainMappedWet);
    
    
    if (!juce::approximatelyEqual<float>(modSumDry, 0.0f))
    {
        
        auto modDry = modSumDry > 0.0f ? dry + ((2.0f - dry) * modSumDry) : dry + (dry * modSumDry);
        mGainMappedDry = jmap(modDry, 0.0f, 2.0f, -12.0f, 12.0f);
        mGainMappedDry = Decibels::decibelsToGain(mGainMappedDry, -12.0f);        
    }
    else
    {
        mGainMappedDry = jmap(dry, 0.0f, 2.0f, -12.0f, 12.0f);
        mGainMappedDry = Decibels::decibelsToGain(mGainMappedDry, -12.0f);
    }
    
    mSmoothingGlobalDry.setTargetValue(mGainMappedDry);
}

// Update FX params ================================================================================

void PluginProcessor::setChorusParameters()
{
    auto& modulationRate = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusRate]);
    auto& modulationDepth = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusDepth]);
    auto& dryWetChorus = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusWetDry]);
    
    auto& centerDelay = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusWidth]);
    auto& feedback = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_ChorusFeedback]);
    
    
    auto modSumWet = 0.0f;
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    for (auto set : mData.mModulationTargetData)
    {
        // read modulation source at [0] because lfo is phase locked each block. I modulated by block since Chorus::process not by sample 
        if (std::get<1>(set) == ChorusWet)
            modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
    }
    
    if (modSumWet != 0)
    {
        auto modWet = modSumWet > 0.0f ? dryWetChorus + ((1.0f - dryWetChorus) * modSumWet) : dryWetChorus + (dryWetChorus * modSumWet);
        
        mChorus2->setDryWetParameter(modWet);
    }
    else
    {
        mChorus2->setDryWetParameter(dryWetChorus);
    }
    
    
    mChorus2->setRateParameter(modulationRate.load()); 
    mChorus2->setDepthParameter(modulationDepth.load());
    mChorus2->setStereoWidthParameter(centerDelay.load());
    mChorus2->setFeedbackGainParameter(feedback.load()); 
}


void PluginProcessor::setBitcrusherParameters()
{
    // Modulated by block (reading phase locked lfo position, see setLFOParameters). Can I here any difference compared to by sample?
    
    float modSumWet = 0.0f;
    float modSumDownSample = 0.0f;
    float modSumBitDepth = 0.0f;
    float modSumNoise = 0.0f;
    
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    for (auto& set : mData.mModulationTargetData)
    {
        // read modulation source at [0] because lfo is phase locked each block. What if envelopefollwer??
        if (std::get<1>(set) == BitCrusherWet)
            modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
        
        else if (std::get<1>(set) == BitCrusherRate)
                 modSumDownSample +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
        
        else if (std::get<1>(set) == BitCrusherBit)
                 modSumBitDepth +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
        
        else if (std::get<1>(set) == BitCrusherNoise)
                 modSumNoise +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
        
    }
    
    
    if (modSumWet != 0)
    {
        auto dryWet = mBitCrusherDryWetMix->load();
        const auto& modWet = modSumWet > 0.0f ? dryWet + ((1.0f - dryWet) * modSumWet) : dryWet + (dryWet * modSumWet);
        mBitCrusher->setDryWetMix(modWet);
        mmBitCrusherActiveWet = modWet;
    }
    
    else
    {
        mBitCrusher->setDryWetMix(mBitCrusherDryWetMix->load());
    }
    
    
    
    if (modSumBitDepth != 0)
    {
        auto currentRes = mResolution->load();
        const auto& resolutionMod = modSumBitDepth > 0.0f ? currentRes + ((16.0f - currentRes) * modSumBitDepth)
        : std::fmax( currentRes + (currentRes * modSumBitDepth), 2.0f);
        
        mBitCrusher->setResolution(resolutionMod);       
    }
    
    else
    {
        mBitCrusher->setResolution(mResolution->load());
    }
    
    
    
    if (modSumDownSample != 0)
    {
        auto currentDownSample = mDownsample->load();
        const auto& downsampleMod = modSumDownSample > 0.0f ? currentDownSample + ((50.0f - currentDownSample) * modSumDownSample)
        : std::fmax( currentDownSample + (currentDownSample * modSumDownSample), 1.0f);
        
        mBitCrusher->setDownsample(downsampleMod);
    }
    
    else
    {
        mBitCrusher->setDownsample(mDownsample->load());
    }
    
    
    if (modSumNoise != 0)
    {
        auto noiseModulated = modSumNoise > 0.0f ? mNoise->load() + ((100.0f - mNoise->load()) * modSumNoise)
        : mNoise->load() + (mNoise->load() * modSumNoise);
        
        mBitCrusher->setNoise(noiseModulated);
    }
    
    else
    {
        mBitCrusher->setNoise(mNoise->load());
    }
    
    
}


void PluginProcessor::setReverbParameters()
{
    auto& roomSize = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbSize]);
    auto& width = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbWidth]);
    auto& damping = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbDamping]);
    auto& dryLevel = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbDry]);
    auto& wetLevel = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbWet]);
    auto& freezeMode = *parameters.getRawParameterValue (MFXParameterID[mFXParameter_ReverbFreeze]);

    mReverb->setReverbParams(roomSize, width,
                             damping, dryLevel,
                             wetLevel, freezeMode);
}

void PluginProcessor::setPhaserParameters()
{
    auto& rate = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserRate]);
    auto& depth = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserDepth]);
    auto& dryWet = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserDryWet]);
    auto& centerFrequency = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserCenterFrequency]);
    auto& feedback = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_PhaserFeedback]);
    
    
        auto modSumWet = 0.0f;
    
        float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                              mData.mModulatorOutput[LFO2].getWritePointer(0),
                                              mData.mModulatorOutput[EnvF].getWritePointer(0),
                                              mData.mModulatorOutput[adsr].getWritePointer(0),
                                              mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
        for (auto set : mData.mModulationTargetData)
        {
            if (std::get<1>(set) == PhaserWet)
                modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][0] * std::get<2>(set);
        }
    
    
    
    
        if (modSumWet != 0)
        {
            auto modWet = modSumWet > 0.0f ? dryWet + ((1.0f - dryWet) * modSumWet) : dryWet + (dryWet * modSumWet);
            mJPhasor.setMix(modWet);
        }
        else
        {
            mJPhasor.setMix(dryWet);
        }
    
    
    
    
    mJPhasor.setRate(rate); //(0-100)
    mJPhasor.setDepth(depth);
    mJPhasor.setCentreFrequency(centerFrequency);
    mJPhasor.setFeedback(feedback);
  
}

// Update modulator params - advanced settings ================================================================================

void PluginProcessor::setEnvelopeFollowerParameters()
{
    
    auto& sensitivity = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Sensitivity]);
    auto& attack = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Attack]);
    auto& release = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Release]);
    

    /** Sets the ratio (must be higher or equal to 1) */
    mData.mEnvelopeFollower->setRatio (sensitivity);
    mData.mCurrentEnvParamSensitivity = sensitivity;


    
    mData.mEnvelopeFollower->setThreshold(-60.0f);
    
    /** Sets the attack time in milliseconds */
    mData.mEnvelopeFollower->setAttack (attack);
    
    /** Sets the release time in milliseconds */
    mData.mEnvelopeFollower->setRelease (release);
}

void PluginProcessor::setLFO1Parameters()
{
    auto& rate = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Rate]);
    auto& note = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Note]); // use this, isSync and bpm (mBpm)
    
    auto& type = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Type]);
    auto& isSync = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Sync]);
    
    
    mData.mLfoModulator1->setType(static_cast<LfoStyle>(type.load()));

    mData.isLFO1Trigger = mIsLFO1Retrigger->load();
    
    
      if (isSync && mIsLFO1Retrigger->load() !=1.0f)
      {
        
        if ((mIsPlaying && !mResetLFO1PhaseOnStart ) || mTimeInSamples != mCurrentPositionLFO1)
        {
            // Get current sampletime position in DAW coresponding to radians/offsets (mTimeInSamples == DAW timeline in samples) 
            double phaseInc = static_cast<double>( ((mData.mLfoModulator1->getSyncFrequency() / getSampleRate()) * juce::MathConstants<double>::twoPi) *  mTimeInSamples);
            // We wrap phaseInc around 2Pi to keep it within cycle 2Pi range.
            mData.mLfoModulator1->reset(std::fmod( phaseInc, juce::MathConstants<double>::twoPi));
            mResetLFO1PhaseOnStart = true;
            mCurrentPositionLFO1 = mTimeInSamples;
        }
    
      }
    
    
    if (!mIsPlaying)
    {
        mResetLFO1PhaseOnStart = false;
    }
    
    
    
    if (!mIsPlaying && isSync && mIsLFO1Retrigger->load() != 1.0f)
    {
        double phaseInc = static_cast<double>( (mData.mLfoModulator1->getSyncFrequency() / getSampleRate()) * (juce::MathConstants<double>::twoPi * mSumBlokData ) );
        mData.mLfoModulator1->reset(std::fmod( phaseInc, juce::MathConstants<double>::twoPi));
    }
    
    

    
    
    mData.mCurrentParamBpm = mBpm;
    mData.mCurrentLFO1ParamRate = rate;
    mData.mCurrentLFO1ParamNote = note;
    mData.mIsSyncLFO1 = isSync;

   
    
    
    
}


void PluginProcessor::setLFO2Parameters()
{
    
    auto& rate = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Rate]);
    auto& note = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Note]);
    auto& type = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Type]);
    auto& isSync = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Sync]);
    
    mData.mLfoModulator2->setType(static_cast<LfoStyle>(type.load()));
    
    mData.isLFO2Trigger = mIsLFO2Retrigger->load();
    
    
        
    if (isSync && mIsLFO2Retrigger->load() != 1.0f)
    {
        
        if ((mIsPlaying && !mResetLFO2PhaseOnStart ) || mTimeInSamples != mCurrentPositionLFO2)
        {
            // Get current sampletime position in DAW coresponding to radians/offsets (mTimeInSamples == DAW timeline in samples) 
            double phaseInc = static_cast<double>( ((mData.mLfoModulator2->getSyncFrequency() / getSampleRate()) * juce::MathConstants<double>::twoPi) *  mTimeInSamples);
            
            // We wrap phaseInc around 2Pi to keep it within cycle 2Pi range.
            mData.mLfoModulator2->reset(std::fmod( phaseInc, juce::MathConstants<double>::twoPi)); 
            mResetLFO2PhaseOnStart = true;
            mCurrentPositionLFO2 = mTimeInSamples;
            
        }
        
    }

    
    
    if (!mIsPlaying)
    {
        mResetLFO2PhaseOnStart = false;
    }
    
    
    
    if (!mIsPlaying && isSync && mIsLFO2Retrigger->load() != 1.0f)
    {
        // Not sure why I did this ?? TODO:: do I need this?
        // If not playing, we simulate current sampletime position in DAW coresponding to radians/offsets (mSumBlokDatas == DAW time in samples) 
        double phaseInc = static_cast<double>( (mData.mLfoModulator2->getSyncFrequency() / getSampleRate()) * (juce::MathConstants<double>::twoPi * mSumBlokData ) );
        // We wrap phaseInc around 2Pi to keep it within cycle 2Pi range.
        mData.mLfoModulator2->reset(std::fmod( phaseInc, juce::MathConstants<double>::twoPi));
    }
    
    
    
    
    mData.mCurrentParamBpm = mBpm;
    mData.mCurrentLFO2ParamRate = rate;
    mData.mCurrentLFO2ParamNote = note;
    mData.mIsSyncLFO2 = isSync;
    
    
}

// TO DO: move responsibility "setting/update" the ModulationTargetData tuple to modulationData.h. Simply pass index, source, value and target as parameter.  
void PluginProcessor::updateModulationTargetData()
{    
    // LFO1
    const auto& target1LFO1 = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_LFO1_Target1]);
    const auto& target1LFO1Amount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Target1_Amount]);
    const auto& target2LFO1 = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_LFO1_Target2]);
    const auto& target2LFO1Amount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Target2_Amount]);
    std::get<0>(mData.mModulationTargetData[0]) = LFO1;
    std::get<1>(mData.mModulationTargetData[0]) = (target1LFO1.load() == 31) ? 33 : target1LFO1.load();    // == 30  ? 32
    std::get<2>(mData.mModulationTargetData[0]) = target1LFO1Amount.load();
    std::get<0>(mData.mModulationTargetData[1]) = LFO1;
    std::get<1>(mData.mModulationTargetData[1]) = (target2LFO1.load() == 31) ? 33 : target2LFO1.load();
    std::get<2>(mData.mModulationTargetData[1]) = target2LFO1Amount.load();
    
    // LFO2
    const auto& target1LFO2 = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_LFO2_Target1]);
    const auto& target1LFO2Amount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Target1_Amount]);
    const auto& target2LFO2 = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_LFO2_Target2]);
    const auto& target2LFO2Amount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Target2_Amount]);
    std::get<0>(mData.mModulationTargetData[2]) = LFO2;
    std::get<1>(mData.mModulationTargetData[2]) = target1LFO2;
    std::get<2>(mData.mModulationTargetData[2]) = target1LFO2Amount.load();
    std::get<0>(mData.mModulationTargetData[3]) = LFO2;
    std::get<1>(mData.mModulationTargetData[3]) = target2LFO2;
    std::get<2>(mData.mModulationTargetData[3]) = target2LFO2Amount.load();
    
    
    // Envelope Follower
    const auto& target1EnvF = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_EnvF_Target1]);
    const auto& target1EnvFAmount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Target1_Amount]);
    const auto& target2EnvF = *parameters.getRawParameterValue( MFXParameterID[mFXParameter_Advanced_EnvF_Target2]);
    const auto& target2EnvFAmount = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Target2_Amount]);
    std::get<0>(mData.mModulationTargetData[4]) = EnvF;
    std::get<1>(mData.mModulationTargetData[4]) = target1EnvF.load() == 32 ? 33 : target1EnvF.load(); // ==31 ? 32
    std::get<2>(mData.mModulationTargetData[4]) = target1EnvFAmount.load();
    std::get<0>(mData.mModulationTargetData[5]) = EnvF;
    std::get<1>(mData.mModulationTargetData[5]) = target2EnvF.load() == 32 ? 33 : target2EnvF.load();
    std::get<2>(mData.mModulationTargetData[5]) = target2EnvFAmount.load();

     

    // MIDI ADSR
    
    std::get<0>(mData.mModulationTargetData[6]) = adsr;
    std::get<1>(mData.mModulationTargetData[6]) = mADSRTarget1->load() == 32 ? 33 : mADSRTarget1->load(); // ==31 ? 32
    std::get<2>(mData.mModulationTargetData[6]) = mADSRTarget1_Amount->load();
    std::get<0>(mData.mModulationTargetData[7]) = adsr;
    std::get<1>(mData.mModulationTargetData[7]) = mADSRTarget2->load() == 32 ? 33 : mADSRTarget2->load();
    std::get<2>(mData.mModulationTargetData[7]) = mADSRTarget2_Amount->load();  


    // MIDI ADSR 2

    std::get<0>(mData.mModulationTargetData[8]) = adsr2;
    std::get<1>(mData.mModulationTargetData[8]) = mADSR2Target1->load() == 32 ? 33 : mADSR2Target1->load(); // ==31 ? 32
    std::get<2>(mData.mModulationTargetData[8]) = mADSR2Target1_Amount->load();
    std::get<0>(mData.mModulationTargetData[9]) = adsr2;
    std::get<1>(mData.mModulationTargetData[9]) = mADSR2Target2->load() == 32 ? 33 : mADSR2Target2->load();
    std::get<2>(mData.mModulationTargetData[9]) = mADSR2Target2_Amount->load();
}


void PluginProcessor::setMoogFilterParameters()
{
    
    auto& cutoff = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_FilterCutoff]);
    auto& resonans = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_FilterResonans]);
    auto& drive = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_FilterDrive]);
    auto& type = *parameters.getRawParameterValue(MFXParameterID[mFXParameter_FilterType]);

    mMoogFilter->updateParameters(type, cutoff, resonans, drive);
    
}

void PluginProcessor::setStereoWidth()
{
    mStereo->setWidth(mStereoWidth->load());
}

void PluginProcessor::modulateStereoWidth(int processLoopIndex)
{
    
    
    auto modSumWidth = 0.0f;
    
    
    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    for (auto& set : mData.mModulationTargetData)
    {
        if (std::get<1>(set) == StereoWidth)
            modSumWidth +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
    }
    
    
    if (!juce::approximatelyEqual<float>(modSumWidth, 0.0f) )
    {
        mStereo->setWidth(mStereoWidth->load(), modSumWidth);
    }

    
    
}



