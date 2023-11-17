/*
  ==============================================================================

    MFXParameterSlider.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXParameterSlider.h"
//#include <ApplicationServices/ApplicationServices.h>   // this is experimental stuff, about problem in logic pro and garageband, mouse not invisable when click and drag



MFXParameterSlider::MFXParameterSlider( AudioProcessorValueTreeState& state,
                                        const String& parameterID,
                                        const String& parameterLabel,
                                        MFXParameter valueTextIndex,
                                        PluginProcessor* inProcessor)
: juce::Slider(parameterLabel), mParameterValueText(valueTextIndex), mProcessor(*inProcessor)
{
    setSliderStyle(SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    
    mAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(state,parameterID, *this);
    
    addListener(this);
    
    
    
    
    startTimer(30);
}

MFXParameterSlider::~MFXParameterSlider()
{
    removeListener(this);
}

void MFXParameterSlider::mouseDown(const MouseEvent& event)
{
        
    
    if (this->hitTest(event.getMouseDownX(), event.getMouseDownY()))       // not needed
    {
        Slider::mouseDown(event);
        setMouseCursor(MouseCursor::NoCursor);
        mInitialDownMousePosition = Desktop::Desktop::getMousePosition();

        //updateMouseCursor();
    }
    
    mProcessor.getAccessToUIPersistentData().mIsSliderTouchedByUser = true;
}

void MFXParameterSlider::mouseUp(const MouseEvent& event)
{
    Slider::mouseUp(event);
    Slider::setMouseCursor(MouseCursor::NormalCursor);
    //CGDisplayShowCursor(kCGDirectMainDisplay);
    Desktop::setMousePosition(mInitialDownMousePosition);
    //updateMouseCursor();
}

void MFXParameterSlider::mouseExit(const MouseEvent& event)
{
      mProcessor.getAccessToUIPersistentData().mHelperUIText = "";
}

void MFXParameterSlider::timerCallback()
{
    if (this->isMouseOver())
        mProcessor.getAccessToUIPersistentData().mHelperUIText = MFXParameterHelpText[getParameterValueText()];

}


void MFXParameterSlider::sliderValueChanged (Slider*)
{
    
    
    //isSliderTouchedByUser = true;
}





//void MFXParameterSlider::paint (Graphics& g)
//{
//     if(isMouseOverOrDragging())
//     {
//         paintComponentLabel(g, this, mParameterValueText);
//         
//     }
//    
//     else
//     {
//         paintComponentLabel(g, this, this->getName());
//     }
//}





