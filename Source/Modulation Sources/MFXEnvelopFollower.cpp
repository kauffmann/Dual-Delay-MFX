



/*
 ==============================================================================
 
 This file is part of the JUCE library.
 Copyright (c) 2020 - Raw Material Software Limited
 
 JUCE is an open source library subject to commercial or open-source
 licensing.
 
 By using JUCE, you agree to the terms of both the JUCE 6 End-User License
 Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).
 
 End User License Agreement: www.juce.com/juce-6-licence
 Privacy Policy: www.juce.com/juce-privacy-policy
 
 Or: You may also use this code under the terms of the GPL v3 (see
 www.gnu.org/licenses).
 
 JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
 EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
 DISCLAIMED.
 
 ==============================================================================
 



 ==============================================================================
 
 MFXEnvelopeFollower.cpp.
 
 Author:  Customized to project by Michael Kauffmann
 A envelope follower created using JUCE compressor class. Class generate envelope folower values from incoming audio.
 The state of envf is updated from controller (pluginProcessor) and modulationData.
 
 ==============================================================================
 */



#include "MFXEnvelopFollower.h"




//==============================================================================
template <typename SampleType>
MFXEnvelopeFollower<SampleType>::MFXEnvelopeFollower()
{
    update();
}

//==============================================================================
template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::setThreshold (SampleType newThreshold)
{
    thresholddB = newThreshold;
    update();
}

template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::setRatio (SampleType newRatio, const float ratioModulation)
{
    jassert (newRatio >= static_cast<SampleType> (1.0));
    
    ratio = newRatio + (( 5.0f - newRatio) * jmin(std::fabs(ratioModulation), 1.0f ));  // not sure this is right - check this! at fast lfo rate !!artefacts
    
    ratio = std::fmin(ratio, 5.0f);
    //thresholddB = ratio < 4 ? -45 : -60; experiment 
    update();
}

template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::setAttack (SampleType newAttack)
{
    attackTime = newAttack;
    update();
}

template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::setRelease (SampleType newRelease)
{
    releaseTime = newRelease;
    update();
}

//==============================================================================
template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);
    
    sampleRate = spec.sampleRate;
    
    envelopeFilter.prepare (spec);
    
    update();
    reset();
}

template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::reset()
{
    envelopeFilter.reset();
}

//==============================================================================
template <typename SampleType>
SampleType MFXEnvelopeFollower<SampleType>::processSample (int channel, SampleType inputValue) // , added SampleType outputValue (wetDelayBuffer)
{
    // Ballistics filter with peak rectifier
    auto env = envelopeFilter.processSample (channel, inputValue); // Keep inputValue (is incoming audio buffer before processed)
    
    // VCA
    auto gain = (env < threshold) ? static_cast<SampleType> (1.0) // At 0 hard cliping happen, sometimes interesting
    : std::pow (env * thresholdInverse, ratioInverse - static_cast<SampleType> (1.0));
    
    
    // Output gain is used to modulate fx params. 1-gain is to reverse param curve - do not want to start at 1
    return 1 - gain;
}

template <typename SampleType>
void MFXEnvelopeFollower<SampleType>::update()
{
    threshold = Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
    thresholdInverse = static_cast<SampleType> (1.0) / threshold;
    ratioInverse     = static_cast<SampleType> (1.0) / ratio;
    
    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}

//==============================================================================
template class MFXEnvelopeFollower<float>;
template class MFXEnvelopeFollower<double>;
