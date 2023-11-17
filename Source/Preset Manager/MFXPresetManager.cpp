/*
  ==============================================================================

    MFXPresetManager.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXPresetManager.h"

#if JUCE_WINDOWS
static const String directorySeparator = "\\";
#elif JUCE_MAC
static const String directorySeparator = "/";
#elif JUCE_LINUX
static const String directorySeparator = "/";

#endif

//commonDocumentsDirectory

MFXPresetManager::MFXPresetManager(AudioProcessor* inProcessor, AudioProcessorValueTreeState& apvts) : mCurrentPresetIsSaved(false),
                                                                  mCurrentPresetName("Default"),
                                                                  mProcessor(inProcessor),
                                                                   mAPVTS(apvts)
{
    const String pluginName = (String) mProcessor->getName();
    
    
    // If change, Remember to change 2 locations here and topPanel
    
#if JUCE_WINDOWS

    // path point to this C:\ProgramData\Time Razor
    mPresetDirectory = (File::getSpecialLocation(File::commonApplicationDataDirectory)).getFullPathName() + directorySeparator + pluginName;
    //mPresetDirectory = (File::getSpecialLocation(File::commonApplicationDataDirectory)).getFullPathName() + directorySeparator + "Be Good Audio" + directorySeparator + pluginName;

#elif JUCE_MAC
    // path point to this C:\ProgramData\Time Razor
    mPresetDirectory = (File::getSpecialLocation(File::commonDocumentsDirectory)).getFullPathName() + directorySeparator + pluginName;
    //mPresetDirectory = (File::getSpecialLocation(File::commonApplicationDataDirectory)).getFullPathName() + directorySeparator + "Be Good Audio" + directorySeparator + pluginName;


#endif
    
    
    
    

    if(!File(mPresetDirectory).exists()){
        File(mPresetDirectory).createDirectory();
    }
    
    // store in array (runtime access)
    storeLocalPreset();
    
    
   
}
MFXPresetManager::~MFXPresetManager()
{
    
}



int MFXPresetManager::getNumberOfPresets()
{
    return mLocalPresets.size();
}

String MFXPresetManager::getPresetName(int inPresetIndex)
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();  
}

void MFXPresetManager::createNewPreset() // TODO:: consider rename....this do not create a new preset, only set default values, a reset to default function - prepareNewPre..
{
    auto& parameters = mProcessor->getParameters();
    
    
    for(int i = 0; i < parameters.size(); i++)
    {
        AudioProcessorParameterWithID* parameter =
        (AudioProcessorParameterWithID*)parameters.getUnchecked(i);
        
        const float defaultValue =
        parameter->getDefaultValue();
    
        parameter->setValueNotifyingHost(defaultValue);
        
    }
    
    mCurrentPresetIsSaved = false;
    mCurrentPresetName = "Default";
    
    
}

void MFXPresetManager::savePreset()
{
    MemoryBlock destinationData;
    std::unique_ptr<XmlElement> xml (mAPVTS.copyState().createXml());
     
    juce::AudioPluginInstance::copyXmlToBinary  (*xml, destinationData);

    
    mCurrentlyLoadedPreset.deleteFile();

    mCurrentlyLoadedPreset.appendData(destinationData.getData(),
                                      destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
}

void MFXPresetManager::saveAsPreset(String inPresetName)
{
    File presetFile = File(mPresetDirectory + directorySeparator + "User" + directorySeparator + inPresetName + PRESET_FILE_EXTENTION);
    
    
    if(!presetFile.exists()){
        presetFile.create();
    } else {
        presetFile.deleteFile();
    }
    
    MemoryBlock destinationData;
    

    juce::ValueTree params("Params");
    
    for (auto& param: mProcessor->getParameters())
    {
        juce::ValueTree paramTree(getParamID(param));
        paramTree.setProperty("Value", param->getValue(), nullptr);
        params.appendChild(paramTree, nullptr);
    }
    
    juce::ValueTree pluginPreset("TimeRazor");
    pluginPreset.appendChild(params, nullptr);
    
    
    juce::AudioPluginInstance::copyXmlToBinary(*pluginPreset.createXml(), destinationData);
    
    
   
    
    presetFile.appendData(destinationData.getData(),
                          destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPreset();
}

void MFXPresetManager::loadPreset(int inPresetIndex)
{
    mCurrentID = inPresetIndex;
    
    
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    // check file not empty or corupt
    if(mCurrentlyLoadedPreset.loadFileAsData(presetBinary) ){
        
        mCurrentPresetIsSaved = true;
        mCurrentPresetName = getPresetName(inPresetIndex);
        
        

        auto xml = juce::AudioPluginInstance::getXmlFromBinary(presetBinary.getData(), static_cast<int>( presetBinary.getSize()));
        
        if (xml != nullptr)
        {
            auto preset = juce::ValueTree::fromXml(*xml);
            auto params = preset.getChildWithName("Params");
            
            for (auto& param: mProcessor->getParameters())
            {
                auto paramTree = params.getChildWithName(getParamID(param));
                
                if (paramTree.isValid())
                    param->setValueNotifyingHost(paramTree["Value"]);
                
                
                
            }
    
        }
        
        
    }
    
    
    
}

bool MFXPresetManager::getIsCurrentPresetSaved()
{
    return mCurrentPresetIsSaved;
}

String MFXPresetManager::getCurrentPresetName()
{
    return mCurrentPresetName;
}


// This also set the size of the preset categories. We use this to build the comboBox menu in TopPanel class.
// This function must be called when plugin is loaded in DAW and when user save a new user preset.
void MFXPresetManager::storeLocalPreset()
{
    mLocalPresets.clear();
        
    mLocalPresetNamesUserSize = 0;
    mLocalPresetNamesPitchTapeCoruptedSize = 0;
    mLocalPresetNamesBasicSize = 0;
    mLocalPresetNamesMSSize = 0;
    mLocalPresetNamesTremoloStutterSize = 0;
    mLocalPresetNamesResonatingSize = 0;
    mLocalPresetNamesBigLongSize = 0;
    mLocalPresetNamesPanningDuckingSize = 0;
    
    
    


   

    
    for (DirectoryEntry entry : RangedDirectoryIterator (File (mPresetDirectory), true))
         if (entry.getFile().getFileNameWithoutExtension() != ".DS_Store")
         {
             
             
             if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Basic")
             {
                 mLocalPresets.add(entry.getFile());
     
                 mLocalPresetNamesBasicSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Big Long Delays")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesBigLongSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "MS")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesMSSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Panning & Ducking")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesPanningDuckingSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Pitch & Tape & Corupted")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesPitchTapeCoruptedSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Resonating")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesResonatingSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "Tremolo & Stutter")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesTremoloStutterSize++;
             }
             
             else if (entry.getFile().getParentDirectory().getFileNameWithoutExtension() == "User")
             {
                 mLocalPresets.add(entry.getFile());
                 
                 mLocalPresetNamesUserSize++;
             }
             
             
         }
    
    
    
    mLocalPresets.sort(); // this actually do nothing, as files are not sorted(not a string). Delete this!


   
    
    
}

void MFXPresetManager::setCurrentPresetName(String newName)
{
    mCurrentPresetName = newName;
}
