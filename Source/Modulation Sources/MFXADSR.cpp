/*
  ==============================================================================

    MFXADSR.cpp
   
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXADSR.h"

// Must be called before using a instance of this class, in pluginProcessor::prepare(..)
void MFXADSR::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    setSampleRate(spec.sampleRate);
}

// Call this before ADSR::noteOn() that trigger envelope start. 
void MFXADSR::updateADSR () noexcept
{
    if (adsrParams.attack != mAttack || adsrParams.decay != mDecay || adsrParams.sustain != mSustain || adsrParams.release != mRelease)
    {
        adsrParams.attack = mAttack;
        adsrParams.decay = mDecay;
        adsrParams.sustain = mSustain;
        adsrParams.release = mRelease;
       
        setParameters(adsrParams);
        
    }
    
        
    
    
    

    
}


// Must be called before updateADSR(). Ideally you should call this from pluginProcessor class (controller) in process(..), making sure parameters are updated in realtime.
void MFXADSR::setParams(const float attack, const float decay, const float sustain, const float release) noexcept
{
    // We only setParams if changed. 
    if (mAttack != attack || mDecay != decay || mSustain != sustain|| mRelease != release)
    {
        mAttack = attack;
        mDecay = decay;
        mSustain = sustain;
        mRelease = release;

        
    }
    
    

}

