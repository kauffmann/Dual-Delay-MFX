/*
  ==============================================================================

    MFXCenterPanelMenuBar.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include "MFXPanelBase.h"
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"
#include "MFXParameterComBox.h"
#include "MFXParameterButton.h"





class MFXCenterPanelMenuBar : public MFXPanelBase
{
    
public:
    MFXCenterPanelMenuBar(PluginProcessor* inProcessor);
    ~MFXCenterPanelMenuBar();
    
    
    void addFxTypeComboBoxListener(ComboBox::Listener* inListener);
    
    void removeFxTypeComboListener(ComboBox::Listener* inListener);
    
    void paint (Graphics& g) override;
    
    void resized() override;
    
    void addFxSettingsListener(ToggleButton::Listener* inListener);

    void removeFxSettingsListener(ToggleButton::Listener* inListener);
    
    void createFxSettingButtons();
    
    
    


private:
    std::unique_ptr<MFXParameterComboBox> mFxTypeComboBox;
    
    OwnedArray<MFXParameterButton> mButtons;
    
    
    
    
   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXCenterPanelMenuBar);
    
};
