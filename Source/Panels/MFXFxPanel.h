/*
  ==============================================================================

    MFXFxPanel.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include "MFXPanelBase.h"
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"
#include "MFXHelperFunctions.h"
#include "MFXParameterSlider.h"
#include "MFXParameterComBox.h"
#include "MFXParameterButton.h"
//#include <juce_gui_extra/juce_gui_extra.h> if not in library - to use JUCE_LIVE_CONSTANT 

enum FxPanelStyle
{
    mFxPanelStyle_Delay1 = 0,
    mFxPanelStyle_Delay2,
    mFxPanelStyle_Chorus,
    mFxPanelStyle_Phaser,
    mFxPanelStyle_BitCrusher,
    mFxPanelStyle_Reverb,
    mFxPanelStyle_Filter,
    mFxPanelStyle_Advanced,
    mFxPanelStyle_MIDI_ADSR,
    mFxPanelStyle_TotalNumStyles
};


class MFXFxPanel : public MFXPanelBase, public ComboBox::Listener, public Timer, public ImageButton::Listener
{
    
public:
    MFXFxPanel(PluginProcessor* inProcessor);
    
    ~MFXFxPanel();
    
    void paint(Graphics& g) override;
    
    void setFxPanelStyle(FxPanelStyle inStyle);
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void timerCallback() override;
    
    void buttonClicked (Button* currentButton) override;
    
    void createDuckingUI();
    
    void createMixUI();
    
    void createFXLevelsUI();
    
    void createCutModeUI();
    
private:
    FxPanelStyle mStyle;
    
    // OW + all pointers to object gets deleted when owner (mSliders/this) is destroyed.
    OwnedArray<MFXParameterSlider> mSliders;
    OwnedArray<MFXParameterSlider> mGlobalSliders;
    OwnedArray<MFXParameterSlider> mFXPanelHorisontalSliders;
    OwnedArray<MFXParameterButton> mButtons;
    OwnedArray<MFXParameterButton> mGlobalButtons;
    
    OwnedArray<MFXParameterComboBox> mComboBoxes;

    std::unique_ptr<MFXParameterComboBox> mFilterType;
    std::unique_ptr<MFXParameterComboBox> mDelayTimeModeLeft;
    std::unique_ptr<MFXParameterComboBox> mDelayTimeModeRight;
    std::unique_ptr<MFXParameterComboBox> mDelayTimeModeMain1;
    std::unique_ptr<MFXParameterComboBox> mDelayTimeModeMain2;
    
    std::unique_ptr<Label> mFilterTypeLabel;

    std::unique_ptr<ArrowButton> mDelay1TimeTransitionMode, mDelay2TimeTransitionMode;
    std::unique_ptr<ImageButton> mSetRandomValues;
    std::unique_ptr<ImageButton> mSetRandomAllValues;
    std::unique_ptr<ImageButton> mMidi;

    juce::GroupComponent windowBorder; // unused
    juce::GroupComponent windowBorderLFO; // unused
  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXFxPanel);
      
};
