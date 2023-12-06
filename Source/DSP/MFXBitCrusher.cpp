/*
  ==============================================================================

    MFXBitCrusher.cpp
   
    Author: Basic Code by Aaron Leese (Stagecraft Software). Based off Juce tutorial 39 with special guest Aaron Leese- https://youtu.be/1PLn8IAKEb4
    Encapsulated into seperate class, by Michael Kauffmann.

  ==============================================================================
*/

#include "MFXBitCrusher.h"




    
MFXBitCrusher::MFXBitCrusher(){}


MFXBitCrusher::~MFXBitCrusher(){}



//==============================================================================
void MFXBitCrusher::prepareToPlay (const juce::dsp::ProcessSpec& spec)
{
    mSampleRate = spec.sampleRate;
}

void MFXBitCrusher::reset()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void MFXBitCrusher::process (AudioBuffer<float>& inBuffer)
{   
    int numSamples = inBuffer.getNumSamples();
    
    float noiseAmt = -120 + 120*(mNoise /100); // dB
    noiseAmt = jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = Decibels::decibelsToGain(noiseAmt);
    
    
    
    // SAFETY CHECK :::: since some hosts will change buffer sizes without calling prepToPlay (Bitwig)
    if (noiseBuffer.getNumSamples() != numSamples)
    {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clears
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears   use inBuffer instead, and no setSize.......
    }
    
    
    // COPY for processing ...
    currentOutputBuffer.copyFrom(0, 0, inBuffer.getReadPointer(0), numSamples);
    
    if (inBuffer.getNumChannels() > 1)
        currentOutputBuffer.copyFrom(1, 0, inBuffer.getReadPointer(1), numSamples);
    
    // BUILD NOISE ::::::
    {
        noiseBuffer.clear();
        
        Array<float> noise = getWhiteNoise(numSamples);
        
        // range bound
        noiseAmt = jlimit<float>(0, 1, noiseAmt);
        
        FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);
        
        // ADD the noise ...
        FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
        FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples); // STEREO
        
    }
    
    
    // ADD NOISE to the incoming AUDIO ::::
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
    currentOutputBuffer.addFrom(1, 0, noiseBuffer.getReadPointer(1), numSamples);
    
    
    
    for (int chan=0; chan < currentOutputBuffer.getNumChannels(); chan++)
    {
        float* data = currentOutputBuffer.getWritePointer(chan);
        
        for (int i=0; i < numSamples; i++)
        {
            float bitDepth = mResolution;
            const int& rateDivide = mDownsample;
            
            float totalQLevels = powf(2, bitDepth); // power of 2( 0|1 ), how many values to represent audio. 2 bits == 4 values....
            
            int val = roundToInt(data[i] * totalQLevels);
            data[i] = val / totalQLevels;
            
            // this is samplerate
            if (rateDivide > 1)
            {
                if (i%rateDivide != 0) data[i] = data[i - i%rateDivide];
            }
            
        }
    }
    
    // Mix
    FloatVectorOperations::multiply(currentOutputBuffer.getWritePointer(0), mDryWetMix, currentOutputBuffer.getNumSamples());
    FloatVectorOperations::multiply(currentOutputBuffer.getWritePointer(1), mDryWetMix, currentOutputBuffer.getNumSamples());
    
    FloatVectorOperations::multiply(inBuffer.getWritePointer(0), 1 - mDryWetMix, inBuffer.getNumSamples());
    FloatVectorOperations::multiply(inBuffer.getWritePointer(1), 1 - mDryWetMix, inBuffer.getNumSamples());
    
    for (int chan=0; chan < currentOutputBuffer.getNumChannels(); chan++)
    {
        inBuffer.addFrom(chan, 0, currentOutputBuffer, chan, 0, currentOutputBuffer.getNumSamples());
    }
}




void MFXBitCrusher::setNoise(const float& noise) noexcept
{
    mNoise = noise;
}

void MFXBitCrusher::setDownsample(const float& downsample) noexcept
{
    mDownsample = downsample;
}

void MFXBitCrusher::setResolution(const float& resolution) noexcept
{
    mResolution = resolution;
}

void MFXBitCrusher::setDryWetMix(const float& dryWetMix) noexcept
{
    mDryWetMix = dryWetMix;
}


