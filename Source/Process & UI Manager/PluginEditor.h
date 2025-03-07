/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
    
    Seperation of concern: initialize and host child components (set global
    lookAndFell, initialize child components with a processer, other global GUI related).
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MFXTopPanel.h"
#include "MFXGainPanel.h"
#include "MFXCenterPanelMenuBar.h"
#include "MFXFxPanel.h"
#include "MFXInterfaceDefine.h"
#include "MFXLookAndFeel.h"
#include "MFXUsedParameters.h"


class PluginEditor : public Component
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& audioProcessor;
    
    std::unique_ptr<MFXTopPanel> mTopPanel;
    std::unique_ptr<MFXGainPanel> mInputGainPanel;
    std::unique_ptr<MFXGainPanel> mOutputGainPanel;
    std::unique_ptr<MFXCenterPanelMenuBar> mMenuBar;
    std::unique_ptr<MFXFxPanel> mFxPanel;
    
    
    std::unique_ptr<MFXLookAndFeel> mLookAndFeel;
    
    Image mBackgroundImage;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

class WrappedRasterAudioProcessorEditor : public AudioProcessorEditor
{
public:
    WrappedRasterAudioProcessorEditor(PluginProcessor&);
    void resized() override;

private:
    static constexpr int originalWidth{ 750}; // 1200  // 750
    static constexpr int originalHeight{ 310}; // 800   //310

    PluginEditor rasterComponent;
    //ApplicationProperties applicationProperties;  // not using, but good to know about
    PluginProcessor& mProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WrappedRasterAudioProcessorEditor)
};