/*
  ==============================================================================

    MFXTopPanel.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXTopPanel.h"


MFXTopPanel::MFXTopPanel(PluginProcessor* inProcessor)
: MFXPanelBase(inProcessor)
{
    
    startTimer(30);
    
    mPresetDisplay = std::make_unique<ComboBox>();  
    
    
    mPresetDisplay->addListener(this);
    mPresetDisplay->setColour(MFXParameterComboBox::backgroundColourId, juce::Colour::fromRGB(48, 48, 48));
    addAndMakeVisible(mPresetDisplay.get());
    
    updatePresetComboBox();
    
    
    mOptionalDisplay = std::make_unique<ComboBox>();
    mOptionalDisplay->setColour(MFXParameterComboBox::backgroundColourId, juce::Colours::transparentBlack);
    mOptionalDisplay->setColour(ComboBox::arrowColourId, colour_1);
    
    mOptionalDisplay->addListener(this);
    mOptionalDisplay->addItem("New", 1);
    
    
    mOptionalDisplay->addItem("Save As", 2);
    
    mOptionalDisplay->addItem("About", 3);

    mOptionalDisplay->addItem("Read Manual", 4);
    
    mOptionalDisplay->setColour(ComboBox::textColourId, colour_3);
    
    
    
    addAndMakeVisible(mOptionalDisplay.get());
    
    mNameOfPlugin = std::make_unique<TextButton>();
    mNameOfPlugin->setName("Time Razor");
    mNameOfPlugin->setButtonText("Time Razor");
    mNameOfPlugin->setColour(TextButton::ColourIds::textColourOffId, colour_1);
    mNameOfPlugin->setColour(TextButton::ColourIds::textColourOnId, colour_1);
    
    mNameOfPlugin->onStateChange = [&]()
    {
        if (mNameOfPlugin->isMouseOver())
            mProcessor->getAccessToUIPersistentData().mHelperUIText = "Click here a few times if you want to clear all audio. Useful when working with long feedback delay time or reverb";
        else mProcessor->getAccessToUIPersistentData().mHelperUIText = "";
    };
    
    
    mNameOfPlugin->onClick = [=]()
    {
        mProcessor->setClearBuffer(true);
    };

    
    
    addAndMakeVisible(mNameOfPlugin.get());
    
    
    mBackWard = std::make_unique<ArrowButton>("", 0.5, colour_1);
    mForward = std::make_unique<ArrowButton>("", 0.0, colour_1);
    
    
    
    // On click, it jumps 1 preset/step back
    mBackWard->onClick = [this] ()
    {
        // Same as comment in mForward->onClick() below about *
        
        int index = mPresetDisplay->getSelectedId() - 1;
        int lastIndex = mProcessor->getPresetManager()->getCurrentID();
        lastIndex = lastIndex == 0 ? 1 : lastIndex; // this deals with firstItem/default added *
        
        
        if (!(index <= 0) && !mPresetDisplay->getText().contains("*"))
            mPresetDisplay->setSelectedId(index);
        else if (mPresetDisplay->getText().contains("*"))
                 mPresetDisplay->setSelectedId(lastIndex);
        
    };
    
    addAndMakeVisible(mBackWard.get());
    
    // On click, it jumps 1 preset/step forward
    mForward->onClick = [this] ()
    {
        
        /* when a * is added in timerCallback below, display enter no selection/edited mode. getSelectedId() will return 0. 
           Therefor we need to get index from presetManager instead, which is an array - so must add 1 to match
           comboBox (1 to lastItem...1,2,3..). When no * - getSelectedId() + 1 is used.
        */
        
        int index = mPresetDisplay->getSelectedId() + 1;
        int lastIndex = mProcessor->getPresetManager()->getCurrentID() + 2; 
        
        
        if (index <= mPresetDisplay->getNumItems() && !mPresetDisplay->getText().contains("*"))
            mPresetDisplay->setSelectedId(index);
        
        else if (lastIndex <= mPresetDisplay->getNumItems())
                 mPresetDisplay->setSelectedId(lastIndex);
        
            
    };
    
    addAndMakeVisible(mForward.get());
    
    
    mProcessor->mPresetNameChangeEvent.addActionListener(this);
    
    mCreditPanel = std::make_unique<MFXCreditPanel>();

    mUnlockPanel = std::make_unique<MFXUnlockPanel>(inProcessor);


    /*If you add this as a parameter to a member function of a class,
    it will be passed as a constant pointer to the object on which the function is called.
    The this pointer is automatically created by the compiler and points to the object for which the member function was called.
    Passing this as a parameter allows the member function to accessand modify the member variablesand
    other member functions of the object for which it was called.Since the this
    pointer is a constant pointer, the member function cannot modify the memory location
    of the object, but it can modify its member variablesand call its member functions.
    In this sense, this is not passed by value, but rather by reference, as the this pointer
    is a constant pointer to the memory location of the object.*/
    mUnlockPanel->addListenerToButtons(this);
    
   

}



