/*
  ==============================================================================

    MFXPanelBase.h
    
    Author:  Michael Kauffmann


    A stupid class. 
    Originally the idea was about encapsulating/generalization a base class that all UI classes use. 
    But UI classes already have a ProcessorProcessor, they simply pass it to Panelbase. 
    No reason having this class since it is not doing anything, except store a PluginPocessor and component inherit. 
    TODO: Consider refactor this out of project.
    

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
