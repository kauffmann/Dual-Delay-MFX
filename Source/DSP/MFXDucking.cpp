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


/*
  ==============================================================================

    MFXDucking.cpp.
    
    Author:  Overwritten by Michael Kauffmann
    I re-designed class to modulate parameters instead of gain of aaudio signal.


  ==============================================================================
*/



#include "MFXDucking.h"



        
        //==============================================================================
        template <typename SampleType>
        MFXDucking<SampleType>::MFXDucking()
        {
            update();
        }
        
        //==============================================================================
        template <typename SampleType>
        void MFXDucking<SampleType>::setThreshold (SampleType newThreshold)
        {
            thresholddB = newThreshold;
            update();
        }
        
        template <typename SampleType>
        void MFXDucking<SampleType>::setRatio (SampleType newRatio)
        {
            jassert (newRatio >= static_cast<SampleType> (1.0));
            
            ratio = newRatio;
            update();
        }
        
        template <typename SampleType>
        void MFXDucking<SampleType>::setAttack (SampleType newAttack)
        {
            attackTime = newAttack;
            update();
        }
        
        template <typename SampleType>
        void MFXDucking<SampleType>::setRelease (SampleType newRelease)
        {
            releaseTime = newRelease;
            update();
        }
        
        //==============================================================================
        template <typename SampleType>
        void MFXDucking<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
        {
            jassert (spec.sampleRate > 0);
            jassert (spec.numChannels > 0);
            
            sampleRate = spec.sampleRate;
            
            envelopeFilter.prepare (spec);
            
            update();
            reset();
        }
        
        template <typename SampleType>
        void MFXDucking<SampleType>::reset()
        {
            envelopeFilter.reset();
        }
        
        //==============================================================================
        template <typename SampleType>
        SampleType MFXDucking<SampleType>::processSample (int channel, SampleType inputValue, SampleType outputValue) // , added SampleType outputValue (wetDelayBuffer)
        {
            // Ballistics filter with peak rectifier
            auto env = envelopeFilter.processSample (channel, inputValue); // Keep inputValue (is incoming audio buffer before processed)
            
            // VCA
            auto gain = (env < threshold) ? static_cast<SampleType> (1.0)
            : std::pow (env * thresholdInverse, ratioInverse - static_cast<SampleType> (1.0));
            
            
            // Output
            return gain * outputValue;   // added outputValue instead of inputValue
        }
        
        template <typename SampleType>
        void MFXDucking<SampleType>::update()
        {
            threshold = Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
            thresholdInverse = static_cast<SampleType> (1.0) / threshold;
            ratioInverse     = static_cast<SampleType> (1.0) / ratio;
            
            envelopeFilter.setAttackTime (attackTime);
            envelopeFilter.setReleaseTime (releaseTime);
        }
        
        //==============================================================================
        template class MFXDucking<float>;
        template class MFXDucking<double>;
        
    
