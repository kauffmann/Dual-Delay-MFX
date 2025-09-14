/*
  ==============================================================================

    MFXParameterButton.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXUsedParameters.h"
#include "PluginProcessor.h"


class MFXParameterButton : public ImageButton, public Timer
{
    
public:
    
    
    MFXParameterButton( AudioProcessorValueTreeState& state,
                       const String& parameterID,
                       const String& parameterLabel, const Image& imageNormal, const Image& imageDown,
                       PluginProcessor* inProcessor,
                       const MFXParameter& valueTextIndex = mFXParameter_TotalNumParameters);
    
    // add MFXParameter valueTextIndex, inline getMFXParameter valueTextIndex, then like slider exit and timer
    MFXParameterButton( AudioProcessorValueTreeState& state,
                       const String& parameterID,
                       const String& parameterLabel,const Image& imageNormal, const Image& imageDown,
                       std::function<String()>&& valueFunction, PluginProcessor* inProcessor);

    ~MFXParameterButton();
    
   
    
    void mouseExit(const MouseEvent& event) override;
    
    void timerCallback() override;
    
    MFXParameter getParameterValueText()
    {
        return mParameterValueText;
    }
    
private:
    std::function<String()> valueSupplier;
    MFXParameter mParameterValueText;

    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mAttachment;
    
    PluginProcessor& mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXParameterButton);
    
};
