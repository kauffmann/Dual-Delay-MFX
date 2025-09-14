/*
  ==============================================================================

    MFXFxPanel.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXFxPanel.h"






MFXFxPanel::MFXFxPanel(PluginProcessor* inProcessor)
: MFXPanelBase(inProcessor)
{
    startTimerHz (30);
}

MFXFxPanel::~MFXFxPanel(){}

void MFXFxPanel::paint(Graphics& g)
{
    
    g.setColour(colour_16);
    g.setFont(font_6);
    String label;
    
    switch (mStyle)
    {
            
        case (mFxPanelStyle_Delay2):{
             label = "DELAY 2";
             g.drawFittedText(label,
                              55,
                              50,
                              100.0f, 25.0f, Justification::centred, 1);
             repaint();
            
            
        } break;
            
        case (mFxPanelStyle_Chorus):{    
             label = "CHORUS";
             g.drawFittedText(label,
                             55,
                             50,
                             100.0f, 25.0f, Justification::centred, 1);
             repaint();
            
        } break;
            
        case (mFxPanelStyle_Reverb):{
             label = "REVERB";

             g.drawFittedText(label,
                              55,
                              50,
                              100.0f, 25.0f, Justification::centred, 1);
             repaint();
        } break;
        
        case (mFxPanelStyle_Filter):{
             label = "MULTIMODE FILTER";

             g.drawFittedText(label,
                              80,
                              50,
                              105.0f, 25.0f, Justification::centred, 1);
             repaint();
            
            
            
        } break;
            
        case (mFxPanelStyle_Phaser):{
             label = "PHASER";

             g.drawFittedText(label,
                             55,
                             50,
                             100.0f, 25.0f, Justification::centred, 1);
             repaint();
        } break;
            
        case (mFxPanelStyle_BitCrusher):{
             label = "BITCRUSHER";

             g.drawFittedText(label,
                             80,
                             50,
                             105.0f, 25.0f, Justification::centred, 1);
             repaint();
        } break;
            
        case (mFxPanelStyle_Delay1):{
             label = "DELAY 1";
             g.drawFittedText(label,
                              55,
                              50,
                              100.0f, 25.0f, Justification::centred, 1);
             
             repaint();
             
            
            
        } break;
            
        case (mFxPanelStyle_Advanced):{
            
            g.drawFittedText("Envelope Follower",
                             220,
                             47,
                             100.0f, 18.0f, Justification::centred, 1);
            
            g.drawFittedText("LFO 1",
                             100,
                             120,
                             30.0f, 18.0f, Justification::centred, 1);
            
            
            g.drawFittedText("LFO 2",
                             270,
                             120,
                             30.0f, 18.0f, Justification::centred, 1);
            
            repaint();
            
            
        } break;   

        case (mFxPanelStyle_MIDI_ADSR): {

        
            g.drawFittedText("Envelope 1",120,75,
                100.0f, 18.0f, Justification::centred, 1);


            g.drawFittedText("Envelope 2",340,75,
                100.0f, 18.0f, Justification::centred, 1);


            g.drawFittedText("MIDI", 66, 200,
                30.0f, 18.0f, Justification::centred, 1);

           

            repaint();


        } break;
            
        default:
        
             label = "DELAY";
             g.drawFittedText(label,
                              55,
                              50,
                              100.0f, 25.0f, Justification::centred, 1);
             repaint();
            
            //jassertfalse;   // if asserts, why do I end here?
        
    }
    
    
    
    
    
    for (auto& slider : mSliders)
    {
        // Should have been in param init lambda (PluginProcessor::createParameterLayout()) but valueText was not updated correct when timeMode changed.
        if (slider->getName() == "Time Left")
        {
            float TimeModeIndexDelay = mStyle == mFxPanelStyle_Delay1 ? mProcessor->parameters.getRawParameterValue( MFXParameterID[mFXParameter_Delay1TimeModeLeft])->load()
            : mProcessor->parameters.getRawParameterValue( MFXParameterID[mFXParameter_Delay2TimeModeLeft])->load();
            float value = slider->getValue();
            
            String valueToText = (TimeModeIndexDelay > 0)
            ? setValueToNoteText(value, TimeModeIndexDelay)
            : String((value * 4000),1) + setValueToNoteText(value,TimeModeIndexDelay);
            
            mStyle == mFxPanelStyle_Delay1 ? mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Delay1TimeLeft] = valueToText
            : mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Delay2TimeLeft] = valueToText;
        }
        else if (slider->getName() == "Time Right")
        {
            float TimeModeIndexDelay = mStyle == mFxPanelStyle_Delay1 ? mProcessor->parameters.getRawParameterValue( MFXParameterID[mFXParameter_Delay1TimeModeRight])->load()
            : mProcessor->parameters.getRawParameterValue( MFXParameterID[mFXParameter_Delay2TimeModeRight])->load();
            float value = slider->getValue();
            
            String valueToText = (TimeModeIndexDelay > 0)
            ? setValueToNoteText(value, TimeModeIndexDelay)
            : String((value * 4000),1) + setValueToNoteText(value,TimeModeIndexDelay);
            
            mStyle == mFxPanelStyle_Delay1 ? mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Delay1TimeRight] = valueToText
            : mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Delay2TimeRight] = valueToText;
        }
    
        
        
        if (slider->isMouseOverOrDragging())
        {
            // Fixed Reaper issue
            paintComponentLabel(g, slider, mProcessor->parameters.getParameter(MFXParameterID[slider->getParameterValueText()])->getCurrentValueAsText());
        }
        else if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mid_Side_Mode])->getValue() == true
                 && slider->getName() == "Time Left")
        {
            paintComponentLabel(g, slider, "Mid");
        }
        else if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mid_Side_Mode])->getValue() == true
                 && slider->getName() == "Time Right")
        {
            paintComponentLabel(g, slider, "Side");
        }
        
        
        else if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mid_Side_Mode])->getValue() == true
                 && slider->getName() == "Feedback Left")
        {
            paintComponentLabel(g, slider, "Feedback Mid");
        }
        
        else if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Mid_Side_Mode])->getValue() == true
                 && slider->getName() == "Feedback Right")
        {
            paintComponentLabel(g, slider, "Feedback Side");
        }
        
        else
        {
            paintComponentLabel(g, slider, slider->getName());
        }
          
    }
    
    
    for (auto& slider : mGlobalSliders)
    {
        if (slider->isMouseOverOrDragging())
        {
            //Reaper fix
            paintComponentLabel(g, slider, mProcessor->parameters.getParameter(MFXParameterID[slider->getParameterValueText()])->getCurrentValueAsText(), true);
        }
        else
        {
            if (slider->getName() != "Delay 2")
                paintComponentLabel(g, slider, slider->getName(), true);
            
            if (slider->getName() == "Delay 2" && mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Parallel_Mode])->load() == false)
                paintComponentLabel(g, slider, "Delay 1/2", true); 
            else
                paintComponentLabel(g, slider, slider->getName(), true);
        }
        
    }
    
    
    for (auto& slider : mFXPanelHorisontalSliders)
    {
        
        
        if (slider->isMouseOverOrDragging())
        {
            if(slider->getName() == "Note")
            {
               juce::StringArray notes = mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Note])->getAllValueStrings();
               
                mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Advanced_LFO1_Note] =
                notes.getReference( (int) slider->getValue());
                
                mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Advanced_LFO2_Note] =
                notes.getReference( (int) slider->getValue());
                
                paintComponentLabel(g, slider, mProcessor->getAccessToUIPersistentData().mParameterValueText[slider->getParameterValueText()], true);
            }
                
            else
            {
                // Reaper Fix
                paintComponentLabel(g, slider, mProcessor->parameters.getParameter(MFXParameterID[slider->getParameterValueText()])->getCurrentValueAsText(), true);
            }
        }
        else
        {
            paintComponentLabel(g, slider, slider->getName(), true);
        }
        
    }
    
    g.setFont(font_5);   
    g.drawFittedText(mProcessor->getAccessToUIPersistentData().mHelperUIText,
                     10,
                     getHeight() - 18,
                     500.0f, 25.0f, Justification::left, 1);
    
}

void MFXFxPanel::setFxPanelStyle(FxPanelStyle inStyle)
{
    mStyle = inStyle;
	//-------------------------------------------Clear all UI elements to avoid mixed UI components conflicts-------------------------------------------
    mSliders.clear();
    mComboBoxes.clear();
	mButtons.clear();
    mFXPanelHorisontalSliders.clear();
	
    

	mDelay1TimeTransitionMode = nullptr;
	mDelay2TimeTransitionMode = nullptr;
  
	mSetRandomValues = nullptr;
	mSetRandomAllValues = nullptr;
	mMidi = nullptr;

   

    //-------------------------------------------------------------------------------------
	// I'm not happy about size and x and y position of UI components, it has become a mess and hard to read.
    // There are places where I don't use it, but use x/y values when runnng JUCE_LIVE_CONSTANT  Must refactor this. 
    
    const int slider_size = 56;
    const int button_size = 60;
    
    int x = 130;
    
    int y = (getHeight() * 0.5) - (slider_size * 0.5);
    
    switch (mStyle)
    {
        case (mFxPanelStyle_Delay2):
        { 
            
            int y = (getHeight() * 0.4) - (slider_size * 0.4);
            
            const auto& image_TimeLink2Off =  ImageCache::getFromMemory(BinaryData::New_link_off_org__2_png, BinaryData::New_link_off_org__2_pngSize);
            const auto& image_TimeLink2On =  ImageCache::getFromMemory(BinaryData::New_link_on_org2_org_2_png, BinaryData::New_link_on_org2_org_2_pngSize);
            MFXParameterButton* timeLinkD2 =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2LinkTime],
                                   MFXParameterLabel[mFXParameter_Delay2LinkTime],
                                   image_TimeLink2Off, image_TimeLink2On, mProcessor, mFXParameter_Delay2LinkTime);
            timeLinkD2->setBounds(146, 132, 45, 45);
            
            
            
            addAndMakeVisible(timeLinkD2);
            mButtons.add(timeLinkD2);
            
           
            
            
            const auto& image_FeedbackLink2Off =  ImageCache::getFromMemory(BinaryData::New_link_off_org__2_png, BinaryData::New_link_off_org__2_pngSize);
            const auto& image_FeedbackLink2On =  ImageCache::getFromMemory(BinaryData::New_link_on_org2_org_2_png, BinaryData::New_link_on_org2_org_2_pngSize);
            MFXParameterButton* feedbackLinkD2 =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2LinkFeedback]
                                   ,MFXParameterLabel[mFXParameter_Delay2LinkFeedback],
                                   image_FeedbackLink2Off, image_FeedbackLink2On, mProcessor);
            feedbackLinkD2->setBounds(314, 132, 45, 45);
            
            addAndMakeVisible(feedbackLinkD2);
            mButtons.add(feedbackLinkD2);
            
            const auto& image_PingPong2Off =  ImageCache::getFromMemory(BinaryData::New_PingPong_Off_darker_org2_png, BinaryData::New_PingPong_Off_darker_org2_pngSize);
            const auto& image_PingPong2On =  ImageCache::getFromMemory(BinaryData::New_PingPong_On_darker_org2_png, BinaryData::New_PingPong_On_darker_org2_pngSize);
            MFXParameterButton* pingpongD2 =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2_Pingpong],
                                   MFXParameterLabel[mFXParameter_Delay2_Pingpong],
                                   image_PingPong2Off, image_PingPong2On, mProcessor, mFXParameter_Delay2_Pingpong);

        
            pingpongD2->setBounds(202, 121, button_size, button_size);
            addAndMakeVisible(pingpongD2);
            
            
            mButtons.add(pingpongD2);
            
            
            MFXParameterSlider* timeLeftD2 =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2TimeLeft],
                    MFXParameterLabel[mFXParameter_Delay2TimeLeft],
                    mFXParameter_Delay2TimeLeft, mProcessor);
            timeLeftD2->setBounds(x, y, slider_size, slider_size);
            
            
            
            addAndMakeVisible(timeLeftD2);
            mSliders.add(timeLeftD2);
            
            MFXParameterComboBox* delayTimeModeLeft = new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Delay2TimeModeLeft], mProcessor);
            
            
            delayTimeModeLeft->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeLeft])->getAllValueStrings() ,1);
            
            auto modeValue = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeModeLeft]);
            delayTimeModeLeft->setSelectedItemIndex(modeValue->load());
            
            int xTimeCombo = x + (slider_size);
            
            delayTimeModeLeft->setBounds(xTimeCombo + 15, y + 23, 80, 18);
            delayTimeModeLeft->setName("Time Mode");
            delayTimeModeLeft->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));

			mComboBoxes.add(delayTimeModeLeft);
            addAndMakeVisible(delayTimeModeLeft);
            
            
            x = x + (slider_size * 3.0);
            
            

            
            
            MFXParameterSlider* feedbackLeftD2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2LeftFeedback],
                                   MFXParameterLabel[mFXParameter_Delay2LeftFeedback],
                                   mFXParameter_Delay2LeftFeedback, mProcessor);
            feedbackLeftD2->setBounds(x, y, slider_size, slider_size);
            
            addAndMakeVisible(feedbackLeftD2);
            mSliders.add(feedbackLeftD2);
            x = x + (slider_size * 2.0);
            
            MFXParameterSlider* offsetD2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2_Offset],
                                   MFXParameterLabel[mFXParameter_Delay2_Offset],
                                   mFXParameter_Delay2_Offset, mProcessor);
            offsetD2->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(offsetD2);
            mSliders.add(offsetD2);
            
            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            
            MFXParameterSlider* timeRightD2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2TimeRight],
                                   MFXParameterLabel[mFXParameter_Delay2TimeRight],
                                   mFXParameter_Delay2TimeRight, mProcessor);
            timeRightD2->setBounds(x, y, slider_size, slider_size);

            addAndMakeVisible(timeRightD2);
            mSliders.add(timeRightD2);
            
            
            MFXParameterComboBox* delayTimeModeRight = new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Delay2TimeModeRight], mProcessor);
            
            
            delayTimeModeRight->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeRight])->getAllValueStrings() ,1);
            
            
            auto modeValueRight = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay2TimeModeRight]);
            delayTimeModeRight->setSelectedItemIndex(modeValueRight->load());
            
            delayTimeModeRight->setBounds(xTimeCombo + 15, y + 23, 80, 18);
            delayTimeModeRight->setName("Time Mode");
            delayTimeModeRight->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            
			mComboBoxes.add(delayTimeModeRight);
            addAndMakeVisible(delayTimeModeRight);
            x = x + (slider_size * 3.0);
            
            

            
            MFXParameterSlider* feedbackRightD2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2RightFeedback],
                                   MFXParameterLabel[mFXParameter_Delay2RightFeedback],
                                   mFXParameter_Delay2RightFeedback, mProcessor);
            feedbackRightD2->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(feedbackRightD2);
            mSliders.add(feedbackRightD2);
            x = x + (slider_size * 2);
            

            mDelay2TimeTransitionMode = std::make_unique<ArrowButton>("", 0.25, colour_11); 
            

            mDelay2TimeTransitionMode->onClick = [&]()
            {
                const bool fadeMode = mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay2])->getValue();
                
                if (fadeMode == 0.0f)
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay2])->setValueNotifyingHost(1.0f);
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay2])->setValueNotifyingHost(0.0f);
                
                setFxPanelStyle(mStyle);
            };


            mDelay2TimeTransitionMode->setBounds(431,
                140
                , 18, 15); 
            addAndMakeVisible(mDelay2TimeTransitionMode.get());   


            if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay2])->getValue() == 0.0f)
            {
                MFXParameterSlider* smoothingD2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2Repitch],
                                                                         MFXParameterLabel[mFXParameter_Delay2Repitch],
                                                                         mFXParameter_Delay2Repitch, mProcessor);
                smoothingD2->setBounds(x, y, slider_size, slider_size);
                addAndMakeVisible(smoothingD2);
                mSliders.add(smoothingD2);
                x = x + (slider_size * 2);
            }
            else
            {
                MFXParameterSlider* fade = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_FadeFrequency_Delay2],
                                                                  MFXParameterLabel[mFXParameter_FadeFrequency_Delay2],
                                                                  mFXParameter_FadeFrequency_Delay2, mProcessor);
                fade->setBounds(x, y, slider_size, slider_size);
                

                addAndMakeVisible(fade);
                mSliders.add(fade);
                x = x + (slider_size * 2);
            }

           
            
            
            
            
            mSliders[mSliders.indexOf(timeLeftD2) ]->onValueChange = [=] ()
            {
                if (timeLinkD2->getToggleState() == true)
                    mSliders[mSliders.indexOf(timeRightD2) ]->setValue (mSliders[mSliders.indexOf(timeLeftD2)]->getValue());
                
            };
            
            
            
            
            
            mSliders[mSliders.indexOf(timeRightD2) ]->onValueChange = [=] ()
            {
                if (timeLinkD2->getToggleState() == true)
                    mSliders[mSliders.indexOf(timeLeftD2) ]->setValue (mSliders[mSliders.indexOf(timeRightD2)]->getValue());
                
            };
            
            
            mSliders[mSliders.indexOf(feedbackLeftD2) ]->onValueChange = [=] ()
            {
                if (feedbackLinkD2->getToggleState() == true)
                    mSliders[mSliders.indexOf(feedbackRightD2) ]->setValue (mSliders[mSliders.indexOf(feedbackLeftD2)]->getValue());
                
            };
            
            mSliders[mSliders.indexOf(feedbackRightD2) ]->onValueChange = [=] ()
            {
                if (feedbackLinkD2->getToggleState() == true)
                    mSliders[mSliders.indexOf(feedbackLeftD2) ]->setValue (mSliders[mSliders.indexOf(feedbackRightD2)]->getValue());
                
            };
             
           
            
             
            
        } break;
            
        case (mFxPanelStyle_Chorus):
        {

            MFXParameterSlider* rate =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ChorusRate],
                                   MFXParameterLabel[mFXParameter_ChorusRate],
                                   mFXParameter_ChorusRate, mProcessor);
            rate->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(rate);
            mSliders.add(rate);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* depth =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ChorusDepth],
                                   MFXParameterLabel[mFXParameter_ChorusDepth],
                                   mFXParameter_ChorusDepth, mProcessor);
            depth->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(depth);
            mSliders.add(depth);
            x = x + (slider_size * 2);
            

            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            
            MFXParameterSlider* centerDelay =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ChorusWidth],
                                   MFXParameterLabel[mFXParameter_ChorusWidth],
                                   mFXParameter_ChorusWidth, mProcessor);
            centerDelay->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(centerDelay);
            mSliders.add(centerDelay);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* feedback =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ChorusFeedback],
                                   MFXParameterLabel[mFXParameter_ChorusFeedback],
                                   mFXParameter_ChorusFeedback,mProcessor);
            feedback->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(feedback);
            mSliders.add(feedback);
            x = x + (slider_size * 2);
            
            
            
            
        } break;
            
        case (mFxPanelStyle_Reverb):
        {
            
            MFXParameterSlider* size =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ReverbSize],
                                   MFXParameterLabel[mFXParameter_ReverbSize],
                                   mFXParameter_ReverbSize, mProcessor);
            size->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(size);
            mSliders.add(size);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* witdh =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ReverbWidth],
                                   MFXParameterLabel[mFXParameter_ReverbWidth],
                                   mFXParameter_ReverbWidth, mProcessor);
            witdh->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(witdh);
            mSliders.add(witdh);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* damping =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ReverbDamping],
                                   MFXParameterLabel[mFXParameter_ReverbDamping],
                                   mFXParameter_ReverbDamping, mProcessor);
            damping->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(damping);
            mSliders.add(damping);
            x = x + (slider_size * 2);
            
            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            

            x = x + (slider_size * 2);
            

            
            
            const auto& image_FreezeOff =  ImageCache::getFromMemory(BinaryData::New_Freeze_Off_org2_png, BinaryData::New_Freeze_Off_org2_pngSize);
            const auto& image_FreezeOn =  ImageCache::getFromMemory(BinaryData::New_Freeze_On_org2_png, BinaryData::New_Freeze_On_org2_pngSize);
            
            MFXParameterButton* freeze =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_ReverbFreeze],
                                   MFXParameterLabel[mFXParameter_ReverbFreeze], image_FreezeOff, image_FreezeOn, mProcessor);
            freeze->setBounds(247, y, button_size, button_size);
            
            addAndMakeVisible(freeze);
            mButtons.add(freeze);
            
            
           
            
           
            
        } break;
        
        case (mFxPanelStyle_Filter):
        {

            
            MFXParameterSlider* cutoff =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_FilterCutoff],
                                   MFXParameterLabel[mFXParameter_FilterCutoff],
                                   mFXParameter_FilterCutoff, mProcessor);
            cutoff->setBounds(x, y, slider_size, slider_size);
            
            addAndMakeVisible(cutoff);
            mSliders.add(cutoff);
            
        
            x = x + (slider_size * 2);
            
            
            MFXParameterSlider* resonans =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_FilterResonans],
                                   MFXParameterLabel[mFXParameter_FilterResonans],
                                   mFXParameter_FilterResonans, mProcessor);
            resonans->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(resonans);
            mSliders.add(resonans);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* drive =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_FilterDrive],
                                   MFXParameterLabel[mFXParameter_FilterDrive],
                                   mFXParameter_FilterDrive, mProcessor);
            drive->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(drive);
            mSliders.add(drive);
            x = x + (slider_size * 2);
            
            MFXParameterComboBox* filterType = new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_FilterType], mProcessor);
            
    
            filterType->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_FilterType])->getAllValueStrings(), 1);
            auto typeValue = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_FilterType]);
            filterType->setSelectedItemIndex(typeValue->load());
            filterType->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            
            filterType->setBounds(220,
                                   165,
                                   100, 25);
            filterType->setName("Type");

			mComboBoxes.add(filterType);
            addAndMakeVisible(filterType);
            

           
            
            
        } break;
            
            
        case (mFxPanelStyle_Phaser):
        {

            
            MFXParameterSlider* rate =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_PhaserRate],
                                   MFXParameterLabel[mFXParameter_PhaserRate],
                                   mFXParameter_PhaserRate, mProcessor);
            rate->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(rate);
            mSliders.add(rate);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* depth =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_PhaserDepth],
                                   MFXParameterLabel[mFXParameter_PhaserDepth],
                                   mFXParameter_PhaserDepth, mProcessor);
            depth->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(depth);
            mSliders.add(depth);
            x = x + (slider_size * 2);
            

            
            
            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            
            MFXParameterSlider* centerFrequency =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_PhaserCenterFrequency],
                                   MFXParameterLabel[mFXParameter_PhaserCenterFrequency],
                                   mFXParameter_PhaserCenterFrequency, mProcessor);
            centerFrequency->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(centerFrequency);
            mSliders.add(centerFrequency);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* feedback =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_PhaserFeedback],
                                   MFXParameterLabel[mFXParameter_PhaserFeedback],
                                   mFXParameter_PhaserFeedback, mProcessor);
            feedback->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(feedback);
            mSliders.add(feedback);
            x = x + (slider_size * 2);
            
            

        } break;
            
        case (mFxPanelStyle_BitCrusher):
        {

            
            MFXParameterSlider* resolution =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_BitRedux],
                                   MFXParameterLabel[mFXParameter_BitRedux],
                                   mFXParameter_BitRedux, mProcessor);
            resolution->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(resolution);
            mSliders.add(resolution);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* downsample =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_RateRedux],
                                   MFXParameterLabel[mFXParameter_RateRedux],
                                   mFXParameter_RateRedux, mProcessor);
            downsample->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(downsample);
            mSliders.add(downsample);
            x = x + (slider_size * 2);
            
            MFXParameterSlider* noise =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_NoiseAmount],
                                   MFXParameterLabel[mFXParameter_NoiseAmount],
                                   mFXParameter_NoiseAmount, mProcessor);
            noise->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(noise);
            mSliders.add(noise);
            
            
            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            
           
            
            
            
            
        } break;
            
        case (mFxPanelStyle_Delay1):
        {
            

            
            //     Random Dice Time
         
            const auto& image_RandomOff = ImageCache::getFromMemory(BinaryData::RandomTime_Dice_Off_png, BinaryData::RandomTime_Dice_Off_pngSize);
            const auto& image_RandomOn = ImageCache::getFromMemory(BinaryData::RandomTime_Dice_On_png, BinaryData::RandomTime_Dice_On_pngSize);
            mSetRandomValues = std::make_unique<ImageButton>(); 
            mSetRandomValues->setImages(false, false, true, image_RandomOff, 1.0f,
                Colour(0x0), image_RandomOff, 1.0f,
                Colour(0x0), image_RandomOn, 1.0f,
                Colour(0x0));

            mSetRandomValues->setBounds(100, 115, 45, 45);  

            addAndMakeVisible(mSetRandomValues.get());


            mSetRandomValues->onStateChange = [&]()
            {
                if (mSetRandomValues->isMouseOver())
                    mProcessor->getAccessToUIPersistentData().mHelperUIText = "Add random time/note values to both delay lines, parallel/serial mode and delay level balance (FX level). Notice if linked";
                else mProcessor->getAccessToUIPersistentData().mHelperUIText = "";
            };

            MFXPresetManager* presetManager = mProcessor->getPresetManager();
            

            mSetRandomValues->onClick = [=]() 
            {
               presetManager->setCurrentPresetName("New");
               juce::Random randomValue;
               
                // Delay time,mode 1
                
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeLeft])->setValueNotifyingHost(randomValue.nextFloat());
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeRight])->setValueNotifyingHost(randomValue.nextFloat());
                                
               
                if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeLeft])->getValue() > 0.0f)
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeLeft])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.25f, 1.0f));
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeLeft])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.01f, 0.25f));
                
                
                if(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeRight])->getValue() > 0.0f)
                   mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeRight])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.25f, 1.0f));
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeRight])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.01f, 0.25f));




                // Delay time,mode 2
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeLeft])->setValueNotifyingHost(randomValue.nextFloat());
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeRight])->setValueNotifyingHost(randomValue.nextFloat());

                if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeLeft])->getValue() > 0.0f)
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeLeft])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.25f, 1.0f));
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeLeft])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.01f, 0.25f));


                if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeModeRight])->getValue() > 0.0f)
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeRight])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.25f, 1.0f));
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2TimeRight])->setValueNotifyingHost(jmap(randomValue.nextFloat(), 0.01f, 0.25f));



                // other - parallel/serial, Delay 1/2 level
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2WetDry])->setValueNotifyingHost(randomValue.nextFloat());
                mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Parallel_Mode])->setValueNotifyingHost(randomValue.nextBool());
                
                mProcessor->setClearBuffer(true);

            };

           
            
            
            
            
            
            
            //--------------------------------------------------------------------------------------------------


            //     Random Dice All


            const auto& image_RandomAllOff = ImageCache::getFromMemory(BinaryData::Random_Dice_Off_png, BinaryData::Random_Dice_Off_pngSize);
            const auto& image_RandomAllOn = ImageCache::getFromMemory(BinaryData::Random_Dice_On_png, BinaryData::Random_Dice_On_pngSize);
            mSetRandomAllValues = std::make_unique<ImageButton>();
            mSetRandomAllValues->setImages(false, false, true, image_RandomAllOff, 1.0f,
                Colour(0x0), image_RandomAllOff, 1.0f,
                Colour(0x0), image_RandomAllOn, 1.0f,
                Colour(0x0));

            mSetRandomAllValues->setBounds(100, 145, 45, 45);

            // We have no presets in this show version of Time Razor. Clicking this dice cause app to crash. 
            //addAndMakeVisible(mSetRandomAllValues.get());


            mSetRandomAllValues->onStateChange = [&]()
            {
                if (mSetRandomAllValues->isMouseOver())
                    mProcessor->getAccessToUIPersistentData().mHelperUIText = "Use this to add random pre-existing preset values. Then use other dice to set random time related levels";
                else mProcessor->getAccessToUIPersistentData().mHelperUIText = "";
            };


            

            mSetRandomAllValues->onClick = [=]()
            {
                // We always get current max elements/preset size, and no out of bounds bugs
                int numberOfPresets = presetManager->getNumberOfPresets();
                
                juce::Random randomPresetValue;
                presetManager->loadPreset(randomPresetValue.nextInt(numberOfPresets));  
                
                presetManager->setCurrentPresetName("New");
                
                mProcessor->setClearBuffer(true);

            };

            //---------------------------------------------------------------------------------------------------

            
            int y = (getHeight() * 0.4) - (slider_size * 0.4);
            
            const auto& image_TimeLinkOff =  ImageCache::getFromMemory(BinaryData::New_link_off_org__2_png, BinaryData::New_link_off_org__2_pngSize);
            const auto& image_TimeLinkOn =  ImageCache::getFromMemory(BinaryData::New_link_on_org2_org_2_png, BinaryData::New_link_on_org2_org_2_pngSize);
            MFXParameterButton* timeLink =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1LinkTime],
                                   MFXParameterLabel[mFXParameter_Delay1LinkTime],
                                   image_TimeLinkOff, image_TimeLinkOn, mProcessor, mFXParameter_Delay1LinkTime);
            timeLink->setBounds(146,132,45, 45);
            
            addAndMakeVisible(timeLink);
            mButtons.add(timeLink);
            
            
            
            const auto& image_FeedbackOff =  ImageCache::getFromMemory(BinaryData::New_link_off_org__2_png, BinaryData::New_link_off_org__2_pngSize);
            const auto& image_FeedbackOn =  ImageCache::getFromMemory(BinaryData::New_link_on_org2_org_2_png, BinaryData::New_link_on_org2_org_2_pngSize);
            MFXParameterButton* feedbackLink =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1LinkFeedback],
                                   MFXParameterLabel[mFXParameter_Delay1LinkFeedback],
                                   image_FeedbackOff, image_FeedbackOn, mProcessor);
            feedbackLink->setBounds(314,132,45, 45);
            
            addAndMakeVisible(feedbackLink);
            mButtons.add(feedbackLink);
            
            
            const auto& image_PingPongOff =  ImageCache::getFromMemory(BinaryData::New_PingPong_Off_darker_org2_png, BinaryData::New_PingPong_Off_darker_org2_pngSize);
            const auto& image_PingPongOn =  ImageCache::getFromMemory(BinaryData::New_PingPong_On_darker_org2_png, BinaryData::New_PingPong_On_darker_org2_pngSize);
        
            MFXParameterButton* pingpong =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1Pingpong],
                                   MFXParameterLabel[mFXParameter_Delay1Pingpong],
                                   image_PingPongOff, image_PingPongOn, mProcessor, mFXParameter_Delay1Pingpong);
            pingpong->setBounds(202,121, button_size, button_size);
            
            addAndMakeVisible(pingpong);
            mButtons.add(pingpong);
            
            
            
            

        
            
            MFXParameterSlider* timeLeft =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1TimeLeft],
                                   MFXParameterLabel[mFXParameter_Delay1TimeLeft],
                                   mFXParameter_Delay1TimeLeft, mProcessor);
            timeLeft->setBounds(x, y, slider_size, slider_size);
            
            addAndMakeVisible(timeLeft);
            mSliders.add(timeLeft);
            
            
            
            
            
            MFXParameterComboBox* delayTimeModeMain1 =  new MFXParameterComboBox(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1TimeModeLeft], mProcessor);
            
            
            
            
            delayTimeModeMain1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeLeft])->getAllValueStrings() ,1);
            
            auto modeValue = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeModeLeft]);
            delayTimeModeMain1->setSelectedItemIndex(modeValue->load());
            
            int xTimeCombo = x + (slider_size);
            
            delayTimeModeMain1->setBounds(xTimeCombo + 15,
                                           y + 23,
                                           80, 18);
            delayTimeModeMain1->setName("Time Mode");
            
            delayTimeModeMain1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            
                        
            
			mComboBoxes.add(delayTimeModeMain1);
            addAndMakeVisible(delayTimeModeMain1);
            
            
            x = x + (slider_size * 3.0);
            
            
            
            
            MFXParameterSlider* feedbackLeft =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1LeftFeedback],
                                   MFXParameterLabel[mFXParameter_Delay1LeftFeedback],
                                   mFXParameter_Delay1LeftFeedback, mProcessor);
            feedbackLeft->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(feedbackLeft);
            mSliders.add(feedbackLeft);
            x = x + (slider_size * 2.0);
            
            MFXParameterSlider* offset =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1_Offset],
                                   MFXParameterLabel[mFXParameter_Delay1_Offset],
                                   mFXParameter_Delay1_Offset, mProcessor);

            offset->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(offset);
            mSliders.add(offset);
            
            x = 130;
            y = (getHeight() * 0.9) - (slider_size);
            
            MFXParameterSlider* timeRight =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1TimeRight],
                                   MFXParameterLabel[mFXParameter_Delay1TimeRight],
                                   mFXParameter_Delay1TimeRight, mProcessor);
            timeRight->setBounds(x, y, slider_size, slider_size);
            
            addAndMakeVisible(timeRight);
            mSliders.add(timeRight);
            
            
            MFXParameterComboBox* delayTimeModeMain2 = new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Delay1TimeModeRight], mProcessor);
            
            
            delayTimeModeMain2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1TimeModeRight])->getAllValueStrings() ,1);
            
            auto modeValueRight = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Delay1TimeModeRight]);
            delayTimeModeMain2->setSelectedItemIndex(modeValueRight->load());
            
            delayTimeModeMain2->setBounds(xTimeCombo + 15,
                                           y + 23,
                                           80,
                                           18);
            delayTimeModeMain2->setName("Time Mode");
            delayTimeModeMain2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            
            
			mComboBoxes.add(delayTimeModeMain2);
            
            addAndMakeVisible(delayTimeModeMain2);
            x = x + (slider_size * 3.0);
            
            

            
            MFXParameterSlider* feedbackRight =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1RightFeedback],
                                   MFXParameterLabel[mFXParameter_Delay1RightFeedback],
                                   mFXParameter_Delay1RightFeedback, mProcessor);
            feedbackRight->setBounds(x, y, slider_size, slider_size);
            addAndMakeVisible(feedbackRight);
            mSliders.add(feedbackRight);
            x = x + (slider_size * 2);
            

            mDelay1TimeTransitionMode = std::make_unique<ArrowButton>("", 0.25, colour_11);  // not visieble yet

            mDelay1TimeTransitionMode->onClick = [&]()
            {

                
                const bool fadeMode = mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay1])->getValue();
                
                if (fadeMode == 0.0f)
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay1])->setValueNotifyingHost(1.0f);
                else
                    mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay1])->setValueNotifyingHost(0.0f);
                
                setFxPanelStyle(mStyle);
                
            };


            mDelay1TimeTransitionMode->setBounds(431, 140, 18, 15);
            addAndMakeVisible(mDelay1TimeTransitionMode.get());
            
            
            if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_IsFadeMode_Delay1])->getValue() == 0.0f)
            {
              
                MFXParameterSlider* smoothing =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay1_Repitch],
                MFXParameterLabel[mFXParameter_Delay1_Repitch],
                mFXParameter_Delay1_Repitch, mProcessor);
                smoothing->setBounds(x, y, slider_size, slider_size);
                addAndMakeVisible(smoothing);
                mSliders.add(smoothing);
                x = x + (slider_size * 2);

            }
            else
            {
                
                MFXParameterSlider* fade = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_FadeFrequency_Delay1],
                                           MFXParameterLabel[mFXParameter_FadeFrequency_Delay1], mFXParameter_FadeFrequency_Delay1, mProcessor);
                fade->setBounds(x, y, slider_size, slider_size);
                addAndMakeVisible(fade);
                mSliders.add(fade);
                x = x + (slider_size * 2);
            }
            
            
            mSliders[mSliders.indexOf(timeLeft) ]->onValueChange = [=] ()
            {
                if (timeLink->getToggleState() == true)
                    mSliders[mSliders.indexOf(timeRight) ]->setValue (mSliders[mSliders.indexOf(timeLeft)]->getValue());
            };
                
           
            
            mSliders[mSliders.indexOf(timeRight) ]->onValueChange = [=] ()
            {
                if (timeLink->getToggleState() == true)
                    mSliders[mSliders.indexOf(timeLeft) ]->setValue (mSliders[mSliders.indexOf(timeRight)]->getValue());
                       
            };
            
            
            mSliders[mSliders.indexOf(feedbackLeft) ]->onValueChange = [=] ()
            {
                if (feedbackLink->getToggleState() == true)
                    mSliders[mSliders.indexOf(feedbackRight) ]->setValue (mSliders[mSliders.indexOf(feedbackLeft)]->getValue());
                
            };
            
            mSliders[mSliders.indexOf(feedbackRight) ]->onValueChange = [=] ()
            {
                if (feedbackLink->getToggleState() == true)
                    mSliders[mSliders.indexOf(feedbackLeft) ]->setValue (mSliders[mSliders.indexOf(feedbackRight)]->getValue());
                
            };
            
            
        } break;
            
            
        case (mFxPanelStyle_Advanced):
        {
        
            
           
            
            x = 60;
            
            y = 75;
            
            
            const int horisontalSlider_width = 50;
            const int horisontalSlider_height = 20;
            const int imageButton_width = 45;
            const int imageButton_height = 29;
            const int imageButton_x = 171;
            const int imageButton_y = 135;
            
            
            MFXParameterSlider* sensitivity =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Sensitivity],
                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Sensitivity],
                                   mFXParameter_Advanced_EnvF_Sensitivity, mProcessor);
            sensitivity->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            sensitivity->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            sensitivity->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(sensitivity);
            mFXPanelHorisontalSliders.add(sensitivity);
            
            x += slider_size;
            
            MFXParameterSlider* Attack =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Attack],
                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Attack],
                                   mFXParameter_Advanced_EnvF_Attack, mProcessor);
            Attack->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            Attack->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            Attack->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(Attack);
            mFXPanelHorisontalSliders.add(Attack);
            
            x += slider_size;
            
            MFXParameterSlider* release =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Release],
                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Release],
                                   mFXParameter_Advanced_EnvF_Release, mProcessor);
            release->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            release->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            release->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(release);
            mFXPanelHorisontalSliders.add(release);
            
            
            x += slider_size;
            int yBox = 100;

            MFXParameterComboBox* envTarget1 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Target1], mProcessor);
            
            envTarget1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_EnvF_Target1])->getAllValueStrings() ,1);
            
            auto targetValue = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Target1]);
            envTarget1->setSelectedItemIndex(targetValue->load());
            
            
            
            envTarget1->setBounds(x, yBox, 107, 18);
            
            envTarget1->getLocalBounds().reduce(0, 10);
            
    
            envTarget1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            
            envTarget1->setJustificationType(4);
            
            
            mComboBoxes.add(envTarget1);
            addAndMakeVisible(envTarget1);
            
            
            
            
            
            
            MFXParameterSlider* envTargetAmount1 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Target1_Amount ],
                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Target1_Amount],
                                   mFXParameter_Advanced_EnvF_Target1_Amount, mProcessor);
            envTargetAmount1->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            envTargetAmount1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            envTargetAmount1->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(envTargetAmount1);
            mFXPanelHorisontalSliders.add(envTargetAmount1);
            
            
            
            x += 115;
            
            MFXParameterComboBox* envTarget2 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Target2], mProcessor);
            
            envTarget2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_EnvF_Target2])->getAllValueStrings() ,1);
            
            auto target2Value = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_EnvF_Target2]);
            envTarget2->setSelectedItemIndex(target2Value->load());
            
            
            
            envTarget2->setBounds(x,
                                  yBox,
                                  107, 18);
            
            envTarget2->setJustificationType(4);
            envTarget2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(envTarget2);
            mComboBoxes.add(envTarget2);
            
            
            
            
            
            MFXParameterSlider* envTargetAmount2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_EnvF_Target2_Amount ],
                                   MFXParameterLabel[mFXParameter_Advanced_EnvF_Target2_Amount],
                                   mFXParameter_Advanced_EnvF_Target2_Amount, mProcessor);
            envTargetAmount2->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            envTargetAmount2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            envTargetAmount2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(envTargetAmount2);
            mFXPanelHorisontalSliders.add(envTargetAmount2);
            
            
            
            /* LFO 1 -----------------------------------------------------------------------------------------------*/
            
            x = 51;  // X_LFO1
            y = 141;//  Y_LFO1
            yBox = 140;
            
            if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Sync])->getValue() == 0)
            {
            
                MFXParameterSlider* rate1 =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Rate],
                                       MFXParameterLabel[mFXParameter_Advanced_LFO1_Rate],
                                       mFXParameter_Advanced_LFO1_Rate, mProcessor);
                rate1->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
                rate1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
                rate1->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
                addAndMakeVisible(rate1);
                mFXPanelHorisontalSliders.add(rate1);
                
            }
            
            else
            {
            
                MFXParameterSlider* note1 =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Note],
                                       MFXParameterLabel[mFXParameter_Advanced_LFO1_Note],
                                       mFXParameter_Advanced_LFO1_Note, mProcessor);
                note1->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
                note1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
                
                note1->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
                addAndMakeVisible(note1);
                mFXPanelHorisontalSliders.add(note1);
                juce::StringArray notes = mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Note])->getAllValueStrings();
                
