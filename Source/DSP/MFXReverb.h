/*
  ==============================================================================

    MFXReverb.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXUsedParameters.h"
#include "ModulationData.h"
#include "MFXJuce_Reverb.h"


class MFXReverb
{
    
public:
    
    MFXReverb(ModulationData& data);
    ~MFXReverb();
    
    

    bool isEnabled () const noexcept;
    

    void setEnabled (bool newValue) noexcept;
    

    void prepareToProces (const double sampleRate, int samplesPerBlock, int totalNumOutputChannels);
    

    void reset () noexcept;
    

    void process (AudioBuffer<float>& inBuffer ) noexcept;
    
    void prepareModulation(const int& processLoopIndex) noexcept;
    
    void setReverbParams(const float roomSize, const float width, const float damping,
                         const float dryLevel, const float wetLevel,
                         const float freezeMode) noexcept;
    
private:
    
    Reverb_Redesign reverb; 
    Reverb_Redesign::Parameters reverbParams;
    ModulationData& mData;
    float mCurrentWetLevel {0.0f};
    float mCurrentSizeLevel {0.0f};
    float mCurrentWidthLevel {0.0f};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXReverb);
    
    
};


