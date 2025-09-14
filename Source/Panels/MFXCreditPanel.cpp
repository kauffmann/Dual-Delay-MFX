/*
  ==============================================================================

    MFXCreditPanel.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXCreditPanel.h"


MFXCreditPanel::MFXCreditPanel(){}

MFXCreditPanel::~MFXCreditPanel(){}

 
void MFXCreditPanel::paint(Graphics& g)
{
    
    mCreditBackGround = ImageCache::getFromMemory(BinaryData::CreditPanel_org2_png, BinaryData::CreditPanel_org2_pngSize);
    g.drawImageWithin(mCreditBackGround, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit, false);
    
    
    g.setColour(colour_16);
    g.setFont(font_4);
    
    g.drawFittedText("Design, programming and preset design by Michael Kauffmann",
                     100,
                     2,
                     400.0f, 20.0f, Justification::centredLeft, 1);
    
    g.drawFittedText(String(CharPointer_UTF8("© 2023 Be Good Audio. All rights reserved. Made in Denmark.")),
                     100,
                     17,
                     400.0f, 20.0f, Justification::centredLeft, 1);

    g.drawFittedText(String(CharPointer_UTF8("Version: 1.1.2.Share")),
        getWidth() - 100,
        17,
        400.0f, 20.0f, Justification::left, 1);
    
    
}

void MFXCreditPanel::resized()
{
    // bounds area of creditPanel are set in topPanel when "about" in drop dwon box is selected, using getLocalBounds() to fit creditpanel within topPanel area.
}

void MFXCreditPanel::mouseDown(const juce::MouseEvent &event)
{
    this->setVisible(false);
    
}