MFXTopPanel::~MFXTopPanel()
{
    
    if(mPresetDisplay->getText().contains("*"))
       mProcessor->getAccessToUIPersistentData().mIsSliderTouchedByUser = true;

    mProcessor->mPresetNameChangeEvent.removeActionListener(this);
    mUnlockPanel->removeListenerToButtons(this);
    mOptionalDisplay->removeListener(this);
    mPresetDisplay->removeListener(this);

    
    
}

void MFXTopPanel::paint(Graphics& g)
{
    g.setColour (Colours::seagreen);
    g.drawLine(0, getHeight() - 4, getWidth(), getHeight() - 3, 4);
    g.setColour (Colours::blueviolet);
    g.drawLine(0, getHeight() - 8, getWidth(), getHeight() - 8, 4);
    
    const auto& image_menu =  ImageCache::getFromMemory(BinaryData::New_menu_hamburger_org2_png, BinaryData::New_menu_hamburger_org2_pngSize);
   
    
    
    g.drawImageWithin(image_menu,0,6, image_menu.getWidth() / 4, image_menu.getHeight() / 4, juce::RectanglePlacement::stretchToFit);
    
    const auto& image_logo =  ImageCache::getFromMemory(BinaryData::New_LOGO_org2_png, BinaryData::New_LOGO_org2_pngSize);
    g.drawImageWithin(image_logo, 93, 3, image_logo.getWidth() / 2, image_logo.getHeight() / 2, juce::RectanglePlacement::stretchToFit);
    
   

   /* if (!(mProcessor->readUnlockedStatus()) && !mProcessor->getDemoMode())
    {
       
        const auto area = getLocalBounds().removeFromTop(35);
        mUnlockPanel->setBounds(area);

        addAndMakeVisible(mUnlockPanel.get());
    }*/
    
}

void MFXTopPanel::resized()
{
    const int comboBox_w = 200;
    const int comboBox_x = getWidth() * 0.5 - comboBox_w * 0.5;
    const int arrow_x = comboBox_x - 30;
    
    mPresetDisplay->setBounds(comboBox_x, 8, comboBox_w, 18);
    mBackWard->setBounds(arrow_x, 12, 14, 15);
    mForward->setBounds(arrow_x + 15, 12, 14, 15);
    
    
    mOptionalDisplay->setBounds(5, 8, comboBox_w - 100, 25);
    
    mNameOfPlugin->setBounds(getWidth() - (150), 0, 150, getHeight() -10);
    mNameOfPlugin->changeWidthToFitText();
    
}

void MFXTopPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{   
    
    
    
    
    if(comboBoxThatHasChanged == mPresetDisplay.get())
    {
        // to avoid abrupt audio behavior when changing preset
        mProcessor->setClearBuffer(true);
        
        
        MFXPresetManager* presetManager = mProcessor->getPresetManager();

        mCurrentIndex = mPresetDisplay->getSelectedItemIndex();
        
       
        
        
        // if we close/open plugin window - original preset should not be reloaded overwritting new user settings.
        
        if(!mProcessor->getAccessToUIPersistentData().mIsSliderTouchedByUser && this->isShowing() == 1.0f) // isShow prevent DAW recall saved session to trigger this code, setting link to 0
        {
            // reset link time/feed - because if on and jumps to new preset that is off will cause error
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2LinkTime])->setValueNotifyingHost(0.0f);
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1LinkTime])->setValueNotifyingHost(0.0f);
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay2LinkFeedback])->setValueNotifyingHost(0.0f);
            mProcessor->parameters.getParameter(MFXParameterID[mFXParameter_Delay1LinkFeedback])->setValueNotifyingHost(0.0f);
            
            presetManager->loadPreset(mCurrentIndex);
            // use in FXPanel timer callback - setFXStyle(). 
            mProcessor->getAccessToUIPersistentData().mIsPresetLoaded = true;            
        } 
    }
    
    else if (comboBoxThatHasChanged == mOptionalDisplay.get())
    {
             MFXPresetManager* presetManager = mProcessor->getPresetManager();
          
             if(mOptionalDisplay->getText() == "New")
             {
                 presetManager->createNewPreset();
                 updatePresetComboBox();
                 // a preset is not loaded but this will make sure UI (repitch slider) is correct
                 mProcessor->getAccessToUIPersistentData().mIsPresetLoaded = true;
             }
          
             else if (mOptionalDisplay->getText() == "Save")
             {
              
                  if (presetManager->getIsCurrentPresetSaved() == 0)
                      displaySaveAsPopup();
             
                  else presetManager->savePreset();
             }
          
             else if(mOptionalDisplay->getText() == "Save As")
             {
                 //if (mProcessor->readUnlockedStatus())     
                     displaySaveAsPopup();
             }
          
             else if(mOptionalDisplay->getText() == "About")
             {
                 const auto area = getLocalBounds().removeFromTop(35); 
                 mCreditPanel->setBounds(area);
                 
                 addAndMakeVisible(mCreditPanel.get());          
             }

             else if (mOptionalDisplay->getText() == "Read Manual")
             {
                 const char* webpageURL = "https://www.be-good-audio.com/support";

                 // Command to open the webpage in the default browser
                 #if JUCE_WINDOWS
                 const char* command = "start";
                 #elif JUCE_MAC
                 const char* command = "open";
                 #endif

                 // Construct the full command
                 std::string fullCommand = command;
                 fullCommand += " ";
                 fullCommand += webpageURL;

                 // Open the webpage in the default browser
                 std::system(fullCommand.c_str());

             }

             mOptionalDisplay->setText(" ", NotificationType::dontSendNotification);     
        
    }
}


void MFXTopPanel::timerCallback()
{
    if (mProcessor->getAccessToUIPersistentData().mIsSliderTouchedByUser)
    {
        MFXPresetManager* presetManager = mProcessor->getPresetManager();
        mPresetDisplay->setText (presetManager->getCurrentPresetName().contains("*") ? presetManager->getCurrentPresetName():
                                presetManager->getCurrentPresetName() + "*", NotificationType::dontSendNotification);
        
        mProcessor->getAccessToUIPersistentData().mIsSliderTouchedByUser = false;    
    }
    
    else if (mProcessor->getAccessToUIPersistentData().mIsButtonTouchedByUser)
    {
        MFXPresetManager* presetManager = mProcessor->getPresetManager();
        mPresetDisplay->setText(presetManager->getCurrentPresetName().contains("*") ? presetManager->getCurrentPresetName():
                                presetManager->getCurrentPresetName() + "*", NotificationType::dontSendNotification);
        
        mProcessor->getAccessToUIPersistentData().mIsButtonTouchedByUser = false;
    }

    else if (mProcessor->getAccessToUIPersistentData().mIsComboBoxTouchedByUser)
    {
        MFXPresetManager* presetManager = mProcessor->getPresetManager();
        mPresetDisplay->setText(presetManager->getCurrentPresetName().contains("*") ? presetManager->getCurrentPresetName():
                                presetManager->getCurrentPresetName() + "*", NotificationType::dontSendNotification);
        
        mProcessor->getAccessToUIPersistentData().mIsComboBoxTouchedByUser = false;
    }
    
}


void MFXTopPanel::actionListenerCallback (const String &message)
{
    MFXPresetManager* presetManager = mProcessor->getPresetManager();
    mPresetDisplay->setText(presetManager->getCurrentPresetName());
}

void MFXTopPanel::buttonClicked(Button* button)
{
   /* if (button->getName() == "Demo") 
    {
        mProcessor->setDemoMode(true);
        mUnlockPanel->setVisible(false);
    }
    
    if (button->getName() == "Unlock")
    {
        mUnlockPanel->setVisible(false);
    }*/
}