//                note1->onValueChange = [=]()
//                {
//                    // this will not be called unless manually turn knob - not if load preset that reads/set value from file
//                    mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Advanced_LFO1_Note] = notes.getReference((int)note1->getValue());
//                    
//                   
//                };
            
            }
            
            
            
            
            
            
            x += 50; // slider_size;
            
            MFXParameterComboBox* wavType1 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Type], mProcessor);
            
            wavType1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Type])->getAllValueStrings() ,1);
            
            auto wavSelectInit1 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Type]);
            wavType1->setSelectedItemIndex(wavSelectInit1->load());
            
            
            
            wavType1->setBounds(x,
                                  yBox,
                                  70, 18);
            
            wavType1->setJustificationType(4);
            wavType1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(wavType1);
            mComboBoxes.add(wavType1);
            
            // sync button
            
            
            
            const auto& image_lfoHz =  ImageCache::getFromMemory(BinaryData::New_Sync_option_Hz_org2_png, BinaryData::New_Sync_option_Hz_org2_pngSize);
            const auto& image_lfoSync =  ImageCache::getFromMemory(BinaryData::New_Sync_option_note_org2_png, BinaryData::New_Sync_option_note_org2_pngSize);
            MFXParameterButton* lfo1Freq =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Sync],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Sync],
                                   image_lfoHz, image_lfoSync, mProcessor);
            lfo1Freq->setBounds(178, 141, imageButton_width, imageButton_height);
            
            addAndMakeVisible(lfo1Freq);
            mButtons.add(lfo1Freq);

            lfo1Freq->onClick = [&]()
            {
                
                mFXPanelHorisontalSliders.clear();
                mButtons.clear();
                mComboBoxes.clear();
                // If user click button, that means note rate should be used, therefor repaint to draw note selector
                setFxPanelStyle(mStyle);
                

            };
            
            //   Retrigger button

            const auto& image_retriggerOff = ImageCache::getFromMemory(BinaryData::Retrigger_Off_png, BinaryData::Retrigger_Off_pngSize);
            const auto& image_retriggerOn = ImageCache::getFromMemory(BinaryData::Retrigger_On_png, BinaryData::Retrigger_On_pngSize);
            MFXParameterButton* lfo1Retrigger = new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_LFO1_Retrigger],
                                                                       MFXParameterLabel[mFXParameter_Midi_LFO1_Retrigger],
                                                                       image_retriggerOff, image_retriggerOn, mProcessor, mFXParameter_Midi_LFO1_Retrigger);
            
            lfo1Retrigger->setBounds(215, 141, imageButton_width, imageButton_height);

            addAndMakeVisible(lfo1Retrigger);
            mButtons.add(lfo1Retrigger);

            




            //----------------------------
            
            x = 57;
            y = 170;
            yBox = 165;
            
            MFXParameterComboBox* lfoTarget1 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Target1], mProcessor);
            
            lfoTarget1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Target1])->getAllValueStrings() ,1);
            
            auto lfoSelectInit1 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Target1]);
            lfoTarget1->setSelectedItemIndex(lfoSelectInit1->load());
            
            
            
            lfoTarget1->setBounds(x,
                                  yBox,
                                  107, 18);
            
            lfoTarget1->setJustificationType(4);
            lfoTarget1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(lfoTarget1);
            mComboBoxes.add(lfoTarget1);

            x += 108;
            
            
            
            MFXParameterSlider* lfoTargetAmount1 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Target1_Amount],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Target1_Amount],
                                   mFXParameter_Advanced_LFO1_Target1_Amount, mProcessor);
            lfoTargetAmount1->setBounds(x,
                                        y,
                                        horisontalSlider_width,
                                        horisontalSlider_height);
            
            lfoTargetAmount1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            lfoTargetAmount1->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
            
            addAndMakeVisible(lfoTargetAmount1);
            mFXPanelHorisontalSliders.add(lfoTargetAmount1);
            
            
            
           
            x = 57;
            y = 200;
            yBox = 194;
            
            
            
            MFXParameterComboBox* lfoTarget2 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Target2], mProcessor);
            
            lfoTarget2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Target2])->getAllValueStrings() ,1);
            
            auto lfoSelectInit2 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO1_Target2]);
            lfoTarget2->setSelectedItemIndex(lfoSelectInit2->load());
            
            
            
            lfoTarget2->setBounds(x,
                                  yBox,
                                  107, 18);
            
            lfoTarget2->setJustificationType(4);
            lfoTarget2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(lfoTarget2);
            mComboBoxes.add(lfoTarget2);
            
            
            
            x += 108;

            
            MFXParameterSlider* lfoTargetAmount2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO1_Target2_Amount],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO1_Target2_Amount],
                                   mFXParameter_Advanced_LFO1_Target2_Amount, mProcessor);
            lfoTargetAmount2->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            lfoTargetAmount2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            lfoTargetAmount2->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
            addAndMakeVisible(lfoTargetAmount2);
            mFXPanelHorisontalSliders.add(lfoTargetAmount2);
            
            
            
            
            

            
            
            
            
            
            /* LFO 2 -----------------------------------------------------------------------------------------------*/
            
            
            
            
            
            
            
            x = 231; // X_LFO2
            y = 141; //  Y_LFO2
            yBox = 140;
            
            if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO2_Sync])->getValue() == 0)
            {
            
                MFXParameterSlider* rate2 =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Rate],
                                       MFXParameterLabel[mFXParameter_Advanced_LFO2_Rate],
                                       mFXParameter_Advanced_LFO2_Rate, mProcessor);
                rate2->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
                rate2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
                
                rate2->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
                addAndMakeVisible(rate2);
                mFXPanelHorisontalSliders.add(rate2);
            
            }
            else
            {
                
                MFXParameterSlider* note2 =
                new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Note],
                                       MFXParameterLabel[mFXParameter_Advanced_LFO2_Note],
                                       mFXParameter_Advanced_LFO2_Note, mProcessor);
                note2->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
                note2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
                
                note2->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
                addAndMakeVisible(note2);
                mFXPanelHorisontalSliders.add(note2);
                juce::StringArray notes2 = mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO1_Note])->getAllValueStrings();
                
                note2->onValueChange = [=]()
                {
                    mProcessor->getAccessToUIPersistentData().mParameterValueText[mFXParameter_Advanced_LFO2_Note] = notes2.getReference((int)note2->getValue());
                    
                    
                };
                
            }
            
            
            
            
            x += 50;
            
            MFXParameterComboBox* wavType2 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Type], mProcessor);
            
            wavType2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO2_Type])->getAllValueStrings() ,1);
            
            auto wavSelectInit2 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Type]);
            wavType2->setSelectedItemIndex(wavSelectInit2->load());
            
            
            
            wavType2->setBounds(x,
                                yBox,
                                70, 18);
            
            wavType2->setJustificationType(4);
            wavType2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(wavType2);
            mComboBoxes.add(wavType2);
            
            // sync buttons
            
            
            const auto& image_lfo2Hz =  ImageCache::getFromMemory(BinaryData::New_Sync_option_Hz_org2_png, BinaryData::New_Sync_option_Hz_org2_pngSize);
            const auto& image_lfo2Sync =  ImageCache::getFromMemory(BinaryData::New_Sync_option_note_org2_png, BinaryData::New_Sync_option_note_org2_pngSize);
            MFXParameterButton* lfo2Freq =
            new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Sync],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Sync],
                                   image_lfo2Hz, image_lfo2Sync, mProcessor);
            lfo2Freq->setBounds(356,141,
                             imageButton_width,
                             imageButton_height);
            
            addAndMakeVisible(lfo2Freq);
            mButtons.add(lfo2Freq);

            lfo2Freq->onClick = [&]()
            {
                
                mFXPanelHorisontalSliders.clear();
                mButtons.clear();
                mComboBoxes.clear();
                // If user click button, that means note rate should be used, therefor repaint to draw note selector
                setFxPanelStyle(mStyle);
            };

            
            //   Retrigger button

            const auto& image_retriggerOff2 = ImageCache::getFromMemory(BinaryData::Retrigger_Off_png, BinaryData::Retrigger_Off_pngSize);
            const auto& image_retriggerOn2 = ImageCache::getFromMemory(BinaryData::Retrigger_On_png, BinaryData::Retrigger_On_pngSize);
            
            MFXParameterButton* lfo2Retrigger = new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_LFO2_Retrigger],
                                                                       MFXParameterLabel[mFXParameter_Midi_LFO2_Retrigger],
                                                                       image_retriggerOff2, image_retriggerOn2, mProcessor, mFXParameter_Midi_LFO2_Retrigger);
            
            lfo2Retrigger->setBounds(393, 141, imageButton_width, imageButton_height);

            addAndMakeVisible(lfo2Retrigger);
            mButtons.add(lfo2Retrigger);
            
            
            
            x = 240;
            y = 170;
            yBox = 165;
            
            
            MFXParameterComboBox* lfo2Target1 = new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Target1], mProcessor);
            
            lfo2Target1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO2_Target1])->getAllValueStrings() ,1);
            
            auto lfo2SelectInit1 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Target1]);
                                                                               lfo2Target1->setSelectedItemIndex(lfo2SelectInit1->load());
            
            
            
            lfo2Target1->setBounds(236, yBox, 107, 18);
            
            lfo2Target1->setJustificationType(4);
            lfo2Target1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(lfo2Target1);
            mComboBoxes.add(lfo2Target1);
            
            
            
            
            
            MFXParameterSlider* lfo2TargetAmount1 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Target1_Amount],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Target1_Amount],
                                   mFXParameter_Advanced_LFO2_Target1_Amount, mProcessor);
            lfo2TargetAmount1->setBounds(342, y, horisontalSlider_width, horisontalSlider_height);
            
            lfo2TargetAmount1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            lfo2TargetAmount1->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
            addAndMakeVisible(lfo2TargetAmount1);
            mFXPanelHorisontalSliders.add(lfo2TargetAmount1);
            
            
            
            x = 240;
            y = 200;
            yBox = 194;
            
            
            
            MFXParameterComboBox* lfo2Target2 =
            new MFXParameterComboBox( mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Target2], mProcessor);
            
            lfo2Target2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Advanced_LFO2_Target2])->getAllValueStrings() ,1);
            
            auto lfo2SelectInit2 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Advanced_LFO2_Target2]);
            lfo2Target2->setSelectedItemIndex(lfo2SelectInit2->load());
            
            
            
            lfo2Target2->setBounds(236,
                                  yBox,
                                  107, 18);
            
            lfo2Target2->setJustificationType(4);
            lfo2Target2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(lfo2Target2);
            mComboBoxes.add(lfo2Target2);
            
            
            
            
            
            
            MFXParameterSlider* lfo2TargetAmount2 =
            new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Advanced_LFO2_Target2_Amount],
                                   MFXParameterLabel[mFXParameter_Advanced_LFO2_Target2_Amount],
                                   mFXParameter_Advanced_LFO2_Target2_Amount, mProcessor);
            lfo2TargetAmount2->setBounds(342, y, horisontalSlider_width, horisontalSlider_height);
            lfo2TargetAmount2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
            
            lfo2TargetAmount2->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
            addAndMakeVisible(lfo2TargetAmount2);
            mFXPanelHorisontalSliders.add(lfo2TargetAmount2);
            
            
            
            
        }
            break;

        case (mFxPanelStyle_MIDI_ADSR):
        {

            


            x = 60;

            y = 105;


            const int horisontalSlider_width = 50;
            const int horisontalSlider_height = 20;
            const int imageButton_width = 45;
            const int imageButton_height = 29;
            const int imageButton_x = 171;
            const int imageButton_y = 135;



            const auto& image_Midi_Off = ImageCache::getFromMemory(BinaryData::Midi_Off_png, BinaryData::Midi_Off_pngSize);
            const auto& image_Midi_On = ImageCache::getFromMemory(BinaryData::Midi_On_png, BinaryData::Midi_On_pngSize);
            mMidi = std::make_unique<ImageButton>();
            mMidi->setImages(false, false, true, image_Midi_Off, 1.0f,
                Colour(0x0), image_Midi_Off, 1.0f,
                Colour(0x0), image_Midi_On, 1.0f,
                Colour(0x0));

            mMidi->setBounds(100, 189, 45, 45);
            

            addAndMakeVisible(mMidi.get());



            MFXParameterSlider* attack = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Attack],
                                                                MFXParameterLabel[mFXParameter_Midi_ADSR_Attack],
                                                                mFXParameter_Midi_ADSR_Attack, mProcessor);
            attack->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            attack->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            attack->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(attack);
            mFXPanelHorisontalSliders.add(attack);

            x += slider_size;
            
            MFXParameterSlider* decay = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Decay],
                                                               MFXParameterLabel[mFXParameter_Midi_ADSR_Decay],
                                                               mFXParameter_Midi_ADSR_Decay, mProcessor);
            decay->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            decay->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            decay->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(decay);
            mFXPanelHorisontalSliders.add(decay);

            x += slider_size;

            MFXParameterSlider* sustain = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Sustain],
                                                                 MFXParameterLabel[mFXParameter_Midi_ADSR_Sustain],
                                                                 mFXParameter_Midi_ADSR_Sustain, mProcessor);
            sustain->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            sustain->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            sustain->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(sustain);
            mFXPanelHorisontalSliders.add(sustain);

            x += slider_size;

            MFXParameterSlider* release = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Release],
                                                                 MFXParameterLabel[mFXParameter_Midi_ADSR_Release],
                                                                 mFXParameter_Midi_ADSR_Release, mProcessor);
            release->setBounds(x, y, horisontalSlider_width, horisontalSlider_height);
            release->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            release->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(release);
            mFXPanelHorisontalSliders.add(release);


            x = 60;
            y = 130;

            MFXParameterComboBox* envTarget1 = new MFXParameterComboBox(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Target1], mProcessor);

            envTarget1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Midi_ADSR_Target1])->getAllValueStrings(), 1);

            auto targetValue = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target1]);
            envTarget1->setSelectedItemIndex(targetValue->load());



            envTarget1->setBounds(65, y, 107, 18);

            envTarget1->getLocalBounds().reduce(0, 10);


            envTarget1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));

            envTarget1->setJustificationType(4);


            mComboBoxes.add(envTarget1);
            addAndMakeVisible(envTarget1);


            



            MFXParameterSlider* envTargetAmount1 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Target1_Amount],
                                                   MFXParameterLabel[mFXParameter_Midi_ADSR_Target1_Amount],
                                                   mFXParameter_Midi_ADSR_Target1_Amount, mProcessor);
            envTargetAmount1->setBounds(172, y, horisontalSlider_width, horisontalSlider_height);
            envTargetAmount1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            envTargetAmount1->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(envTargetAmount1);
            mFXPanelHorisontalSliders.add(envTargetAmount1);



           

            MFXParameterComboBox* envTarget2 = new MFXParameterComboBox(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Target2], mProcessor);

            envTarget2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Midi_ADSR_Target2])->getAllValueStrings(), 1);

            auto target2Value = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR_Target2]);
            envTarget2->setSelectedItemIndex(target2Value->load());



            envTarget2->setBounds(65,
                y+25,
                107, 18);

            envTarget2->setJustificationType(4);
            envTarget2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(envTarget2);
            mComboBoxes.add(envTarget2);





            MFXParameterSlider* envTargetAmount2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR_Target2_Amount],
                                                   MFXParameterLabel[mFXParameter_Midi_ADSR_Target2_Amount],
                                                   mFXParameter_Midi_ADSR_Target2_Amount, mProcessor);
            envTargetAmount2->setBounds(172, y+25, horisontalSlider_width, horisontalSlider_height);
            envTargetAmount2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            envTargetAmount2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(envTargetAmount2);
            mFXPanelHorisontalSliders.add(envTargetAmount2);



            //---------------------ADSR 2
            x = 60;
            y = 105;

            MFXParameterSlider* attack2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Attack],
                                          MFXParameterLabel[mFXParameter_Midi_ADSR2_Attack], mFXParameter_Midi_ADSR2_Attack, mProcessor);
            attack2->setBounds(x + 220, y, horisontalSlider_width, horisontalSlider_height);
            attack2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            attack2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(attack2);
            mFXPanelHorisontalSliders.add(attack2);

            x += slider_size;

            MFXParameterSlider* decay2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Decay],
                                         MFXParameterLabel[mFXParameter_Midi_ADSR2_Decay], mFXParameter_Midi_ADSR2_Decay, mProcessor);
            decay2->setBounds(x + 220, y, horisontalSlider_width, horisontalSlider_height);
            decay2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            decay2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(decay2);
            mFXPanelHorisontalSliders.add(decay2);

            x += slider_size;

            MFXParameterSlider* sustain2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Sustain],
                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Sustain], mFXParameter_Midi_ADSR2_Sustain, mProcessor);
            sustain2->setBounds(x + 220, y, horisontalSlider_width, horisontalSlider_height);
            sustain2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            sustain2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(sustain2);
            mFXPanelHorisontalSliders.add(sustain2);

            x += slider_size;

            MFXParameterSlider* release2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Release],
                                           MFXParameterLabel[mFXParameter_Midi_ADSR2_Release], mFXParameter_Midi_ADSR2_Release, mProcessor);
            release2->setBounds(x + 220, y, horisontalSlider_width, horisontalSlider_height);
            release2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            release2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(release2);
            mFXPanelHorisontalSliders.add(release2);


            
            

            MFXParameterComboBox* env2Target1 = new MFXParameterComboBox(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Target1], mProcessor);

            env2Target1->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Midi_ADSR2_Target1])->getAllValueStrings(), 1);

            auto targetValue1 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target1]);
            env2Target1->setSelectedItemIndex(targetValue1->load());



            env2Target1->setBounds(285, 130, 107, 18);

            env2Target1->getLocalBounds().reduce(0, 10);


            env2Target1->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));

            env2Target1->setJustificationType(4);


            mComboBoxes.add(env2Target1);
            addAndMakeVisible(env2Target1);






            MFXParameterSlider* env2TargetAmount1 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Target1_Amount],
                                                    MFXParameterLabel[mFXParameter_Midi_ADSR2_Target1_Amount], mFXParameter_Midi_ADSR2_Target1_Amount, mProcessor);
            env2TargetAmount1->setBounds(392, 130, horisontalSlider_width, horisontalSlider_height);
            env2TargetAmount1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            env2TargetAmount1->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(env2TargetAmount1);
            mFXPanelHorisontalSliders.add(env2TargetAmount1);





            MFXParameterComboBox* env2Target2 = new MFXParameterComboBox(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Target2], mProcessor);

            env2Target2->addItemList(mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Midi_ADSR2_Target2])->getAllValueStrings(), 1);

            auto target2Value2 = mProcessor->parameters.getRawParameterValue(MFXParameterID[mFXParameter_Midi_ADSR2_Target2]);
            env2Target2->setSelectedItemIndex(target2Value2->load());



            env2Target2->setBounds(285,
                155,
                107, 18);

            env2Target2->setJustificationType(4);
            env2Target2->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
            addAndMakeVisible(env2Target2);
            mComboBoxes.add(env2Target2);





            MFXParameterSlider* env2TargetAmount2 = new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Midi_ADSR2_Target2_Amount],
                MFXParameterLabel[mFXParameter_Midi_ADSR2_Target2_Amount],
                mFXParameter_Midi_ADSR2_Target2_Amount, mProcessor);
            env2TargetAmount2->setBounds(392, 155, horisontalSlider_width, horisontalSlider_height);
            env2TargetAmount2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);

            env2TargetAmount2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
            addAndMakeVisible(env2TargetAmount2);
            mFXPanelHorisontalSliders.add(env2TargetAmount2);
            

            





        } break;

        default:
        case (mFxPanelStyle_TotalNumStyles):
        {
            // This should not be happening
            jassertfalse;
        } break;
    }
    
    
    repaint();
}

