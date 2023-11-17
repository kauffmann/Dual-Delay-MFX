/*
  ==============================================================================

    MFXLookAndFell.h
    
    Author:  Michael Kauffmann
 
    Contain lots of comment code. I'll keep it for now.

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
        // store image assets
        //mSliderImage = ImageCache::getFromMemory(BinaryData::kadenze_knob_png, BinaryData::kadenze_knob_pngSize);
        
        // comboBox colours
        
          // if not drawCombobox() overwrite then this
          //setColour(ComboBox::backgroundColourId, juce::Colour::fromRGB(132,144,123)); // was colour_3
  //          setColour(ComboBox::outlineColourId, colour_6);
//          setColour(ComboBox::arrowColourId, colour_1);
            setColour(ComboBox::textColourId, juce::Colour::fromRGB(155,155,155));
        

        
        
        //button text colours
//        setColour(ToggleButton::buttonColourId, colour_1);
//        setColour(ToggleButton::textColourOnId, colour_1);
//        setColour(ToggleButton::textColourOffId, colour_1);
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
//        } else if(isMouseOverButton){
//            fillColour = colour_13; // 3
        }
        else
        {
            fillColour = colour_6; // 5
        }
        
        //const float cornerSize = 6.0f;
        //const juce::Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        
        //g.setColour(fillColour);
        //g.fillRoundedRectangle(bounds.reduced(1), cornerSize);
        
    }
    


    void drawImageButton(Graphics& g, Image* image, int imageX, int imageY,
        int imageW, int imageH, const Colour& overlayColour,
        float imageOpacity, ImageButton& button) override
    {

        

        if (!button.isEnabled())
            imageOpacity *= 0.3f;

        AffineTransform t = RectanglePlacement(RectanglePlacement::xLeft)
            .getTransformToFit(image->getBounds().toFloat(),
                Rectangle<int>(imageX/16, imageY, imageW /2, imageH/2).toFloat());     // added /2

        

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


    // I don't use this  , TODO clean up  
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
        g.setFont (font_1); // fontSize
        
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
            //auto tick = getTickShape (0.75f);
            //g.fillPath (tick, tick.getTransformToScaleToFit (tickBounds.reduced (4, 5).toFloat(), false));
            // this replace above shape (a boring check sign) with a simple but nice looking square
            tickBounds.reduce(2.0f, 2.0f);
            g.fillRoundedRectangle (tickBounds, 2.0f);
        }
    }
    
    /** comboBoxes */
    Font getLabelFont(Label& label) override
    {
        return font_4; // was 1, latest 5     selected text
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override
    {
    
        
        juce::Rectangle<int> r (area);
        
        
        Colour fillColour = isHighlighted ? colour_5 : colour_6;
        g.setColour(fillColour);
        //g.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight()); // r.getHeight() -1 creates border
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
        
         //box.setColour(ComboBox::backgroundColourId, box.findColour(ComboBox::backgroundColourId));
         g.setColour(box.findColour(ComboBox::backgroundColourId));  // was colour_3
        //g.setColour(colour_2);
         
         //g.setColour(box.findColour(ComboBox::outlineColourId));
        
        


         

        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);  // commented 30 sep 2021. this way its tranparent
        
        
        
        

        juce::Rectangle<int> arrow (width - 30, 0, 20, height);
        
        Path path;
        
        
        // This draw a arrow down
//        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
//        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
//        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
//        
//        //Colour arrowColour = box.findColour(ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f);
//        g.setColour(colour_1); // arrowColour colour_1
//        g.strokePath(path, PathStrokeType(2.0f));
//        
        
       
    }
    
