/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KDelayAudioProcessorEditor (KDelayAudioProcessor&);
    ~KDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KDelayAudioProcessor& processor;
    
    Slider mDryWetSlider;
    Slider mFeedbackSlider;
    Slider mDelayTimeslider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KDelayAudioProcessorEditor)
};
