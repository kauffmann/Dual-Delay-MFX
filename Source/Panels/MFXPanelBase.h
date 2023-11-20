/*
  ==============================================================================

    MFXPanelBase.h
    
    Author:  Michael Kauffmann


    A stupid class - clearly a over-engineering thing here!
    Only value is component inherit and mProcessor that all UI classes use. TODO: refactor this out of project.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MFXInterfaceDefine.h"


class MFXPanelBase : public Component
{

public:
    MFXPanelBase (PluginProcessor* inProcessor);
    ~MFXPanelBase();
    
    void mouseEnter (const MouseEvent& event) override;
    
    void mouseExit (const MouseEvent& event) override;
    
    void paint (Graphics& g) override; 
    
    
    
    
protected:
    
    PluginProcessor* mProcessor;

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXPanelBase);






};
