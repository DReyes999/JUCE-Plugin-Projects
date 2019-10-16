/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include <memory>

#define MAX_DELAY_TIME 2 //defines our max delay time which dictates our circular buffer size. In seconds.

//==============================================================================
/**
*/
class KDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    KDelayAudioProcessor();
    ~KDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    //My function prototypes go here
    
    //smoothing and interpolation.
    //with our delay time it is possible to try and call samples which are not
    //ints and exist in spaces between buffer spaces.
    //To fix this we will use a Lerp function to extrapolate a value for these
    //Spaces.
    
    float lin_interp(float sample_x, float sample_x1, float inPhase);
    
    
    //==============================================================================

private:
    
    /*
     
     To create a delay we need to create a storage container which
     has enough space to hold the amount of samples dictated by our delay
     time.
     
     The amount of data = samplerate * maxDelayTime;
     
     this will be a circular buffer which will constantly fill with samples and output samples. This will be an array that holds the amount of data as dictated above.
     */
    
    //Start by making one array for the right channel and one for the left.
    // These are just instantiations of float pointers.
//    std::unique_ptr<float[]> mCircularBufferLeft;
//    std::unique_ptr<float[]> mCircularBufferRight;
    
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
    
    //Need a way to know where we are in the buffer and write to the next space
    //This is a write head into the circular buffer
    int mCircularBufferWriteHead;
    
    //Need a way to know when the write head is at the end of the circular buffer
    //This variable gives us a method for determining length and the end of the bufffer as a result
    int mCircularBufferLength;
    
    /*=========== Variables for Reading the Data =============*/
    
    float mDelayTimeInSamples; // = sampleRate * delayTime
    float mDelayReadHead;
    
    /*=========== Variables for feedback =============*/
    
    float mFeedbackLeft;
    float mFeedbackRight;
    
    /*=========== Smoothing =============*/
    
    float mDelayTimeSmoothed;
    
    /*=========== Audio parameters to expose to the interface =============*/
    
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mFeedbackParameter;
    AudioParameterFloat* mDelayTimeParameter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KDelayAudioProcessor)
};
