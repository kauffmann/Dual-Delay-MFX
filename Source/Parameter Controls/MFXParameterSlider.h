/*
  ==============================================================================

    MFXParameterSlider.h
   
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "MFXHelperFunctions.h"
#include "MFXUsedParameters.h"
#include "PluginProcessor.h"



// this is experimental stuff, about problem in logic pro and garageband, mouse not invisable when click and drag
//#include <ApplicationServices/ApplicationServices.h>

//#include <CoreGraphics/CoreGraphics.h>
//#include <CoreFoundation/CoreFoundation.h>

class MFXParameterSlider : public Slider, public Slider::Listener, public Timer
{

public:
    
    
    MFXParameterSlider( AudioProcessorValueTreeState& state,
                        const String& parameterID,
                        const String& parameterLabel,
                        MFXParameter valueTextIndex,
                        PluginProcessor* inProcessor);
    ~MFXParameterSlider();
    
    
    void mouseDown(const MouseEvent& event) override;
    
    void mouseUp(const MouseEvent& event) override;
    
    void mouseExit(const MouseEvent& event) override;
    
    void timerCallback() override;
    
    
    
    //void paint (Graphics& g) override;
    
    void sliderValueChanged (Slider*) override;
    
    inline MFXParameter getParameterValueText()
    {
        
        return mParameterValueText; 
    }
    
private:
    juce::Point<int> mInitialDownMousePosition;
    MFXParameter mParameterValueText;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    PluginProcessor& mProcessor;
    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXParameterSlider);
    
};
