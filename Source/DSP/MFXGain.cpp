/*
  ==============================================================================

    MFXGain.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXGain.h"
#include "MFXAudioHelpers.h"





MFXGain::MFXGain()
{
}

MFXGain::~MFXGain()
{
}


void MFXGain::process(float* inAudioChannel, float inGain, int inNumSamplesToRender)
{
    inGain = Decibels::decibelsToGain(inGain, -70.0f); 
    
    FloatVectorOperations::multiply(inAudioChannel, inGain, inNumSamplesToRender);
}


