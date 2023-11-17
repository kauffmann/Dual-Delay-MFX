/*
  ==============================================================================

    MFXCenterPanelMenuBar.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXCenterPanelMenuBar.h"




MFXCenterPanelMenuBar::MFXCenterPanelMenuBar (PluginProcessor* inProcessor)
: MFXPanelBase(inProcessor)
{
    
    mFxTypeComboBox = std::make_unique<MFXParameterComboBox>(mProcessor->parameters, MFXParameterID[mFXParameter_FXType], mProcessor);
    
    
    
    mFxTypeComboBox->addItem("Delay 1", 1);
    mFxTypeComboBox->addItem("Delay 2", 2);
    mFxTypeComboBox->addItem("Chorus", 3);
    mFxTypeComboBox->addItem("Phaser", 4);
    mFxTypeComboBox->addItem("Bit Crusher", 5);
    mFxTypeComboBox->addItem("Reverb", 6);
    mFxTypeComboBox->addItem("Multimode Filter", 7);
    mFxTypeComboBox->addItem("Advanced", 8);
    mFxTypeComboBox->addItem("Advanced MIDI", 9);
    
    
    mFxTypeComboBox->setSelectedItemIndex(0);
    mFxTypeComboBox->setColour(MFXParameterComboBox::backgroundColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(mFxTypeComboBox.get());
    
    createFxSettingButtons();
    
    
    
}


MFXCenterPanelMenuBar::~MFXCenterPanelMenuBar()
{
    
}


void MFXCenterPanelMenuBar::addFxTypeComboBoxListener(ComboBox::Listener* inListener)
{
    mFxTypeComboBox->addListener(inListener);
}

void MFXCenterPanelMenuBar::removeFxTypeComboListener(ComboBox::Listener* inListener)
{
    mFxTypeComboBox->removeListener(inListener);
}

void MFXCenterPanelMenuBar::paint (Graphics& g)
{
    MFXPanelBase::paint(g);
    
    auto outline =  Rectangle<int>(mFxTypeComboBox->getX(), mFxTypeComboBox->getY() + 5,
                                   mFxTypeComboBox->getWidth()-1, mFxTypeComboBox->getHeight() - 8).toFloat();
    
    g.drawRoundedRectangle(outline, 1.0f, 1.5f);
    
    
    
    
    
    

    
    
    
    
}

void MFXCenterPanelMenuBar::resized()
{
    const int width = 100;
    mFxTypeComboBox->setBounds(getWidth() - width, 0, width, getHeight());
    
    
    
}


void MFXCenterPanelMenuBar::addFxSettingsListener(ToggleButton::Listener* inListener)
{
    mButtons.getUnchecked(0)->addListener(inListener);
    mButtons.getUnchecked(2)->addListener(inListener);
    mButtons.getUnchecked(3)->addListener(inListener);
    mButtons.getUnchecked(4)->addListener(inListener);
    mButtons.getUnchecked(5)->addListener(inListener);
}


void MFXCenterPanelMenuBar::removeFxSettingsListener(ToggleButton::Listener* inListener)
{
    mButtons.getUnchecked(0)->removeListener(inListener);
    mButtons.getUnchecked(2)->removeListener(inListener);
    mButtons.getUnchecked(3)->removeListener(inListener);
    mButtons.getUnchecked(4)->removeListener(inListener);
    mButtons.getUnchecked(5)->removeListener(inListener);
}

void MFXCenterPanelMenuBar::createFxSettingButtons()
{
    
    
    const int button_size = 65; 
    const int button_x = 10;
    const int button_y = - 4;
    const int button_gap = 10;
    
    
    
    
    
    const auto& image_ParallelOff = ImageCache::getFromMemory(BinaryData::New_Parallel_Off_png, BinaryData::New_Parallel_Off_pngSize);
    const auto& image_ParallelOn = ImageCache::getFromMemory(BinaryData::New_On_Parallel_org2_png, BinaryData::New_On_Parallel_org2_pngSize);
    MFXParameterButton* parallel =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Parallel_Mode],
                           MFXParameterLabel[mFXParameter_Parallel_Mode], image_ParallelOff,
                           image_ParallelOn, mProcessor, mFXParameter_Parallel_Mode);
    parallel->setBounds(10, button_y, button_size,
        button_size);
    
    
    addAndMakeVisible(parallel);
    mButtons.add(parallel);
    
    
    
    
    const auto& image_AllFXOff = ImageCache::getFromMemory(BinaryData::New_All_FX_org_2_png, BinaryData::New_All_FX_org_2_pngSize);
    const auto& image_AllFXOn = ImageCache::getFromMemory(BinaryData::New_On_AllFX_org2_png, BinaryData::New_On_AllFX_org2_pngSize);
    MFXParameterButton* allFx =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_AllFxProcessing],
                           MFXParameterLabel[mFXParameter_AllFxProcessing],
                           image_AllFXOff, image_AllFXOn, mProcessor, mFXParameter_AllFxProcessing);
    allFx->setBounds(80, button_y, button_size, button_size);
    
    addAndMakeVisible(allFx);
    mButtons.add(allFx);

    
    // work-around*, because button listen to APVTS but do not call buttonClicked() on listeners when state is set by APVTS (close/reopen win)
    bool duckState = false;
    
    if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->getValue() )
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->setValue(false);
        duckState = true;
    }

    const auto& image_DuckingOff = ImageCache::getFromMemory(BinaryData::New_Ducking_org_2_png, BinaryData::New_Ducking_org_2_pngSize);

    const auto& image_DuckingOn = ImageCache::getFromMemory(BinaryData::New_On_Ducking_org2_png, BinaryData::New_On_Ducking_org2_pngSize);

    MFXParameterButton* ducking =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Mode],
                           MFXParameterLabel[mFXParameter_Ducking_Mode], image_DuckingOff,
                           image_DuckingOn, mProcessor, mFXParameter_Ducking_Mode);
    ducking->setBounds(156, button_y, button_size, button_size);
    
    // work-around*
    if (duckState)
        ducking->triggerClick();

    
    addAndMakeVisible(ducking);
    mButtons.add(ducking);


    // work-around*, because button listen to APVTS but do not call buttonClicked() on listeners when state is set by APVTS (close/reopen win)
    bool mixState = false;
    
    if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mix_Mode])->getValue() )
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mix_Mode])->setValue(false);
        mixState = true;
    }
    
    const auto& image_MixOff = ImageCache::getFromMemory(BinaryData::New_Mix_org_2_png, BinaryData::New_Mix_org_2_pngSize);
    const auto& image_MixOn = ImageCache::getFromMemory(BinaryData::New_On_Mix_org2_png, BinaryData::New_On_Mix_org2_pngSize);
    MFXParameterButton* mix =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Mix_Mode],
                           MFXParameterLabel[mFXParameter_Mix_Mode],
                           image_MixOff, image_MixOn, mProcessor, mFXParameter_Mix_Mode);
    mix->setBounds(230, button_y, button_size, button_size);
    
    
    if (mixState)
        mix->triggerClick();
    
    
    addAndMakeVisible(mix);
    mButtons.add(mix);
    
    
    
    bool fxState = false;
    
    if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FX_Level_Mode])->getValue() )
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FX_Level_Mode])->setValue(false);
        fxState = true;
    }
    
    const auto& image_FXLevelsOff = ImageCache::getFromMemory(BinaryData::New_Fx_level_org_2_png, BinaryData::New_Fx_level_org_2_pngSize);

    const auto& image_FXLevelsOn = ImageCache::getFromMemory(BinaryData::New_On_FX_Levels_org2_png, BinaryData::New_On_FX_Levels_org2_pngSize);
    MFXParameterButton* fxLevels =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_FX_Level_Mode],
                           MFXParameterLabel[mFXParameter_FX_Level_Mode],
                           image_FXLevelsOff, image_FXLevelsOn, mProcessor, mFXParameter_FX_Level_Mode);
    fxLevels->setBounds(278, button_y, button_size, button_size);
    
    
    if (fxState)
        fxLevels->triggerClick();
    
    
    addAndMakeVisible(fxLevels);
    mButtons.add(fxLevels);
    
    
    
    bool cutState = false;
    
    if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->getValue() )
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->setValue(false);
        cutState = true;
    }
    const auto& image_CutModeOff = ImageCache::getFromMemory(BinaryData::New_Shaper_org_2_png, BinaryData::New_Shaper_org_2_pngSize);
    const auto& image_CutModeOn = ImageCache::getFromMemory(BinaryData::New_On_Shaper_org2_png, BinaryData::New_On_Shaper_org2_pngSize);
    MFXParameterButton* cutMode =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Cut_Mode],
                           MFXParameterLabel[mFXParameter_Cut_Mode],
                           image_CutModeOff, image_CutModeOn, mProcessor, mFXParameter_Cut_Mode);
    cutMode->setBounds(358, button_y, button_size, button_size);
    
    if (cutState)
        cutMode->triggerClick();
    
    addAndMakeVisible(cutMode);
    mButtons.add(cutMode);
    
    ducking->onClick = [=]()
    {
        
        if (mix->getToggleState() )
        {    mix->setToggleState(false, NotificationType::dontSendNotification);
            // Need to set param in apvts, because setToggle does not set param in apvts
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mix_Mode])->setValue(false);
        }
        
        else if (fxLevels->getToggleState() )
        {        fxLevels->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FX_Level_Mode])->setValue(false);
        }
        else if (cutMode->getToggleState())
        {
                 cutMode->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->setValue(false);
        }

        
    };
    
    mix->onClick = [=]()
    {
        
        if (ducking->getToggleState())
        {    ducking->setToggleState(false, NotificationType::dontSendNotification);
             mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->setValue(false);
        }
        else if (fxLevels->getToggleState())
        {
                 fxLevels->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FX_Level_Mode])->setValue(false);
        }
        else if (cutMode->getToggleState())
        {
                 cutMode->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->setValue(false);
        }
        

    };
    
    fxLevels->onClick = [=]()
    {
        if (mix->getToggleState())
        {
            mix->setToggleState(false, NotificationType::dontSendNotification);
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mix_Mode])->setValue(false);

        }
        else if (ducking->getToggleState())
        {
                 ducking->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->setValue(false);
        }
        else if (cutMode->getToggleState())
        {
                 cutMode->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->setValue(false);

        }
    };
    
    cutMode->onClick = [=]()
    {
        
        if (mix->getToggleState())
        {
            mix->setToggleState(false, NotificationType::dontSendNotification);
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mix_Mode])->setValue(false);

        }
        else if (ducking->getToggleState())
        {
                 ducking->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->setValue(false);
            
        }
        else if (fxLevels->getToggleState())
        {
                 fxLevels->setToggleState(false, NotificationType::dontSendNotification);
                 mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FX_Level_Mode])->setValue(false);
            
        }
    };
    
}
