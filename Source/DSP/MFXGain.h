/*
  ==============================================================================

    MFXGain.h
    
    Author:  Michael Kauffmann

  ==============================================================================
*/



class MFXGain
{
    
public:
    
    MFXGain();
   ~MFXGain();
    

    void process(float* inAudioChannel, float inGain, int inNumSamplesToRender);
    
    
    
private:
    
};
