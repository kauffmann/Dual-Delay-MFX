/*
  ==============================================================================

    MFXCreditPanel.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXInterfaceDefine.h"


// launch in toppanel when user select credit from optional menu

class MFXCreditPanel : public Component
{
    
public:
    MFXCreditPanel();
    ~MFXCreditPanel();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void mouseDown(const MouseEvent& event) override;
    
    
    
    
private:
    
    Image mCreditBackGround;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXCreditPanel);
    
    
};
