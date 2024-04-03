/*
  ==============================================================================

    MFXReverb.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXReverb.h"


MFXReverb::MFXReverb(ModulationData& data): mData(data){}

MFXReverb::~MFXReverb(){}


void MFXReverb::prepareToProces (const double sampleRate, int samplesPerBlock, int totalNumOutputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = totalNumOutputChannels;
    reverb.reset();
    reverb.setSampleRate(sampleRate);
}


void MFXReverb::reset () noexcept
{
    reverb.reset();
}


void MFXReverb::process (AudioBuffer<float>& inBuffer ) noexcept
{
    
    for (int sample = 0; sample < inBuffer.getNumSamples(); sample++)
    {
        prepareModulation(sample);
        reverb.processStereo(&inBuffer.getWritePointer(0)[sample], &inBuffer.getWritePointer(1)[sample], 1); 
    }
    
 
}

// Call this before reverb.processStereo(...). See process() above.
void MFXReverb::prepareModulation(const int &processLoopIndex) noexcept
{

    auto modSumWet = 0.0f;
    auto modSumSize = 0.0f;
    auto modSumWidth = 0.0f;
    
    

    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer(0),
                                          mData.mModulatorOutput[LFO2].getWritePointer(0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
    
    
    for (auto& set : mData.mModulationTargetData)
    {
        if (std::get<1>(set) == ReverbWet)
            modSumWet +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        
        else if (std::get<1>(set) == ReverbSize)
        {
            modSumSize +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
        
        else if (std::get<1>(set) == ReverbWidth)
        {
            modSumWidth +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
        }
    }
    
    
    
    
    if (modSumWet != 0)
    {
        
        reverbParams.wetLevel = modSumWet > 0.0f ? mCurrentWetLevel + ((1.0f - mCurrentWetLevel) * modSumWet) : mCurrentWetLevel + (mCurrentWetLevel * modSumWet);
        //Balanced dry level in relation to wet level
        auto dryMix = jmap(1.0f - mCurrentWetLevel, 0.0f, 1.0f, 0.0f, 0.6f);
        reverbParams.dryLevel = dryMix;

    }

    
    if (modSumSize != 0)
    {
        reverbParams.roomSize = modSumSize > 0.0f ? mCurrentSizeLevel + ((1.0f - mCurrentSizeLevel) * modSumSize)
        : mCurrentSizeLevel + (mCurrentSizeLevel * modSumSize);
    }
    
    
    
    if (modSumWidth != 0)
    {
        reverbParams.width = modSumWidth > 0.0f ? mCurrentWidthLevel + ((1.0f - mCurrentWidthLevel) * modSumWidth)
        : mCurrentWidthLevel + (mCurrentWidthLevel * modSumWidth);
        
        
        
    }

    if (modSumSize != 0 || modSumWidth != 0 || modSumWet != 0)
        reverb.setParameters (reverbParams);
    
}


void MFXReverb::setReverbParams(const float roomSize, const float width, const float damping,
                                const float dryLevel, const float wetLevel,
                                const float freezeMode) noexcept
{
    reverbParams.roomSize = roomSize;
    reverbParams.width = width;
    reverbParams.damping = damping;
    
    mCurrentWetLevel = wetLevel;
    mCurrentSizeLevel = roomSize;
    mCurrentWidthLevel = width;
    
    
    // Balanced dry level in relation to wet level that is between 0 - 0.35
    auto dryMix = jmap(1.0f - (wetLevel * 2.8f), 0.0f, 1.0f, 0.0f, 0.6f);
    reverbParams.dryLevel = dryMix;
    
    reverbParams.wetLevel = wetLevel;
    reverbParams.freezeMode = freezeMode;
    
    
    
    reverb.setParameters (reverbParams);
    
}
