/*
  ==============================================================================

    MFXUnlockPanel.h
    
    Author:  Michael kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXInterfaceDefine.h"
#include "MFXPanelBase.h"


// launch from toppanel

class MFXUnlockPanel : public MFXPanelBase
{

public:
    MFXUnlockPanel(PluginProcessor* inProcessor);
    ~MFXUnlockPanel();

    void paint(Graphics& g) override;

    void resized() override;

    void addListenerToButtons(Button::Listener* listener);

    void removeListenerToButtons(Button::Listener* listener);


private:

    Image mUnlockBackGround;
    std::unique_ptr <TextEditor> mInputKeyField;
    Image mUnlockButtonImage;
    Image mDemoButtonImage;
    std::unique_ptr<ImageButton> mUnlock;
    std::unique_ptr<ImageButton> mDemo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXUnlockPanel);


};

