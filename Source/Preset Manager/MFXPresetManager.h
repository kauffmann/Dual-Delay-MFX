/*
  ==============================================================================

    MFXPresetManager.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "MFXUsedParameters.h"


#define PRESET_FILE_EXTENTION ".mfxpf"



class MFXPresetManager
{
    
public:
    
    MFXPresetManager(AudioProcessor* inProcessor, AudioProcessorValueTreeState& apvts);
    
    ~MFXPresetManager();
    
    int getNumberOfPresets();
    
    String getPresetName(int inPresetIndex);   // unused
    
    void createNewPreset();
    
    void savePreset();
    
    void saveAsPreset(String inPresetName);
    
    void loadPreset(int inPresetIndex);


    
    // help determine save/save as
    bool getIsCurrentPresetSaved();
    
    String getCurrentPresetName();
    
    void setCurrentPresetName(String newName);
    
   
    //--------------------------------------------------------------------------------------------------------------------------
    // Used for building UI preset Menu
    
    
    int getCurrentID() const noexcept   
    {
        return mCurrentID;
    }
    



    String getPresetNameFromLocalStorage(const int index) const noexcept
    {
        return mLocalPresets[index].getFileNameWithoutExtension();    
    }


    
    
    
    int getPresetNamePitchTapeCoruptedSize() const noexcept
    {
        return mLocalPresetNamesPitchTapeCoruptedSize;
    }
    
    int getPresetNameBasicSize() const noexcept
    {
        return mLocalPresetNamesBasicSize;
    }
    
    int getPresetNameMSSize() const noexcept
    {
        return mLocalPresetNamesMSSize;
    }
    
    int getPresetNameTremoloStutterSize() const noexcept
    {
        return mLocalPresetNamesTremoloStutterSize;
    }
    
    
    int getPresetNameResonatingSize() const noexcept
    {
        return mLocalPresetNamesResonatingSize;
    }
    
    int getPresetNamePanningDuckingSize() const noexcept
    {
        return mLocalPresetNamesPanningDuckingSize;
    }
    
    int getPresetNameBigLongSize() const noexcept
    {
        return mLocalPresetNamesBigLongSize;
    }
    
    int getPresetNameUserSize() const noexcept
    {
        return mLocalPresetNamesUserSize;
    }
    
    
    
    
    
    
    
    
    inline juce::String getParamID(juce::AudioProcessorParameter* param)
    {
        if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
            return paramWithID->paramID;
        
        return param->getName(50);
    }
    
    
private:
    
    
    
    void storeLocalPreset();
    
    bool mCurrentPresetIsSaved;
    
    File mCurrentlyLoadedPreset;
    
    // Store in array because its faster when swithing between presets compared to finding and load/read a file from disk I/O.
    Array<File> mLocalPresets;
    
    
    


    int mLocalPresetNamesUserSize{0};
    int mLocalPresetNamesPitchTapeCoruptedSize{0};
    int mLocalPresetNamesBasicSize{0};
    int mLocalPresetNamesMSSize{0};
    int mLocalPresetNamesTremoloStutterSize{0};
    int mLocalPresetNamesResonatingSize{0};
    int mLocalPresetNamesBigLongSize{0};
    int mLocalPresetNamesPanningDuckingSize{0};


    
    int mCurrentID{0};
    
    String mCurrentPresetName;
    
    String mPresetDirectory;
    
    
    XmlElement* mCurrentPresetXml;
    AudioProcessor* mProcessor;
    AudioProcessorValueTreeState& mAPVTS;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFXPresetManager);
    
    
    
};
