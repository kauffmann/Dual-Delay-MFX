/*
  ==============================================================================

    MFXADSR.h
    
    Author:  Michael Kauffmann

    About ADSR class (made by JUCE). A very simple ADSR envelope class.

    To use it, call setSampleRate() with the current sample rate and give it some parameters with setParameters() then call getNextSample() to get the envelope value to be applied to each audio sample or applyEnvelopeToBuffer() to apply the envelope to a whole buffer.

    Do not change the parameters during playback. If you change the parameters before the release stage has completed then you must call reset() before the next call to noteOn().

    A ADSR envelope created inheriting from ADSR class. Class generate envelope values when midi note on/off are received and getNextSample() are called. 
    The state of envelopoe is updated from controller (pluginProcessor) and modulationData. The generated data are stored and managed by modulationData.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MFXADSR : public juce::ADSR
{
public:

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;

    void updateADSR() noexcept;

    void setParams(const float attack, const float decay, const float sustain, const float release) noexcept;
    
private:
    juce::ADSR::Parameters adsrParams;

    float mAttack{ 0.0f };
    float mDecay{0.0f };
    float mSustain{ 0.0f };
    float mRelease{ 0.0f };
};
