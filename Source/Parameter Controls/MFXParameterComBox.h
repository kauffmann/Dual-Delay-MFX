/*
  ==============================================================================

    MFXParameterComBox.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"
#include "PluginProcessor.h"



class MFXParameterComboBox : public juce::ComboBox, public ComboBox::Listener, public Timer
{
    
public:
    
    
    MFXParameterComboBox( juce::AudioProcessorValueTreeState& stateControl, const juce::String& parameterID, PluginProcessor* inProcessor);
    ~MFXParameterComboBox();
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void timerCallback() override;
    
    
    
    
private:
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
    
    PluginProcessor& mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXParameterComboBox);
    
};
