/*
  ==============================================================================

    MFXStereoWidth.cpp
    
    Author:  Michael Kauffmann

  ==============================================================================
*/

#include "MFXStereo.h"


MFXStereo::MFXStereo(){}

MFXStereo::~MFXStereo(){}

// Process a Mid-Side input to stereo decoding (if M/S selected and processMS() is called first)
// Or it process/decode a stereo input to mono/stereo controling width,
// or if mono input and L/R delay time is different, it process/decode as stereo input to mono/stereo controling width.
// if mono input and if M/S selected and processMS() is called first - then not usable.
void MFXStereo::processWidth (float& leftCh, float& rightCh)
{
    
    /** sum and difference. @param mWidth range from 0-1.0f */

    // encoding
    const float mid = (leftCh + rightCh) / 2.0f;
    const float side = mWidth * (leftCh - rightCh); 
    
    // decoding
    leftCh = mid + side;
    rightCh = mid - side;
    
    
}

// Process stereo input to mid-Side encoding
// Do not call this if only mono input.
void MFXStereo::processMS(float& leftCh, float& rightCh)
{
    

    const auto mid = (leftCh + rightCh) / 2.0f;
    const auto side = (leftCh - rightCh) / 2.0f;   
    leftCh = mid;
    rightCh = side;
    
        
}




