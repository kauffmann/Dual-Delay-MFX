/*
  ==============================================================================

    MFXSimpleFilter.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXSimpleFilter.h"

MFXSimpleFilter::MFXSimpleFilter(){}

MFXSimpleFilter::~MFXSimpleFilter(){}


void MFXSimpleFilter::prepare (const juce::dsp::ProcessSpec& spec)
{
    mSampleRate = spec.sampleRate;
    
    mFilter.prepare (spec);
}

void MFXSimpleFilter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block { buffer};
    mFilter.process (juce::dsp::ProcessContextReplacing<float> (block));
    
}

void MFXSimpleFilter::reset()
{
    mFilter.reset();
}

void MFXSimpleFilter::updateParameters(const float& cutoff, const FilterType type)
{
    if (mSampleRate != 0.0)
    {
        
        //auto qVal   = static_cast<float> (qParam);
        
        switch (type)
        {
            case 1:  *mFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass  (mSampleRate, cutoff);
                break;
            case 2:  *mFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (mSampleRate, cutoff);
                break;
            default:
                     *mFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass  (mSampleRate, cutoff);
                break;
        }
    }
}
