/*
  ==============================================================================

    MFXDspLFO.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXLFO2.h"



MFXLFO2::MFXLFO2(){ mLfo.initialise([](float x) { return std::sinf(-x); },128); }

MFXLFO2::~MFXLFO2(){}


void MFXLFO2::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    mLfo.prepare(spec);
}



void MFXLFO2::setType (const LfoStyle oscSelection) noexcept
{
    switch (oscSelection)
    {
            // Sine
        case mLfoStyle_sin:
            mLfo.initialise ([](float x) { return std::sinf(-x); },128); // -x it start 0 - 1 then 1 - 0 - -1 and up. If x are positive it starts 0 - -1.
            break;
            
            // Saw
        case mLfoStyle_upSaw:
            mLfo.initialise ([] (float x)
        {
            return juce::jmap (x,
                               float (-juce::MathConstants<double>::pi),
                               float (juce::MathConstants<double>::pi),
                               float (1.0f), // (1/-1)makes a down saw. Sounds great and more audible difference between sin/saw. (-1/1) makes up saw.
                               float (-1.0f));
        }, 128);
            break;
            
            
            // Square
        case mLfoStyle_square:
            mLfo.initialise ([] (float x) { return x < 0.0f ? 1.0f : -1.0f; }, 128);
            
            break;
            // Triangle
        case mLfoStyle_triangle:
            mLfo.initialise ([] (float x)
                             {
                                 x = x / juce::MathConstants<double>::pi;
                                 return  juce::jmin(2.0+2.0*x, std::abs((x-0.5)*2.0)-1.0); // triangle in phase from serum
                                 
                             }, 128);
            break;
            // Down Saw
        case mLfoStyle_downSaw:
            mLfo.initialise ([] (float x) 
                             {
                                 return juce::jmap (x,
                                                    float (-juce::MathConstants<double>::pi),
                                                    float (juce::MathConstants<double>::pi),
                                                    float (-1.0f), // (1/-1)makes a down saw. Sounds great and more audible difference between sin/saw. (-1/1) makes up saw.
                                                    float (1.0f));
                             }, 128);
            break;
            
            
            
        default:
            // You shouldn't be here!
            jassertfalse;
            break;
    }
    
    
    /*Future upscale idea: Sample and hold is pretty simple: just increment your phase counter as usual and when it wraps around, take a new random value.
    (Although it might be nicer to run this at twice the speed so it takes two new random values per period.)  Random ran = Random()    ran.nextFloat() 0-1;    */
}


float MFXLFO2::process() noexcept
{
    return mLfo.processSample(0.0f) * mDepth;
}



void MFXLFO2::updateParams(const float& rate, const float& depth, const float& rateModulation) noexcept
{
    
    auto rateFinal = rateModulation > 0.0f ? rate + (( 20.0f - rate) * rateModulation) : rate + (rate * rateModulation);
    mLfo.setFrequency (rateFinal);
    mDepth = depth;
    
    mFrequency = rateFinal;
    
}

void MFXLFO2::updateParamsSync(const double &bpm, const float &noteIndex, const float &depth, const float& rateModulation) noexcept
{
    double rateSync = 0.0;
    
    float index = noteIndex;
    index = rateModulation > 0.0f ? index + (( 20.0f - index) * rateModulation) : index + (index * rateModulation);
    
    // Got Inspiration to do this from : http://www.bradthemad.org/guitar/tempo_explanation.php
    switch (roundToInt(index))
    {
        case 0: // 1
            rateSync = bpm / 240;
            break;
            
        case 1: // 1.
            rateSync = bpm / 360;
            break;
            
        case 2: // 1T
            rateSync = bpm / 160;
            break;
            
        case 3: //1/2
            rateSync = bpm / 120; // (60 x 2)
            break;
            
        case 4: //1/2.
            rateSync = bpm / 180;
            break;
        case 5: //1/2T
            rateSync = bpm / 80;
            break;
            
        case 6: //1/4
            rateSync = bpm / 60;
            break;
            
        case 7: //1/4.
            rateSync = bpm / 90; // bpm / ((1/4 / 2) + 1/4 )
            break;
            
        case 8: //1/4T
            rateSync = bpm / 40; // 1/4T plays 3 times within 2 x 1/4. To get 1  1/4T we do: bpm / ((2 x 1/4) / 3 triol )  
            break;
            
        case 9: //1/8
            rateSync = bpm / 30;
            break;
        
        case 10: //1/8.
            rateSync = bpm / 45; // bpm / ((1/8 / 2) + 1/8 )
            break;
            
        case 11: //1/8T
            rateSync = bpm / 20; // bpm / ((2 x 1/8) / 3 triol ) 
            break;
            
        case 12: //1/16
            rateSync = bpm / 15;
            break;
            
        case 13: //1/16.
            rateSync = bpm / 22.5;
            break;
            
        case 14: //1/16T
            rateSync = bpm / 10; // bpm / ((2 x 1/16) / 3 triol )
            break;
        
        case 15: //1/32
            rateSync = bpm / 7.5;
            break;
            
        case 16: //1/32.
            rateSync = bpm / 11.25;
            break;
            
        case 17: //1/32T
            rateSync = bpm / 5; // bpm / ((2 x 1/32) / 3 triol )
            break;
            
        case 18: //1/64
            rateSync = bpm / 3.75;
            break;
            
        case 19: //1/64.
            rateSync = bpm / 5.625;
            break;
        
        case 20: //1/64T
            rateSync = bpm / 2.5; // bpm / ((2 x 1/64) / 3 triol )
            break;
    
        default:
            rateSync = 1.0;
    }
    
    

    mLfo.setFrequency (rateSync);
    mDepth = depth;
    mSyncFrequency = rateSync; 
    
    
    
    
}

void MFXLFO2::reset(double inc) noexcept
{
    // remember change made in JUCE source files Oscillator class::reset(param added) and Phase::reset(param added). Making changes to files in JUCE framework is bad. TODO: overwrite instead. 
    mLfo.reset(inc);
}
