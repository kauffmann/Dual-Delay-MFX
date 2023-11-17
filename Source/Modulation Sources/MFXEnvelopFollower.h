



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
 */
#pragma once

#include <JuceHeader.h>

/* 
 ==============================================================================
 
 MFXEnvelopeFollower.h
 
 Author: Customized to project by Michael Kauffmann
 A envelope follower created using JUCE compressor class. Class generate envelope folower values from incoming audio.
 The state of envf is updated from controller (pluginProcessor) and modulationData.
 The generated data are stored and managed by modulationData.
 
 ==============================================================================
*/

template <typename SampleType>
class MFXEnvelopeFollower
{
public:
    //==============================================================================
    /** Constructor. */
    MFXEnvelopeFollower();
    
    
    //==============================================================================
    /** Sets the threshold in dB of the compressor.*/
    void setThreshold (SampleType newThreshold);
    
    /** Sets the ratio of the compressor (must be higher or equal to 1).*/
    void setRatio (SampleType newRatio, const float ratioModulation = 0.0f);
    
    /** Sets the attack time in milliseconds of the compressor.*/
    void setAttack (SampleType newAttack);
    
    /** Sets the release time in milliseconds of the compressor.*/
    void setRelease (SampleType newRelease);
    
    //==============================================================================
    /** Initialises the processor. */
    void prepare (const juce::dsp::ProcessSpec& spec);
    
    /** Resets the internal state variables of the processor. */
    void reset();
    
    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context, const ProcessContext& contextWet) noexcept  // added , const ProcessContext& contextWet
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock      = contextWet.getOutputBlock(); // added contexWet, instead of contex
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples  = outputBlock.getNumSamples();
        
        jassert (inputBlock.getNumChannels() == numChannels);
        jassert (inputBlock.getNumSamples()  == numSamples);
        
        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples  = inputBlock .getChannelPointer (channel);
            auto* outputSamples = outputBlock.getChannelPointer (channel);
            
            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = processSample ((int) channel, inputSamples[i], outputSamples[i]); // ,added outputSamples[] (is wetDelayBuffer)
                }
    }
    
    /** Performs the processing operation on a single sample at a time. */
    SampleType processSample (int channel, SampleType inputValue);  
    
private:
    //==============================================================================
    void update();
    
    //==============================================================================
    SampleType threshold, thresholdInverse, ratioInverse;
    juce::dsp::BallisticsFilter<SampleType> envelopeFilter;
    
    double sampleRate = 44100.0;
    //SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
    // ratio = 1 is no effect, 2  starts 0.07, 5 starts 0.01   value goes from 0-1  (if o below 1)
    SampleType thresholddB = -60.0, ratio = 1.0, attackTime = 10.0, releaseTime = 4000.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXEnvelopeFollower);
};
