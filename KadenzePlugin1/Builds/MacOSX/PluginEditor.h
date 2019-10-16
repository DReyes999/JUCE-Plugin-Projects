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
class KadenzePlugin1AudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzePlugin1AudioProcessorEditor (KadenzePlugin1AudioProcessor&);
    ~KadenzePlugin1AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    

private:
    
    // Add a slider to the class
    Slider mGainControlSlider;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KadenzePlugin1AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzePlugin1AudioProcessorEditor)
};
