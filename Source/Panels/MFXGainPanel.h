/*
  ==============================================================================

    MFXGainPanel.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once


#include "MFXPanelBase.h"
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"
#include "MFXHelperFunctions.h"
#include "MFXParameterSlider.h"
#include "VerticalGradientMeter.h"


// global accessible enum
enum LevelMeters
{
    mLevelMeters_In = 0,
    mLevelMeters_Out,
    mLevelMeters_TotalNumMeters
};

class MFXGainPanel : public MFXPanelBase
{
    
public:
    MFXGainPanel(PluginProcessor* inProcessor, LevelMeters meterIOType);
    ~MFXGainPanel();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void setParameterID(int inParameterID, int inParameterLabel, int inParameterValueText);
    
    
    
private:
    
    std::unique_ptr<MFXParameterSlider> mSlider;
    std::unique_ptr<VerticalGradientMeter> verticalGradientMeterL, verticalGradientMeterR;
    
    
    Image mBackgroundImage;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXGainPanel);
    
    
};
