/*
  ==============================================================================

    MFXStateVariableTPTFilter.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXMoogFilter.h"




MFXMoogFilter::MFXMoogFilter(ModulationData& data)
: mData(data){}

MFXMoogFilter::~MFXMoogFilter(){}

void MFXMoogFilter::prepareToProces (const juce::dsp::ProcessSpec& spec) noexcept
{
    mFilter.prepare (spec);
    mFilter.setEnabled(true);
}




void MFXMoogFilter::process(float& sample, const int& channel ) noexcept
{
    mFilter.updateSmoothers();
    sample = mFilter.processSample(sample, channel);
}





void MFXMoogFilter::modulateParameters(int processLoopIndex) noexcept
{
    auto modSumCutoff = 0.0f;
    auto modSumResonans = 0.0f;
    auto modSumDrive = 0.0f;
    

    float* modulatorsWriterPointer[5] = { mData.mModulatorOutput[LFO1].getWritePointer (0),
                                          mData.mModulatorOutput[LFO2].getWritePointer (0),
                                          mData.mModulatorOutput[EnvF].getWritePointer(0),
                                          mData.mModulatorOutput[adsr].getWritePointer(0),
                                          mData.mModulatorOutput[adsr2].getWritePointer(0) };
        
    for (auto& set : mData.mModulationTargetData)
        {
            // string == string, this is the devil (c++ string comparison). High resource consume due to iteration of string when compare. 
            // Fixed -  made enum (MFXUsedParameters::ModulationTargetIndex)
            // carefull though if target AudioParameterChoice/StringArray change - order must match!
            if ( std::get<1>(set) == FilterCutoff )
            {
                modSumCutoff +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
            }
            
            
            else if (std::get<1>(set) == FilterResonans)
            {
                modSumResonans +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
            }
            
            else if (std::get<1>(set) == FilterDrive)
            {
                modSumDrive +=  modulatorsWriterPointer[std::get<0>(set)][processLoopIndex] * std::get<2>(set);
            }
            
        }
        
        
        if ( modSumCutoff != 0.0f  ||
            modSumResonans != 0.0f||
            modSumDrive != 0.0f)
        {
            
            updateParameters(mCurrentTypeValue, mCurrentCutoffValue, mCurrentResonansValue,
                             mCurrentDriveValue, modSumCutoff, modSumResonans, modSumDrive);
            
        }
    
    
    
}





void MFXMoogFilter::updateParameters (const int filterType,
                                      float cutoff,
                                      float resonance,
                                      float drive,
                                      const float& cutoffModulator,
                                      const float& resonansModulator,
                                      const float& driveModulator) noexcept
{

    mCurrentCutoffValue = cutoff;
    mCurrentResonansValue = resonance; 
    mCurrentDriveValue = drive;
    mCurrentTypeValue = filterType;
    
    
    
    
    
    
    switch (filterType)
    {
        case 0:
            mFilter.setMode(juce::dsp::LadderFilterMode::LPF12);
            break;
            
        case 1:
            mFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
            break;
            
        case 2:
            mFilter.setMode(juce::dsp::LadderFilterMode::BPF12);
            break;
            
        case 3:
            mFilter.setMode(juce::dsp::LadderFilterMode::BPF24);
            break;
            
        case 4:
            mFilter.setMode(juce::dsp::LadderFilterMode::HPF12);
            break;
            
        case 5:
            mFilter.setMode(juce::dsp::LadderFilterMode::HPF24);
            break;
            
        default : mFilter.setMode(juce::dsp::LadderFilterMode::LPF12);
            break;
    }
    
    
    cutoff = (cutoffModulator > 0.0f) ? cutoff += ((15000.0f - cutoff) * cutoffModulator) : cutoff += (cutoff * cutoffModulator);
    cutoff = std::fmin(std::fmax(cutoff, 20.0f), 15000.0f);
    
    resonance = resonansModulator > 0.0f ? resonance += ((1 - resonance) * resonansModulator) : resonance += (resonance * resonansModulator);
    resonance = std::fmin(resonance, 1.0f);
    
    drive = driveModulator > 0.0f ? drive += ((20 - drive) * driveModulator) : drive += (drive * driveModulator);
    drive = std::fmin(std::fmax(drive, 1.0f), 20.0f);
    
    mFilter.updateSmoothers();  
    mFilter.setCutoffFrequencyHz(cutoff);
    mFilter.setResonance (resonance);
    mFilter.setDrive(drive);
    
    
    
    
    
}

void MFXMoogFilter::reset() noexcept
{
    mFilter.reset();
    
}

float& MFXMoogFilter::getCurrentCutoffValue() noexcept
{
    return mCurrentCutoffValue;
}

float& MFXMoogFilter::getCurrentResonansValue() noexcept
{
    return mCurrentResonansValue;
}

float& MFXMoogFilter::getCurrentDriveValue() noexcept
{
    return mCurrentDriveValue;
}

int& MFXMoogFilter::getCurrentTypeValue() noexcept
{
    return mCurrentTypeValue;
}