void MFXTopPanel::displaySaveAsPopup()
{
    
    #if JUCE_WINDOWS
    static const String directorySeparator = "\\";
    #elif JUCE_MAC
    static const String directorySeparator = "/";
    #elif JUCE_LINUX
    static const String directorySeparator = "/";
    #endif
    
    mOptionalDisplay->setText("");

    MFXPresetManager* presetManager = mProcessor->getPresetManager();

    String currentPresetName = presetManager->getCurrentPresetName();
    
    #if JUCE_WINDOWS

    mChooser = std::make_unique<FileChooser>("Please enter a name for your preset",
                                             File::getSpecialLocation(File::commonApplicationDataDirectory).getFullPathName() + directorySeparator
                                             + "Dual Delay MFX");

    #elif JUCE_MAC

    mChooser = std::make_unique<FileChooser>("Please enter a name for your preset",
                                             File::getSpecialLocation(File::commonDocumentsDirectory).getFullPathName() + directorySeparator
                                             + "Dual Delay MFX");
    #endif



    mChooser = std::make_unique<FileChooser> ("Please enter a name for your preset",
                                              File::getSpecialLocation (File::commonApplicationDataDirectory).getFullPathName() + directorySeparator
                                              + "Dual Delay MFX");
        
    auto folderChooserFlags = FileBrowserComponent::saveMode;
    
    mChooser->launchAsync (folderChooserFlags, [this, presetManager] (const FileChooser& chooser)
                           {
                               if(!chooser.getResult().getFileNameWithoutExtension().isEmpty())
                               {
                                   presetManager->saveAsPreset(chooser.getResult().getFileName()); 
                                   updatePresetComboBox();
                                
                               }
                            });
                                
}




// Build the preset menu list
void MFXTopPanel::updatePresetComboBox()
{
    MFXPresetManager* presetManager = mProcessor->getPresetManager();
    
    String presetName = presetManager->getCurrentPresetName();
    
    mPresetDisplay->clear(dontSendNotification);
    
    
    const int numPresetsBasic = presetManager->getPresetNameBasicSize();
    const int numPresetsBigLong = presetManager->getPresetNameBigLongSize();
    const int numPresetsMS = presetManager->getPresetNameMSSize();
    const int numPresetsPanningDucking = presetManager->getPresetNamePanningDuckingSize();
    const int numPresetsPitchTapeCorupted = presetManager->getPresetNamePitchTapeCoruptedSize();
    const int numPresetsResonating = presetManager->getPresetNameResonatingSize();
    const int numPresetsTremoloStutter = presetManager->getPresetNameTremoloStutterSize();
    const int numPresetsUser = presetManager->getPresetNameUserSize();
    
    PopupMenu subMenuBasic;
    PopupMenu subMenuBigLong;
    PopupMenu subMenuMS;
    PopupMenu subMenuPanningDucking;
    PopupMenu subMenuPitchTapeCorupted;
    PopupMenu subMenuResonating;
    PopupMenu subMenuTremoloStutter;
    PopupMenu subMenuUser;
    
    
    int menuItemIDCounter = 1;
    int menuItemCounter = 0;
    
    
    
    for(int i = 0; i < numPresetsBasic; i++)
    {
        subMenuBasic.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Basic", subMenuBasic,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsBigLong; i++)
    {
        subMenuBigLong.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;  
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Big Long Delays", subMenuBigLong,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsMS; i++)
    {
        subMenuMS.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;   
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "M/S", subMenuMS,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsPanningDucking; i++)
    {
        subMenuPanningDucking.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;    
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Panning & Ducking", subMenuPanningDucking,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsPitchTapeCorupted; i++)
    {
        subMenuPitchTapeCorupted.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;       
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Pitch & Tape & Corupted", subMenuPitchTapeCorupted,true, nullptr, false, 0 );
    
    
    
    for(int i = 0; i < numPresetsResonating; i++)
    {
        subMenuResonating.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;        
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Resonating", subMenuResonating,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsTremoloStutter; i++)
    {
        subMenuTremoloStutter.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;       
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "Tremolo & Stutter", subMenuTremoloStutter,true, nullptr, false, 0 );
    
    
    for(int i = 0; i < numPresetsUser; i++)
    {
        subMenuUser.addItem(PopupMenu::Item(presetManager->getPresetNameFromLocalStorage(menuItemCounter)).setID(menuItemIDCounter));
        menuItemIDCounter++;
        menuItemCounter++;
    }
    
    mPresetDisplay->getRootMenu()->addSubMenu( "User", subMenuUser,true, nullptr, false, 0 );
    mPresetDisplay->setText(presetManager->getCurrentPresetName(), NotificationType::dontSendNotification);    
}
