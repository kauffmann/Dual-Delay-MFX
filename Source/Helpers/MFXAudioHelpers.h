/*
  ==============================================================================

    MFXAudioHelpers.h
    
    Author:  Michael Kauffmann

    A helper header file with free functions and const variables that can be used across project files by including it. 
    Functions are marked inline to avoid linker error - multiple definitions of function.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MFXUsedParameters.h"
#include <juce_audio_formats/codecs/flac/compat.h>  // added fix M_PI  issue  


#define parameterSmoothingCoeff_Generic 0.04
#define parameterSmoothingCoeff_Fine 0.002
#define meterSmoothingCoeff 0.2

const double kPi = M_PI;
const double kPi2 = M_PI * 2;



inline float linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

inline float dBToNormalizedGain(float inValue)
{
    float inValuedB = Decibels::gainToDecibels(inValue + 0.00001f);
    inValuedB = (inValuedB + 96.0f) / 96.0f;
    
    return inValuedB;
}

inline float denormalize(float inValue)
{
    float absValue = fabs(inValue);
    
    // 0.00000000000001
    if(absValue < 1e-15){
        return 0.0f;
    } else {
        return inValue;
    }
}

inline double tanh_clip(double x)
{
    return x * ( 27 + x * x) / ( 27 + 9 * x * x);
}

inline bool isIntegerValue(double& N)
{
    
    
    // Convert float value of N to integer
    int X = static_cast<int>(N);
    //roundToIntAccurate(N);
    double temp2 = N - X;
    
    // If N is not equivalent to any integer
    if (temp2 > 0) {
        return false;
    }
    return true;
}




//------------Delay Sample Time Calculators. Consider to put in delay class or put in seperate class (base) that delay class inherit from. However, having it here makes it more reusable in other projects -------




// use with 4/4, 2/4  and 1/4 notes
inline double TripleTimeBig(const double beatTime, const int multiplyBeat) noexcept
{
    return (beatTime / 3) * multiplyBeat;
}

// use with 1/8......notes
inline double TripleTimeSmall(const double beatTime, const int divideBeat) noexcept
{
    return beatTime / divideBeat;
}


// use with 4/4 and 2/4 notes
inline double dottedTimeBig(const double beatTime, const int multiplyBeat) noexcept
{
    return (beatTime * multiplyBeat) * 1.5;
}

// use with 1/4, 1/8......notes
inline double dottedTimeSmall(const double beatTime, const int divideBeat) noexcept
{
    return (beatTime / divideBeat) * 1.5;
}

// use with 4/4 and 2/4 notes
inline double straightTimeBig(const double beatTime, const int multiplyBeat) noexcept
{
    return beatTime * multiplyBeat;
}

// use with 1/4, 1/8......notes
inline double straightTimeSmall(const double beatTime, const int divideBeat) noexcept
{
    return beatTime / divideBeat;
}


// use with milliseconds
inline double TimeMs(const double bufferLength, const float smoothedSliderValue) noexcept
{
    return bufferLength * smoothedSliderValue;
}

//--------------------------------------------------------------------------------------



// @sampleTimeOnBeat Remember to get playhead info and calcuclate sampleLength of 1 single beat (sampleLength between beats).
// You must do this in AudioProcessor class.
inline double sync(int toNotes, int inTimeMode, double sampleTimeOnBeat)
{
    
    double delayTimeInSamples = 0.0;
    
    
        switch (toNotes) {
            case 1: // 1/1
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeBig(sampleTimeOnBeat, 4);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeBig(sampleTimeOnBeat, 4);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeBig(sampleTimeOnBeat, 8);
                }
                break;
            case 2: // 1/2
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeBig(sampleTimeOnBeat, 2);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeBig(sampleTimeOnBeat, 2);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeBig(sampleTimeOnBeat, 4);
                }
                break;
            case 3: // 1/4
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeBig(sampleTimeOnBeat, 1);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeBig(sampleTimeOnBeat, 1);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeBig(sampleTimeOnBeat, 2);
                }
                break;
            case 4: // 1/8
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeSmall(sampleTimeOnBeat, 2);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeSmall(sampleTimeOnBeat, 2);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeSmall(sampleTimeOnBeat, 3);
                }
                break;
            case 5: // 1/16
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeSmall(sampleTimeOnBeat, 4);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeSmall(sampleTimeOnBeat, 4);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeSmall(sampleTimeOnBeat, 6);
                }
                break;
            case 6: // 1/32
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeSmall(sampleTimeOnBeat, 8);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeSmall(sampleTimeOnBeat, 8);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeSmall(sampleTimeOnBeat, 12);
                }
                break;
            case 7: // 1/64
                if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Straight)
                {
                    delayTimeInSamples = straightTimeSmall(sampleTimeOnBeat, 16);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Dotted)
                {
                    delayTimeInSamples = dottedTimeSmall(sampleTimeOnBeat, 16);
                }
                
                else if (static_cast<int>(inTimeMode) == mFXDelayTimeMode_Triplet)
                {
                    delayTimeInSamples = TripleTimeSmall(sampleTimeOnBeat, 24);
                }
                break;
                
            default:
                jassert(static_cast<int>(inTimeMode) == mFXDelayTimeMode_ms);
                // should't be here
                break;
        }
    return delayTimeInSamples;
    
}
