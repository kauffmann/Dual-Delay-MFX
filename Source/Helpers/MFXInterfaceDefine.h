/*
  ==============================================================================

    MFXInterfaceDefine.h
    
    Author:  Michael Kauffmann
    
    .css like style sheet .h header file. 
    Allow global access.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


// TODO:: put comment that state what colour it is, god damn!!
const Colour colour_1 = Colours::white.withAlpha(0.4f);
const Colour colour_2 = Colour(0, 0, 0).withAlpha(0.0f);
const Colour colour_3 = Colour(0, 0, 0).withAlpha(0.3f);
const Colour colour_4 = Colour(0, 0, 0).withAlpha(0.6f);
const Colour colour_5 = Colour(105, 105, 105).withAlpha(0.3f);
const Colour colour_6 = Colour(0, 0, 0).withAlpha(0.8f); // black
const Colour colour_7 = Colour(125, 125, 125);
const Colour colour_8 = Colour(Colours::red).withAlpha( 0.7f);
const Colour colour_9 = Colour(Colours::transparentBlack);
const Colour colour_10 = Colours::seagreen.interpolatedWith(colour_1, 0.3f);
const Colour colour_11 = Colours::blueviolet.interpolatedWith(colour_1, 0.3f);
const Colour colour_12 = Colours::white.withAlpha(0.2f);
const Colour colour_13 = Colours::royalblue.withAlpha(0.7f);
const Colour colour_14 = Colours::yellow.interpolatedWith (colour_1, 0.3);
const Colour colour_15 = Colours::red.interpolatedWith ( colour_1, 0.1);
const Colour colour_16 = juce::Colour::fromRGB(155,155,155); // White Text labels
const Colour colour_17 = Colours::white.withAlpha(0.1f);
const Colour colour_18 = Colours::white.withAlpha(0.6f);





// System uses system font: for cross compatiblity win/Mac/Linux use binaries with assets (system access own stored fonts)
// hmmm on windows I had to replace robotoBold with a font from google fonts.....also helvetica desk interface did't work on windows
const Font font_1 = Font(Typeface::createSystemTypefaceFor(BinaryData::Helvetica65Medium_22443_ttf, BinaryData::Helvetica65Medium_22443_ttfSize)).withHeight(12.0).withStyle(1);

const Font font_2 = Font(Typeface::createSystemTypefaceFor(BinaryData::Helvetica65Medium_22443_ttf, BinaryData::Helvetica65Medium_22443_ttfSize)).withHeight(22.0).withStyle(1);

const Font font_3 = Font(Typeface::createSystemTypefaceFor(BinaryData::Helvetica65Medium_22443_ttf, BinaryData::Helvetica65Medium_22443_ttfSize)).withHeight(48.0).withStyle(1);


const Font font_4 = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoBold_ttf,
                                                           BinaryData::RobotoBold_ttfSize)).withHeight(12.0).withStyle(1);

// Use by helpText
const Font font_5 = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoBold_ttf,
                                                           BinaryData::RobotoBold_ttfSize)).withHeight(11.0).withStyle(1);
// Used by Headers like LFO, Chorus....
const Font font_6 = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoBold_ttf,
                                                           BinaryData::RobotoBold_ttfSize)).withHeight(15.0).withStyle(1);

const Font font_7 = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoBold_ttf,
                                                           BinaryData::RobotoBold_ttfSize)).withHeight(10.0).withStyle(1);