// comboBox in center panel
void MFXFxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    FxPanelStyle style = static_cast<FxPanelStyle>( comboBoxThatHasChanged->getSelectedItemIndex() ); // get index to style
    
    setFxPanelStyle(style);
    
    
}

void MFXFxPanel::timerCallback()
{
    // this fix problem when a preset is loaded and fxpanel, eg. delay 1, is unchanged. comboBoxChanged is not called - not setting setFxPanelStyle
    if (mProcessor->getAccessToUIPersistentData().mIsPresetLoaded == 1.0)
    {
        setFxPanelStyle(mStyle);
        mProcessor->getAccessToUIPersistentData().mIsPresetLoaded = false;
    }


    if (mProcessor->isMidiOn() && mMidi != nullptr)
    {
        mMidi->triggerClick();

    }
    repaint();
}



void MFXFxPanel::buttonClicked (Button* currentButton)
{
    

    
    if (currentButton->getToggleState() && currentButton->getName() == "Ducking")
    {  
        mGlobalSliders.clear();
        createDuckingUI();   
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "Ducking")
    {
        mGlobalSliders.clear();
    }
    
    else if (currentButton->getToggleState() && currentButton->getName() == "Mix")
    {
        mGlobalSliders.clear();
        createMixUI();
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "Mix")
    {
        mGlobalSliders.clear();
        for(auto& it : mGlobalButtons)
        {
            if (it->getName() == "MS")
                it->setVisible(false);
        }
    }
    
    else if (currentButton->getToggleState() && currentButton->getName() == "FX Mix")
    {
        mGlobalSliders.clear();
        createFXLevelsUI();
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "FX Mix")
    {
        mGlobalSliders.clear();
    }
        
    else if (currentButton->getToggleState() && currentButton->getName() == "Shaper")
    {
        mGlobalSliders.clear();
        createCutModeUI();
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "Shaper")
    {
        mGlobalSliders.clear();
    }
    
    
    if (currentButton->getToggleState() && currentButton->getName() == "Parallel")
    {
        // APVTS is updated after button state set/ button listener call. creatCutModeUI() will not get correct true/false parallelMode value, so GUI will not build correct.
        // Instead use this variable.
        // Global Variable in usedParameters.h. Needed because UI is destroyed/created when close/open vst window in DAW
        
        mProcessor->getAccessToUIPersistentData().mIsParallelModeOn = true;
        
        if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->getValue())
        {
            mGlobalSliders.clear();
            createCutModeUI();
        }
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "Parallel")
    {
        // APVTS is updated after button state set/ button listener call. creatCutModeUI() will not get correct true/false parallelMode value, so GUI will not build correct.
        // Instead use this variable.
        // Global Variable in usedParameters.h. Needed because UI is destroyed/created when close/open vst window. No need to set isParallelModeOn in both
        // Cut_Mode/ducking_Mode.
        
        mProcessor->getAccessToUIPersistentData().mIsParallelModeOn = false;
        
        if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Cut_Mode])->getValue())
        {
            mGlobalSliders.clear();
            createCutModeUI();
        }
    }
    
    if (currentButton->getToggleState() && currentButton->getName() == "Parallel")
    {
        
        if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->getValue())
        {
            mGlobalSliders.clear();
            createDuckingUI();
        }
    }
    
    else if (!currentButton->getToggleState() && currentButton->getName() == "Parallel")
    {
        
        if (mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Ducking_Mode])->getValue())
        {
            mGlobalSliders.clear();
            createDuckingUI();
        }
    }
    
    if (!(currentButton->getName() == "Mix") && !(currentButton->getName() == "Parallel"))
    {
        for(auto& it : mGlobalButtons)
        {
            if (it->getName() == "MS")
                it->setVisible(false);
        }
    }
    
    

        
    

    
}

