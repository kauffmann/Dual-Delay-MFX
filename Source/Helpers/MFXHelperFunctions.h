/*
  ==============================================================================

    MFXHelperFunctions.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MFXInterfaceDefine.h"
#include "MFXUsedParameters.h"



inline void paintComponentLabel(Graphics& g, Component* inComponent, juce::String label, bool isHorisontalSlider = 0.0f )
{
    const int x = inComponent->getX() - (inComponent->getWidth() * 0.25f);
    const int y = inComponent->getY() + inComponent->getHeight() * 0.75f;
    const int w = inComponent->getWidth() * 1.5f;
    const int h = 20;
    
    
    g.setColour(juce::Colour::fromRGB(155,155,155));
    g.setFont(font_4);
    
    
    if (isHorisontalSlider)
        g.drawFittedText(label, x, y - 10,
            w, h, Justification::centred, 1);
    else
        g.drawFittedText(label, x, y + 5, w, h, Justification::centred, 1);
}



