/*
  ==============================================================================

    MFXLookAndFell.h
    
    Author:  Michael Kauffmann
 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXInterfaceDefine.h"

class MFXLookAndFeel : public LookAndFeel_V4
{

public:
    
    MFXLookAndFeel()
    {
        setColour(ComboBox::textColourId, juce::Colour::fromRGB(155,155,155));

        mSliderImage = ImageCache::getFromMemory(BinaryData::kadenze_knob_png,
            BinaryData::kadenze_knob_pngSize);

    }
    
    virtual ~MFXLookAndFeel(){};
    
    /** buttons    don't use it*/  
    Font getTextButtonFont (TextButton&, int buttonHeight) override
    {
        return font_2;
    }
    
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {    
        Colour fillColour;
        
        if(isButtonDown){
            fillColour = colour_6;    
        }
        else
        {
            fillColour = colour_6;
        }   
    }
    


    void drawImageButton(Graphics& g, Image* image, int imageX, int imageY,
                        int imageW, int imageH, const Colour& overlayColour,
                        float imageOpacity, ImageButton& button) override
    {

        

        if (!button.isEnabled())
            imageOpacity *= 0.3f;

        AffineTransform t = RectanglePlacement(RectanglePlacement::xLeft)
                           .getTransformToFit(image->getBounds().toFloat(),
                            Rectangle<int>(imageX/16, imageY, imageW /2, imageH/2).toFloat());

        if (!overlayColour.isOpaque())
        {
            g.setOpacity(imageOpacity);
            g.drawImageTransformed(*image, t, false);
            
        }
        
        //not used
        if (!overlayColour.isTransparent())
        {
            g.setColour(overlayColour);
            g.drawImageTransformed(*image, t, true);
        }

        
    
    }


    void drawToggleButton (Graphics& g, ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
    {
        auto fontSize = jmin (15.0f, (float) button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f; // 1.1f
        
        drawTickBox (g, button, 4.0f, ((float) button.getHeight() - tickWidth) * 0.5f,
                     tickWidth, tickWidth,
                     button.getToggleState(),
                     button.isEnabled(),
                     shouldDrawButtonAsHighlighted,
                     shouldDrawButtonAsDown);
        
        g.setColour (button.findColour (ToggleButton::textColourId));
        g.setFont (font_1); 
        
        if (! button.isEnabled())
            g.setOpacity (0.5f);
        
        g.drawFittedText (button.getButtonText(),
                          button.getLocalBounds().withTrimmedLeft (roundToInt (tickWidth) + 10)
                          .withTrimmedRight (2),
                          Justification::centredLeft, 10);
    }
    
    
    void drawTickBox (Graphics& g, Component& component,
                                      float x, float y, float w, float h,
                                      const bool ticked,
                                      const bool isEnabled,
                                      const bool shouldDrawButtonAsHighlighted,
                                      const bool shouldDrawButtonAsDown) override
    {
        ignoreUnused (isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        
        Rectangle<float> tickBounds (x, y, w, h);
        
        g.setColour (component.findColour (ToggleButton::tickDisabledColourId));
        g.drawRoundedRectangle (tickBounds, 2.0f, 1.8f);
        
        if (ticked)
        {
            g.setColour (component.findColour (ToggleButton::tickColourId));
            
            tickBounds.reduce(2.0f, 2.0f);
            g.fillRoundedRectangle (tickBounds, 2.0f);
        }
    }
    
    
    /** comboBoxes */
    Font getLabelFont(Label& label) override
    {
        return font_4;
    }
    
    
    
    
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override
    {
    
        
        juce::Rectangle<int> r (area);
        
        
        Colour fillColour = isHighlighted ? colour_5 : colour_6;
        g.setColour(fillColour);
        g.fillRect(r);
        Colour myTextColour = isTicked ? colour_7 : colour_1;
        g.setColour(myTextColour);
        g.setFont(font_4);
        r.setLeft(10);
        r.setY(1);
        g.drawFittedText(text, r, Justification::left, 1);      
    }
    
    
    
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& box) override
    {
        const float cornerSize = 3.0f;
        const juce::Rectangle<int> boxBounds (0, 0, width, height);
        
         
        g.setColour(box.findColour(ComboBox::backgroundColourId)); 
        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);  
        
        juce::Rectangle<int> arrow (width - 30, 0, 20, height);
        
    }
    

    // Experiment sprite sheet slider. Works, but image scaling not good, make max size images.

    //void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    //    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    //{
    //    const int numFrames = mSliderImage.getHeight() / mSliderImage.getWidth();
    //    const int frameIdx = (int)ceil(sliderPos * (numFrames - 1));

    //    const float radius = width * 0.5; //jmin(width * 0.5, height * 0.5);
    //    const float centerX = x + radius; //width * 0.5;
    //    const float centerY = y + height * 0.5;
    //    const float rx = centerX - radius;
    //    const float ry = centerY - radius;

    //    g.drawImage(mSliderImage,
    //        rx,
    //        ry+5, 
    //        2 * radius,
    //        2 * radius,
    //        0,  // down from here, what part of image to draw, starting from x(0),y(frameIdx*...)), the width and height
    //        frameIdx * mSliderImage.getWidth(),
    //        mSliderImage.getWidth(),
    //        mSliderImage.getWidth());
    //}
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override
    {
        
        auto colour1 = Colours::yellow;
        auto colour4 = Colours::whitesmoke;
        
        
        // rectangle to draw our knob in
        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced (10);
        
        // various coefficients
        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        float centerX = x + width * 0.5;
        float centerY = y + height * 0.5;
        
        // scale drawed line curve
        float toAngle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        
        g.setGradientFill(juce::ColourGradient::vertical(Colours::transparentBlack, centerY * 1.2, Colours::floralwhite, centerY * 2.5)); 
        
        g.fillEllipse(bounds);
        g.setColour(colour1.interpolatedWith (colour4, 0.55  )); //tick color
        juce::Path dialTick;
        dialTick.addRectangle (0, -radius + 3, 2.316f,
                                     radius * 0.68
                                     );
        g.fillPath(dialTick, juce::AffineTransform::rotation(toAngle - 0.021 ).translated(centerX, centerY));
        
        
     
        // ------------------------Create outer curved line stroke--------------------------------
        

        
        slider.setColour(Slider::rotarySliderFillColourId, colour1.interpolatedWith (colour4, 0.55  )); 
        
        auto fill = slider.findColour (Slider::rotarySliderFillColourId);
        
        auto lineW = jmin (2.58f , radius * 0.2f); // thickness (width og curved background line)
        auto lineWbackground = jmin (7.5f, radius * 0.5f); // thickness (width og curved background line)
        auto arcRadius = radius - lineW * 0.2f;
        auto arcRadius2 = radius - lineW;
        
        
        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius2,
                                    arcRadius2,
                                    0.027f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);
            
            g.setColour (fill);
            g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::square));
            
        }
        
        // create background
        Path backgroundArc;
        // check addCentredArc doc, cos, sin stuff
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);
        
        g.setColour(juce::Colours::yellow.withAlpha(0.25f)); 
        g.strokePath (backgroundArc, PathStrokeType (lineWbackground, PathStrokeType::curved, PathStrokeType::square));


        
    }

    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override
    {
        
        
        if (slider.isBar())
        {
            g.setColour (slider.findColour (Slider::trackColourId));
            g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f) // -1
                        : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
            
        }
        else
        {
            
            auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);
            
            auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.50f: (float) width * 0.25f);
        
            Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                     slider.isHorizontal() ? (float) y + (float) height * 0.13f : (float) (height + y)); 
            
            Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                                   slider.isHorizontal() ? startPoint.y : (float) y);
            
            Path backgroundTrack;
            backgroundTrack.startNewSubPath (startPoint);
            backgroundTrack.lineTo (endPoint);
            g.setColour (colour_17);
            g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::beveled, PathStrokeType::square});

            Path valueTrack;
            Point<float> minPoint, maxPoint, thumbPoint;
            
            if (isTwoVal || isThreeVal)
            {
                minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };
                
                if (isThreeVal)
                    thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                        slider.isHorizontal() ? (float) height * 0.5f : sliderPos };
                
                maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
                
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
                auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.13f) : sliderPos; 
                
                minPoint = startPoint;
                maxPoint = { kx, ky };
             
            }
            
            
            
            valueTrack.startNewSubPath (minPoint);
            valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
            g.setColour (slider.findColour (Slider::trackColourId)); 
            g.strokePath (valueTrack, { trackWidth, PathStrokeType::beveled, PathStrokeType::square }); 
            

            if (! isTwoVal)
            {
                g.setColour (colour_18);    
            }
            
            if (isTwoVal || isThreeVal)
            {
                
                auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
                auto pointerColour = slider.findColour (Slider::thumbColourId);
                
                if (slider.isHorizontal())
                {
                    drawPointer (g, minSliderPos - sr,
                                 jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
                                 trackWidth * 2.0f, pointerColour, 2);
                    
                    drawPointer (g, maxSliderPos - trackWidth,
                                 jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
                                 trackWidth * 2.0f, pointerColour, 4);
                }
                else
                {
                    drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
                                 minSliderPos - trackWidth,
                                 trackWidth * 2.0f, pointerColour, 1);
                    
                    drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
                                 trackWidth * 2.0f, pointerColour, 3);
                }
            }
            
            
        }
    }

    
   
    
    
private:
    
    
    Image mSliderImage;
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXLookAndFeel);
    
    
    
    
};
