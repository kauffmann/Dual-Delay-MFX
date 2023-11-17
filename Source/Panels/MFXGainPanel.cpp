/*
  ==============================================================================

    MFXGainPanel.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXGainPanel.h"


MFXGainPanel::MFXGainPanel(PluginProcessor* inProcessor, LevelMeters meterIOType)
: MFXPanelBase(inProcessor)
{
   
    if(meterIOType == mLevelMeters_In)
    {
        verticalGradientMeterL = std::make_unique<VerticalGradientMeter>([&]() { return mProcessor->getRmsLevelIn(0); });
        verticalGradientMeterR = std::make_unique<VerticalGradientMeter>([&]() { return mProcessor->getRmsLevelIn(1); });
    
        addAndMakeVisible(verticalGradientMeterL.get());
        addAndMakeVisible(verticalGradientMeterR.get());
    }
    else if (meterIOType == mLevelMeters_Out)
    {
        verticalGradientMeterL = std::make_unique<VerticalGradientMeter>([&]() { return mProcessor->getRmsLevelOut(0); });
        verticalGradientMeterR = std::make_unique<VerticalGradientMeter>([&]() { return mProcessor->getRmsLevelOut(1); });
        
        addAndMakeVisible(verticalGradientMeterL.get());
        addAndMakeVisible(verticalGradientMeterR.get());
    }
    
    
    
}

MFXGainPanel::~MFXGainPanel(){}


void MFXGainPanel::paint(Graphics& g)
{
    MFXPanelBase::paint(g);
    
    if(mSlider)
    {
        if (mSlider->isMouseOverOrDragging())
        {
            //paintComponentLabel(g, mSlider.get(), mProcessor->getAccessToUIPersistentData().mParameterValueText[mSlider->getParameterValueText()]);
            paintComponentLabel(g, mSlider.get(), mProcessor->parameters.getParameter(MFXParameterID[mSlider->getParameterValueText()])->getCurrentValueAsText());
        }
        else
        {
            paintComponentLabel(g, mSlider.get(), mSlider->getName());
        }
        
        
    }
    
    
    repaint();

}

void MFXGainPanel::resized()
{
    const int slider_size = 54;
    
    
    
    mSlider->setBounds((getWidth() * 0.5) - (slider_size * 0.5),
                       (getHeight() * 0.35) - (slider_size * 0.5),
                       slider_size, slider_size);
    
    const int meter_width = 12.0;
    verticalGradientMeterL->setBounds((getWidth() * 0.45) - (meter_width * 0.45),
                           ((getHeight()-10) * 0.55) - (meter_width * 0.5),
                           meter_width,
                           (getHeight()-10) * 0.45);
    
    verticalGradientMeterR->setBounds((getWidth() * 0.55) - (meter_width * 0.55),
                                      ((getHeight()-10) * 0.55) - (meter_width * 0.5),
                                      meter_width,
                                      (getHeight()-10) * 0.45);
    
}

// Seems stupid having 3 params all representing same index. also name of function. TODO:: refactor this!!!!!!!!!!!!!!!!!!!!!!!!!!!
void MFXGainPanel::setParameterID(int inParameterID, int inParameterLabel, int inParameterValueText )
{
    mSlider = std::make_unique<MFXParameterSlider>(mProcessor->parameters, MFXParameterID[inParameterID],
                                                   MFXParameterLabel[inParameterLabel],
                                                   static_cast<MFXParameter>(inParameterValueText), mProcessor);
    
    addAndMakeVisible(mSlider.get());
    
}
