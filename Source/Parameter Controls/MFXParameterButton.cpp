/*
  ==============================================================================

    MFXParameterButton.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXParameterButton.h"

MFXParameterButton::MFXParameterButton (AudioProcessorValueTreeState& state,
                                        const String& parameterID,
                                        const String& parameterLabel,
                                        const Image& imageNormal, const Image& imageDown,
                                        PluginProcessor* inProcessor,
                                        const MFXParameter& valueTextIndex)
: juce::ImageButton(parameterLabel), mParameterValueText(valueTextIndex), mProcessor(*inProcessor)
{
    mAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(state,parameterID, *this);
    
    

    juce::ImageButton::setImages(false, false, true, imageNormal, 1.0f,
                                 Colour(0x0), imageNormal,1.0f,
                                 Colour(0x0), imageDown, 1.0f,
                                 Colour(0x0));
    
    juce::ImageButton::setClickingTogglesState(true);
    
    
    
    startTimer(30);
   
}


MFXParameterButton::MFXParameterButton (AudioProcessorValueTreeState& state,
                                        const String& parameterID,
                                        const String& parameterLabel,
                                        const Image& imageNormal, const Image& imageDown,
                                        std::function<String()>&& valueFunction, PluginProcessor* inProcessor)
: juce::ImageButton(parameterLabel), valueSupplier(std::move(valueFunction)), mProcessor(*inProcessor)
{
    mAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(state,parameterID, *this);
    
    
    juce::ImageButton::setImages(false, false, true, imageNormal, 1.0f,
                                 Colour(0x0), imageNormal,1.0f,
                                 Colour(0x0), imageDown, 1.0f,
                                 Colour(0x0));
    
    juce::ImageButton::setClickingTogglesState(true);
    
    startTimer(30);
}




MFXParameterButton::~MFXParameterButton(){}




void MFXParameterButton::mouseExit(const MouseEvent& event)
{
    mProcessor.getAccessToUIPersistentData().mHelperUIText = "";
}



void MFXParameterButton::timerCallback()
{
    if (this->isMouseOver() && (mParameterValueText != mFXParameter_TotalNumParameters))
        mProcessor.getAccessToUIPersistentData().mHelperUIText = MFXParameterHelpText[getParameterValueText()];

    // This replace mouseDown(const MouseEvent& event) - that somehow conflict on windows so that mouse click on button was unstable (press hard mousebutton)
    if (this->isMouseButtonDown())
        mProcessor.getAccessToUIPersistentData().mIsButtonTouchedByUser = true;
}


