/*
  ==============================================================================

    MFXUnlockPanel.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXUnlockPanel.h"

MFXUnlockPanel::MFXUnlockPanel(PluginProcessor* inProcessor) : MFXPanelBase(inProcessor)
{
    

    mUnlockBackGround = ImageCache::getFromMemory(BinaryData::UnlockPanel_org2_png, BinaryData::UnlockPanel_org2_pngSize);

    mInputKeyField = std::make_unique<TextEditor>();
    mInputKeyField->setCaretVisible(false);
    mInputKeyField->setColour(TextEditor::textColourId, colour_18);
    mInputKeyField->setColour(TextEditor::backgroundColourId, colour_6);
    mInputKeyField->setColour(TextEditor::outlineColourId, colour_1);
    mInputKeyField->setColour(TextEditor::highlightColourId, colour_12);
    mInputKeyField->setFont(font_4);
    mInputKeyField->setInputRestrictions(35); 
    mInputKeyField->setTextToShowWhenEmpty("Please enter your license key(xx-xx-xx-xx)" , colour_1);

    addAndMakeVisible(mInputKeyField.get());

    
    const auto& image_unlockButton = ImageCache::getFromMemory(BinaryData::UnlockButton_org2_png, BinaryData::UnlockButton_org2_pngSize);
    const auto& image_demoButton = ImageCache::getFromMemory(BinaryData::DemoButton_org2_png, BinaryData::DemoButton_org2_pngSize);

    mUnlock = std::make_unique<ImageButton>("Unlock");
    mUnlock->setImages(false, false, true, image_unlockButton, 0.8f,
            Colour(0x0), image_unlockButton, 1.0f,
            Colour(0x0), image_unlockButton, 1.0f,
            Colour(0x0));

    mUnlock->onClick = [&]()
    {
        if (mProcessor->checkLicenseKey(mInputKeyField->getText()))
        {
            mProcessor->saveUnlockData(true);
        }
        else
        {
            mInputKeyField->setText("Invalid licence key");
        }     
    };

    addAndMakeVisible(mUnlock.get());

    
    mDemo = std::make_unique<ImageButton>("Demo");
    mDemo->setImages(false, false, true, image_demoButton, 0.8f,
                     Colour(0x0), image_demoButton, 1.0f,
                     Colour(0x0), image_demoButton, 1.0f,
                     Colour(0x0));
    
    mDemo->onClick = [&]()
    {
       
    };

    addAndMakeVisible(mDemo.get());

}

MFXUnlockPanel::~MFXUnlockPanel()
{
}

void MFXUnlockPanel::paint(Graphics& g)
{
    g.drawImageWithin(mUnlockBackGround, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit, false);
    g.setColour(colour_16);
    g.setFont(font_4);

    g.drawFittedText("In demo mode saving is disabled",
                     485.0,
                     2,
                     450.0f,
                     20.0f,
                     Justification::left,
                     0);

    g.drawFittedText("and there is 5s audio silence after 45s.",
                     485.0,
                     17,
                     450.0f,
                     20.0f,
                     Justification::left,
                     0);
}

void MFXUnlockPanel::resized()
{
    mInputKeyField->setBounds(100.0f, 10.0f, 250.0, 20.0f);
    mUnlock->setBounds(getWidth() - 394.0f, 11.0f, 90, getHeight()+4);
    mDemo->setBounds(getWidth() - 332.0f, 11.0f, 90, getHeight()+4);
}

void MFXUnlockPanel::addListenerToButtons(Button::Listener* listener)
{
    mUnlock->addListener(listener);
    mDemo->addListener(listener);
}

void MFXUnlockPanel::removeListenerToButtons(Button::Listener* listener)
{
    mUnlock->removeListener(listener);
    mDemo->removeListener(listener);
}


