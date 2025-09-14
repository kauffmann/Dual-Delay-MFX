/*
  ==============================================================================

    MFXStereoWidth.h
   
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class MFXStereo
{
    
public:
    
    MFXStereo();
    ~MFXStereo();
    
    //void prepare (const juce::dsp::ProcessSpec& spec);
    
    
    void processWidth (float& leftCh, float& rightCh);
    
    void processMS(float& leftCh, float& rightCh);
    
    
    void setWidth(const float& width, const float& widthModulator = 0.0f) noexcept
    {
        mWidth = widthModulator > 0 ? width + ((1 - width) * widthModulator) : width + (width * widthModulator);
    }
    
    


private:
    //==============================================================================
    float mWidth{0.0f};
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXStereo);
};
