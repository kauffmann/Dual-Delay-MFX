/*
  ==============================================================================

    MFXSimpleFilter.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum FilterType
{
    mFilterType_LowPass = 1,
    mFilterType_HighPass,
    mFilterType_TotalNum
};

class MFXSimpleFilter
{
    
public:
    
    MFXSimpleFilter();
    ~MFXSimpleFilter();
    
    void prepare (const juce::dsp::ProcessSpec& spec);
    
    
    void process (juce::AudioBuffer<float>& buffer);

    
    void reset();
    
    void updateParameters(const float& cutoff, const FilterType type);
    

private:
    //==============================================================================
    double mSampleRate;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> mFilter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXSimpleFilter);
};
