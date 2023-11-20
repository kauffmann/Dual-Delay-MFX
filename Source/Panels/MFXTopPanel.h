/*
  ==============================================================================

    MFXTopPanel.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include "MFXPanelBase.h"
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"
#include "MFXParameterComBox.h"
#include "MFXLookAndFeel.h"
#include "MFXCreditPanel.h"
#include "MFXUnlockPanel.h"



class MFXTopPanel : public MFXPanelBase, public ComboBox::Listener, public ActionListener, public Timer, public Button::Listener
{
    
public:
    MFXTopPanel(PluginProcessor* inProcessor);
    ~MFXTopPanel();
    
    void paint (Graphics& g) override;
    
    void resized () override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void timerCallback() override;
    
    void actionListenerCallback (const String &message) override;

    void buttonClicked(Button* button) override;
    
private:
    
    void displaySaveAsPopup();
    
    void updatePresetComboBox();
    
    std::unique_ptr<ComboBox> mPresetDisplay;  
    std::unique_ptr<ComboBox> mOptionalDisplay;
    
    std::unique_ptr<TextButton> mNameOfPlugin;
    std::unique_ptr<ArrowButton> mForward, mBackWard;
    
    std::unique_ptr<FileChooser> mChooser;
    
    int mCurrentIndex {0};
    int mPreviousIndex {0};
    
    std::unique_ptr<MFXCreditPanel> mCreditPanel;

    std::unique_ptr<MFXUnlockPanel> mUnlockPanel;

    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXTopPanel);
    
    
    
};