void MFXFxPanel::createDuckingUI()
{
    const int horisontalSlider_width = 50;
    const int horisontalSlider_height = 20;
    const int y = 90;
    const int x = (!mProcessor->getAccessToUIPersistentData().mIsParallelModeOn ) ? 178 : 46;
    
    
    
    MFXParameterSlider* threshold =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Threshold],
                           MFXParameterLabel[mFXParameter_Ducking_Threshold],
                           mFXParameter_Ducking_Threshold, mProcessor);
    threshold->setBounds(((!mProcessor->getAccessToUIPersistentData().mIsParallelModeOn ) ? 178 : 46), y - 78, horisontalSlider_width, horisontalSlider_height);
    threshold->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    threshold->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
    addAndMakeVisible(threshold);
    mGlobalSliders.add(threshold);
    
    
    MFXParameterSlider* amount =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Amount],
                           MFXParameterLabel[mFXParameter_Ducking_Amount],
                           mFXParameter_Ducking_Amount, mProcessor);
    amount->setBounds(x + horisontalSlider_width, y - 78, horisontalSlider_width, horisontalSlider_height);
    amount->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    amount->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
    addAndMakeVisible(amount);
    mGlobalSliders.add(amount);
    
    
    MFXParameterSlider* attack =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Attack],
                           MFXParameterLabel[mFXParameter_Ducking_Attack],
                           mFXParameter_Ducking_Attack, mProcessor);
    attack->setBounds(x + (2 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    attack->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    attack->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
    addAndMakeVisible(attack);
    mGlobalSliders.add(attack);
    
    MFXParameterSlider* release =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Release],
                           MFXParameterLabel[mFXParameter_Ducking_Release],
                           mFXParameter_Ducking_Release, mProcessor);
    release->setBounds(x + (3 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    release->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    release->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
    addAndMakeVisible(release);
    mGlobalSliders.add(release);
    
    
    
    if(mProcessor->getAccessToUIPersistentData().mIsParallelModeOn)
    {
        
        MFXParameterSlider* threshold_P =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Threshold_Parallel],
                               MFXParameterLabel[mFXParameter_Ducking_Threshold_Parallel],
                               mFXParameter_Ducking_Threshold_Parallel, mProcessor);
        threshold_P->setBounds(42 + (4.5f * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        threshold_P->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        threshold_P->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
        addAndMakeVisible(threshold_P);
        mGlobalSliders.add(threshold_P);
        
        
        MFXParameterSlider* amount_P =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Amount_Parallel],
                               MFXParameterLabel[mFXParameter_Ducking_Amount_Parallel],
                               mFXParameter_Ducking_Amount_Parallel, mProcessor);
        amount_P->setBounds(x + (5.6f * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        amount_P->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        amount_P->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
        addAndMakeVisible(amount_P);
        mGlobalSliders.add(amount_P);
        
        
        MFXParameterSlider* attack_P =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Attack_Parallel],
                               MFXParameterLabel[mFXParameter_Ducking_Attack_Parallel],
                               mFXParameter_Ducking_Attack_Parallel, mProcessor);
        attack_P->setBounds(x + (6.7f * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        attack_P->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        attack_P->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
        addAndMakeVisible(attack_P);
        mGlobalSliders.add(attack_P);
        
        MFXParameterSlider* release_P =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Ducking_Release_Parallel],
                               MFXParameterLabel[mFXParameter_Ducking_Release_Parallel],
                               mFXParameter_Ducking_Release_Parallel, mProcessor);
        release_P->setBounds(x + (7.8f * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        release_P->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        release_P->setColour(Slider::trackColourId, juce::Colour::fromRGB(65, 112, 247));
        addAndMakeVisible(release_P);
        mGlobalSliders.add(release_P);
    }
}

void MFXFxPanel::createMixUI()
{

    const int horisontalSlider_width = 50;
    const int horisontalSlider_height = 20;
    const int x = 176;
    
    const int y = 90;
    
    MFXParameterSlider* dry =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Global_Dry],
                           MFXParameterLabel[mFXParameter_Global_Dry],
                           mFXParameter_Global_Dry, mProcessor);
    dry->setBounds(x, y - 78, horisontalSlider_width, horisontalSlider_height);
    dry->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    dry->setColour(Slider::trackColourId, juce::Colour::fromRGB(249, 44, 44));
    
    
    
    addAndMakeVisible(dry);
    mGlobalSliders.add(dry);
    
    MFXParameterSlider* wet =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Global_Wet],
                           MFXParameterLabel[mFXParameter_Global_Wet],
                           mFXParameter_Global_Wet, mProcessor);
    wet->setBounds(x + 45, y - 78, horisontalSlider_width, horisontalSlider_height);
    wet->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    wet->setColour(Slider::trackColourId, juce::Colour::fromRGB(249, 44, 44));
    addAndMakeVisible(wet);
    mGlobalSliders.add(wet);
    
    
    MFXParameterSlider* stereoWidth =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Stereo_Width],
                           MFXParameterLabel[mFXParameter_Stereo_Width],
                           mFXParameter_Stereo_Width, mProcessor);
    stereoWidth->setBounds(x + (2 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    stereoWidth->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    stereoWidth->setColour(Slider::trackColourId, juce::Colour::fromRGB(249, 44, 44));
    addAndMakeVisible(stereoWidth);
    mGlobalSliders.add(stereoWidth);
    
    const int button_size = 60;
    const int button_y = 1;
    
    const auto& image_MS_Off =  ImageCache::getFromMemory(BinaryData::New_MS_Off_org2_png, BinaryData::New_MS_Off_org2_pngSize);
    const auto& image_MS_On =  ImageCache::getFromMemory(BinaryData::New_MS_On_org2_png, BinaryData::New_MS_On_org2_pngSize);
    
    MFXParameterButton* midSideMode =
    new MFXParameterButton(mProcessor->parameters, MFXParameterID[mFXParameter_Mid_Side_Mode], MFXParameterLabel[mFXParameter_Mid_Side_Mode],
                           image_MS_Off, image_MS_On, mProcessor, mFXParameter_Mid_Side_Mode);
    midSideMode->setBounds(188 + (3 * horisontalSlider_width), 1, button_size, button_size);
    
    addAndMakeVisible(midSideMode);
    mGlobalButtons.add(midSideMode);
    

}

void MFXFxPanel::createFXLevelsUI()
{
    
    
    const int horisontalSlider_width = 50;
    const int horisontalSlider_height = 20;
    const int x = 176;
    const int y = 90;

    
    
    
    MFXParameterSlider* addDelay =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Delay2WetDry],
                           MFXParameterLabel[mFXParameter_Delay2WetDry],
                           mFXParameter_Delay2WetDry, mProcessor);
    addDelay->setBounds(x, y - 78, horisontalSlider_width, horisontalSlider_height);
    addDelay->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    addDelay->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
    
    
    
    addAndMakeVisible(addDelay);
    mGlobalSliders.add(addDelay);
    
    
    
    
    MFXParameterSlider* addChorus =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ChorusWetDry],
                           MFXParameterLabel[mFXParameter_ChorusWetDry],
                           mFXParameter_ChorusWetDry, mProcessor);
    addChorus->setBounds(x + horisontalSlider_width, y - 78, horisontalSlider_width, horisontalSlider_height);
    addChorus->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    addChorus->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));

    
    addAndMakeVisible(addChorus);
    mGlobalSliders.add(addChorus);
    
    
    
    MFXParameterSlider* addPhaser =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_PhaserDryWet],
                           MFXParameterLabel[mFXParameter_PhaserDryWet],
                           mFXParameter_PhaserDryWet, mProcessor);
    addPhaser->setBounds(x + (2 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    addPhaser->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    addPhaser->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
    addPhaser->onValueChange = [&, addPhaser]()
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_PhaserDryWet])->setValue(addPhaser->getValue());
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_PhaserDryWet])->sendValueChangedMessageToListeners( addPhaser->getValue());
        
    };
    
    addAndMakeVisible(addPhaser);
    mGlobalSliders.add(addPhaser);
    
    MFXParameterSlider* addBitCrusher =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_BitDryWet],
                           MFXParameterLabel[mFXParameter_BitDryWet],
                           mFXParameter_BitDryWet, mProcessor);
    addBitCrusher->setBounds(x + (3 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    addBitCrusher->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    addBitCrusher->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));
    
    addBitCrusher->onValueChange = [&, addBitCrusher]()
    {
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_BitDryWet])->setValue(addBitCrusher->getValue());
        mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_BitDryWet])->sendValueChangedMessageToListeners( addBitCrusher->getValue());
        
    };
    
    addAndMakeVisible(addBitCrusher);
    mGlobalSliders.add(addBitCrusher);
    
    
    MFXParameterSlider* addReverb =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_ReverbWet],
                           MFXParameterLabel[mFXParameter_ReverbWet],
                           mFXParameter_ReverbWet, mProcessor);
    addReverb->setBounds(x + (4 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
    addReverb->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    addReverb->setColour(Slider::trackColourId, juce::Colour::fromRGB(235, 73, 249));

    
    addAndMakeVisible(addReverb);
    mGlobalSliders.add(addReverb);
    
    repaint();
    
}

void MFXFxPanel::createCutModeUI()
{
    
    const int horisontalSlider_width = 50;
    const int horisontalSlider_height = 20;
    const int x = 176;
    const int y = 90;
    
    
    MFXParameterSlider* lowCutDelay1 =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Lowcut_Delay1],
                           MFXParameterLabel[mFXParameter_Lowcut_Delay1],
                           mFXParameter_Lowcut_Delay1, mProcessor);
    lowCutDelay1->setBounds(x, y - 78, horisontalSlider_width, horisontalSlider_height);
    lowCutDelay1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    lowCutDelay1->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
    
    
    addAndMakeVisible(lowCutDelay1);
    mGlobalSliders.add(lowCutDelay1);
    
    
    MFXParameterSlider* highCutDelay1 =
    new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Highcut_Delay1],
                           MFXParameterLabel[mFXParameter_Highcut_Delay1],
                           mFXParameter_Highcut_Delay1, mProcessor);
    highCutDelay1->setBounds(x + horisontalSlider_width, y - 78, horisontalSlider_width, horisontalSlider_height);
    highCutDelay1->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
    highCutDelay1->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
    highCutDelay1->setAlwaysOnTop(true); 
    
    addAndMakeVisible(highCutDelay1);
    mGlobalSliders.add(highCutDelay1);
    
    
    if(mProcessor->getAccessToUIPersistentData().mIsParallelModeOn)
    {
        MFXParameterSlider* lowCutDelay2 =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Lowcut_Delay2],
                               MFXParameterLabel[mFXParameter_Lowcut_Delay2],
                               mFXParameter_Lowcut_Delay2, mProcessor);
        lowCutDelay2->setBounds(x + (2 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        lowCutDelay2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        lowCutDelay2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
        
        
        
        addAndMakeVisible(lowCutDelay2);
        mGlobalSliders.add(lowCutDelay2);
        
        
        MFXParameterSlider* highCutDelay2 =
        new MFXParameterSlider(mProcessor->parameters, MFXParameterID[mFXParameter_Highcut_Delay2],
                               MFXParameterLabel[mFXParameter_Highcut_Delay2],
                               mFXParameter_Highcut_Delay2, mProcessor);
        highCutDelay2->setBounds(x + ( 3 * horisontalSlider_width), y - 78, horisontalSlider_width, horisontalSlider_height);
        highCutDelay2->setSliderStyle(MFXParameterSlider::SliderStyle::LinearHorizontal);
        highCutDelay2->setColour(Slider::trackColourId, juce::Colour::fromRGB(160, 205, 103));
        
        
        addAndMakeVisible(highCutDelay2);
        mGlobalSliders.add(highCutDelay2);
    
    }
    
}

