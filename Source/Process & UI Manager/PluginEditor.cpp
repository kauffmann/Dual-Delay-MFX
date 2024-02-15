/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
 
    Manage (initialize, size, lookAndFeel) parent(this) and child components.
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "PluginProcessor.h"  
#include "PluginEditor.h"

// test allowed here

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : audioProcessor(p)
{    
    mLookAndFeel = std::make_unique<MFXLookAndFeel>(); // good, no need to delete in destructor.
    // Will apply lookAndFell behaviour to "this" and all object(child components) that exist inside this parent/main component it. All mainPanels component is therefor affected.
    setLookAndFeel(mLookAndFeel.get());   
    //LookAndFeel::setDefaultLookAndFeel(mLookAndFeel.get()); // if addressed anywhere in app/this class, this is the default. not a requirement.
    
    
    // Call chain: call this. constructor starts, child component constructer gets called, then this.resized(),
    // then child component resized(), then this. constructor ends,then this. paint(), then child component paint. 
    mTopPanel = std::make_unique<MFXTopPanel>(&audioProcessor);
    addAndMakeVisible(mTopPanel.get());
    
    mInputGainPanel = std::make_unique<MFXGainPanel>(&audioProcessor, mLevelMeters_In);
    mInputGainPanel->setParameterID(mFXParameter_InputGain, mFXParameter_InputGain, mFXParameter_InputGain); // hmmmmmmmm x3 the same
    addAndMakeVisible(mInputGainPanel.get());
    
    
    mOutputGainPanel = std::make_unique<MFXGainPanel>(&audioProcessor, mLevelMeters_Out);
    mOutputGainPanel->setParameterID(mFXParameter_OutputGain, mFXParameter_OutputGain, mFXParameter_OutputGain);
    addAndMakeVisible(mOutputGainPanel.get());
    
    
    mMenuBar = std::make_unique<MFXCenterPanelMenuBar>(&audioProcessor);
    addAndMakeVisible(mMenuBar.get());
    
    mFxPanel = std::make_unique<MFXFxPanel>(&audioProcessor);
    addAndMakeVisible(mFxPanel.get()); 
    
    mMenuBar->addFxTypeComboBoxListener(mFxPanel.get());
    mMenuBar->addFxSettingsListener(mFxPanel.get());
    

    // size responsabilty is moved to wrapper class below

    // setSize will call resized(). Put here so that child components are initialized first.
    //setSize (750, 310); //
    //setResizable(false, false);
    
    //mBackgroundImage = ImageCache::getFromMemory (BinaryData::Bg14_png, BinaryData::Bg14_pngSize);
    
    
}

PluginEditor::~PluginEditor()
{
    setLookAndFeel(nullptr);
    mMenuBar->removeFxTypeComboListener(mFxPanel.get());
    mMenuBar->removeFxSettingsListener(mFxPanel.get());   
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
      g.drawImage(ImageCache::getFromMemory(BinaryData::BgLarge_png, BinaryData::BgLarge_pngSize), getLocalBounds().toFloat());
}

void PluginEditor::resized()
{
        auto area = getLocalBounds();

        mTopPanel->setBounds (area.removeFromTop(45));
        auto const sidebarWidth = 100;
        mInputGainPanel->setBounds (area.removeFromLeft (sidebarWidth));
        mOutputGainPanel->setBounds (area.removeFromRight (sidebarWidth));
        mMenuBar->setBounds (area.removeFromTop(35 )); 
        mFxPanel->setBounds(area.removeFromTop (230));
}





// Wrapper implementation

WrappedRasterAudioProcessorEditor::WrappedRasterAudioProcessorEditor(PluginProcessor& p) :
    AudioProcessorEditor(p),
    rasterComponent(p), mProcessor(p)
{
    
    addAndMakeVisible(rasterComponent);

    //PropertiesFile::Options options;
    //options.applicationName = ProjectInfo::projectName;
    //options.commonToAllUsers = true;
    //options.filenameSuffix = "settings";
    ////options.osxLibrarySubFolder = "Application Support"; // TODO when doing mac version. add win or mac macro
    //applicationProperties.setStorageParameters(options);

    if (auto* constrainer = getConstrainer())
    {
        constrainer->setFixedAspectRatio(static_cast<double> (originalWidth) / static_cast<double> (originalHeight));
        constrainer->setSizeLimits(originalWidth / 4, originalHeight / 4, originalWidth * 2, originalHeight * 2);
    }

    double sizeRatio = mProcessor.getResizeFactor(); // was 1
    
    /*if (auto* properties = applicationProperties.getCommonSettings(true))
    {
        sizeRatio = properties->getDoubleValue("sizeRatio", 1.2);
    }*/

    setResizable(true, true);
    setSize(static_cast<int> (originalWidth * sizeRatio), static_cast<int> (originalHeight * sizeRatio));
}

void WrappedRasterAudioProcessorEditor::resized()
{
    const auto scaleFactor = static_cast<float> (getWidth()) / originalWidth;
    mProcessor.setResizeFactor(scaleFactor);

    //if (auto* properties = applicationProperties.getCommonSettings(true))
    //{
    //    properties->setValue("sizeRatio", scaleFactor);     // setScaleValue(scaleFactor);
    //}
    rasterComponent.setTransform(AffineTransform::scale(scaleFactor));
    rasterComponent.setBounds(0, 0, originalWidth, originalHeight);
}
