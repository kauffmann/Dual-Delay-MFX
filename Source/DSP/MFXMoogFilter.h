/*
  ==============================================================================

    MFXStateVariableTPTFilter.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

#include "MFXUsedParameters.h"
#include "ModulationData.h"
#include "MFXJuce_LadderFilter.h"



        class MFXMoogFilter
        {
        public:
            MFXMoogFilter(ModulationData& data);


            ~MFXMoogFilter();

            void prepareToProces(const juce::dsp::ProcessSpec& spec) noexcept;


            void process(float& sample, const int& channel) noexcept;


            void modulateParameters(int processLoopIndex) noexcept;

            void updateParameters(const int filterType,
                                  float cutoff,
                                  float resonance,
                                  float drive,
                                  const float& cutoffModulator = 0.0f,
                                  const float& resonansModulator = 0.0f,
                                  const float& driveModulator = 0.0f) noexcept;



            void reset() noexcept;

            float& getCurrentCutoffValue() noexcept;

            float& getCurrentResonansValue() noexcept;

            float& getCurrentDriveValue() noexcept;

            int& getCurrentTypeValue() noexcept;

        private:
            
            LadderFilter_Redesign<float> mFilter;
            float mCurrentCutoffValue{ 10000.0f }, mCurrentResonansValue{ 0.0f }, mCurrentDriveValue{ 1.0f };
            int mCurrentTypeValue{ 0 };
            bool mIsPrepared{ false };
            ModulationData& mData;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXMoogFilter);

        };
