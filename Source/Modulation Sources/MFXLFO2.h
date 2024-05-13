/*
  ==============================================================================

    MFXDspLFO.h
   
    Author:  Michael Kauffmann

    A LFO class that generate LFO values. The state of LFO is updated from controller (pluginProcessor) and modulationData.
    The generated data are stored and managed by modulationData.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXAudioHelpers.h"
#include "MFXJuce_Oscillator.h"


enum LfoStyle
{
    mLfoStyle_sin = 0,
    mLfoStyle_upSaw,
    mLfoStyle_square,
    mLfoStyle_triangle,
    mLfoStyle_downSaw,
    mLfoStyle_TotalNumStyles
};


        class MFXLFO2
        {

        public:

            MFXLFO2();
            ~MFXLFO2();

           

            void prepare(const juce::dsp::ProcessSpec& spec) noexcept;

            void setType(const LfoStyle oscSelection) noexcept;

            inline void setFrequency(const float& rate) noexcept
            {
                mLfo.setFrequency(rate);
            }

            float process() noexcept;

            void updateParams(const float& rate, const float& depth, const float& rateModulation = 0.0f) noexcept;

            void updateParamsSync(const double& bpm, const float& noteIndex, const float& depth, const float& rateModulation = 0.0f) noexcept;



            void reset(double inc = 0.0) noexcept;

            inline double getSyncFrequency()
            {
                return mSyncFrequency;
            }

            inline float getFrequency()
            {
                return mFrequency;
            }

        private:

            
            juce::dsp::Oscillator_Redesign<double> mLfo;
            juce::SmoothedValue<double> mSmoother;


            float mRate{ 0.0f }, mDepth{ 1.0f };
            double mSyncFrequency{ 0.0 };
            float mFrequency{ 0.0 };

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MFXLFO2);


        };

