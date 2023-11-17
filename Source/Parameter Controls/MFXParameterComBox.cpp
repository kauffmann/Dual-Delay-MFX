/*
  ==============================================================================

    MFXParameterComBox.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXParameterComBox.h"



MFXParameterComboBox::MFXParameterComboBox( juce::AudioProcessorValueTreeState& stateControl, const juce::String& parameterID, PluginProcessor* inProcessor)
: juce::ComboBox(parameterID), mProcessor(*inProcessor)
{
    mAttachment = std::make_unique<juce::AudioProcessorValueTreeState::
                  ComboBoxAttachment>(stateControl, parameterID, *this);
    addListener(this);
    
    startTimer(30);
}

MFXParameterComboBox::~MFXParameterComboBox() { removeListener(this); }

void MFXParameterComboBox::timerCallback()
{
    if (isPopupActive())
        mProcessor.getAccessToUIPersistentData().mIsComboBoxTouchedByUser = true;
        
}

    
void MFXParameterComboBox::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    
    //isComboBoxTouchedByUser = true;
    
}

