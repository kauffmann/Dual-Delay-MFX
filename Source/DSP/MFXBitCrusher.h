/*
  ==============================================================================

    MFXBitCrusher.h
   
    Author: Based off Juce tutorial 39 with special guest Aaron Leese- https://youtu.be/1PLn8IAKEb4
    Encapsulated into seperate class, added more functionalty and setting up parameter (AudioProcessorValueTreeState) and UI by Michael Kauffmann

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "MFXAudioHelpers.h"
#include "MFXUsedParameters.h"




class MFXBitCrusher
{
    
    
    
public:
    //==============================================================================
    MFXBitCrusher();
    ~MFXBitCrusher();
    
    //==============================================================================
    void prepareToPlay (const juce::dsp::ProcessSpec& spec);
    
    void reset();
    
    void process (AudioBuffer<float>& inBuffer);
                       
    // Consider put all below in helper class or a struct
    static Array<float> getSimpleNoise(int numSamples)
    {
        Random r = Random::getSystemRandom();
        Array<float> noise;
        
        for (int s=0; s < numSamples; s++)
        {
            noise.add((r.nextFloat() - .5)*2);
        }
        
        return noise;   
    }
    
    
    
    static Array<float> getWhiteNoise(int numSamples)
    {
        
        Array<float> noise;
        
        float z0 = 0;
        float z1 = 0;
        bool generate = false;
        
        float mu = 0; // center (0)
        float sigma = 1; // spread -1 <-> 1
        
        float output = 0;
        float u1 = 0;
        float u2 = 0;
        
        Random r = Random::getSystemRandom();
        r.setSeed(Time::getCurrentTime().getMilliseconds());
        const float epsilon = std::numeric_limits<float>::min();
        
        for (int s=0; s < numSamples; s++)
        {
            
            // GENERATE ::::
            // using box muller method
            // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
            generate = !generate;
            
            if (!generate)
                output =  z1 * sigma + mu;
            else
            {
                do
                {
                    u1 = r.nextFloat();
                    u2 = r.nextFloat();
                }
                while ( u1 <= epsilon );
                
                z0 = sqrtf(-2.0 * logf(u1)) * cosf(2*float(double_Pi) * u2);
                z1 = sqrtf(-2.0 * logf(u1)) * sinf(2*float(double_Pi) * u2);
                
                output = z0 * sigma + mu;
            }
            
            // NAN check ...
            jassert(output == output);
            jassert(output > -50 && output < 50);
            
            //
            noise.add(output);
            
        }
        
        return noise;
        
    }
    
    
    
    
    void setNoise(const float& noise) noexcept;
    
    void setDownsample(const float& downsample) noexcept;
    
    void setResolution(const float& resolution) noexcept;
    
    void setDryWetMix(const float& dryWetMix) noexcept;
    
    
private:
    
    
    AudioBuffer<float> noiseBuffer, currentOutputBuffer;
    
    
    float mFeedback[2] {0.0f, 0.0f};
    float mNoise{0.0f}, mDownsample{1.0f}, mResolution{16.0f}, mDryWetMix{0.0f};
    
    double mSampleRate {0.0};
    
    
    

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXBitCrusher)
};