//    PopupMenu::Options getOptionsForComboBoxPopupMenu (ComboBox& box, Label& label) override
//    {
////        return PopupMenu::Options().withItemThatMustBeVisible (box.getSelectedId());
//    }
//    
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override{
        
//        shadowProperties.radius = slider.getWidth()+3;
//        shadowProperties.offset = juce::Point<int> (-1,3); // -1, 3
//        dialShadow.setShadowProperties (shadowProperties);
//        
//        slider.setComponentEffect(&dialShadow);
        
        
        
        auto colour1 = Colours::yellow;
        auto colour4 = Colours::whitesmoke;
        //auto colour3 = Colours::blueviolet;
        
        // rectangle to draw our knob in
        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced (10);
        
        // various coefficients
        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        //float diameter = radius * 2;
        float centerX = x + width * 0.5;
        float centerY = y + height * 0.5;
        
        // scale drawed line curve
        float toAngle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        //g.setColour(juce::Colour::fromFloatRGBA(0.15, 0.15, 0.15, 1)); //center
//        g.setGradientFill(juce::ColourGradient::horizontal(juce::Colour::fromFloatRGBA(0.1, 0.1, 0.1, 1),
//                                                           centerY * .25, juce::Colour::fromFloatRGBA(0.15, 0.15, 0.15, 1),
//                                                           centerY * 2.5));
        
        
        g.setGradientFill(juce::ColourGradient::vertical(Colours::transparentBlack, centerY * 1.2, Colours::floralwhite, centerY * 2.5)); // was .25 / 2.5
        
        g.fillEllipse(bounds);
        //g.setColour(juce::Colour::fromFloatRGBA(0.392f, 0.584f, 0.929f, 0.5f)); //tick color
        g.setColour(colour1.interpolatedWith (colour4, 0.55  )); //tick color
         //g.setColour (Colours::blueviolet);
        juce::Path dialTick;
        //dialTick.addRoundedRectangle(0, -radius + 3, 3.0f, radius * 0.452211, 1.3f);
        dialTick.addRectangle (0, -radius + 3, 2.316f,
                                     radius * 0.68
                                     ); // 0.452211  height/length
        g.fillPath(dialTick, juce::AffineTransform::rotation(toAngle - 0.021 ).translated(centerX, centerY));
        
        
        
        
        
        
        
        
        // ------------------------Create outer curved line stroke--------------------------------
        

        
        slider.setColour(Slider::rotarySliderFillColourId, colour1.interpolatedWith (colour4, 0.55  )); // was 0.45
        
        auto fill = slider.findColour (Slider::rotarySliderFillColourId);
        
        auto lineW = jmin (2.58f , radius * 0.2f); // thickness (width og curved background line) (2.5f, radius * 0.5f)
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
        
        g.setColour(juce::Colours::yellow.withAlpha(0.25f)); //(colour1.withAlpha(0.35f) ); // 0.2
        g.strokePath (backgroundArc, PathStrokeType (lineWbackground, PathStrokeType::curved, PathStrokeType::square));


        
    }

//    juce::Rectangle<int> maxQuadIn(const juce::Rectangle<int>& b) noexcept // was float
//    {
//        const auto minDimen = std::min(b.getWidth(), b.getHeight());
//        const auto x = b.getX() + .5f * (b.getWidth() - minDimen);
//        const auto y = b.getY() + .5f * (b.getHeight() - minDimen);
//        return { static_cast<int>(x), static_cast<int>(y), minDimen, minDimen };
//    }
    
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
            
            auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.50f: (float) width * 0.25f); // height was 0.25 0.5
        
            Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                     slider.isHorizontal() ? (float) y + (float) height * 0.13f : (float) (height + y)); // h * 0.15 set y from height
            
            Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                                   slider.isHorizontal() ? startPoint.y : (float) y);
            
            Path backgroundTrack;
            backgroundTrack.startNewSubPath (startPoint);
            backgroundTrack.lineTo (endPoint);
            g.setColour (colour_17);//(slider.findColour (Slider::backgroundColourId)); // colour_1    colour_12
            g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::beveled, PathStrokeType::square});

            //g.strokePath(backgroundTrack, juce::PathStrokeType(6.0, juce::PathStrokeType::curved, juce::PathStrokeType::EndCapStyle::rounded));
            
            
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
                auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.13f) : sliderPos; // h * 0.15 set y from height 0.13
                
                minPoint = startPoint;
                maxPoint = { kx, ky };
             
            }
            
            //auto thumbWidth = getSliderThumbRadius (slider) * 0.6f;// 0.8
            
            valueTrack.startNewSubPath (minPoint);
            valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
            g.setColour (slider.findColour (Slider::trackColourId)); //was colour_11, change allow locally setcolour of slider. 
            g.strokePath (valueTrack, { trackWidth, PathStrokeType::beveled, PathStrokeType::square }); // change from rounded to beveled.
            //g.strokePath(valueTrack, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::EndCapStyle::rounded));

            
            if (! isTwoVal)
            {
                g.setColour (colour_18);//(slider.findColour (Slider::thumbColourId));
                //g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));
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
