/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzePlugin1AudioProcessorEditor::KadenzePlugin1AudioProcessorEditor (KadenzePlugin1AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    
    //Access the parameters
    auto& params = processor.getParameters();
    //get the gain parameter
    AudioParameterFloat* gainParameter = (AudioParameterFloat*)params.getUnchecked(0);
    
    // Add other UI elements below
    
    /************** Add a slider for Gain **********************/
    
    // Add a control slider for gain control
    // Set the size and location of the slider
    mGainControlSlider.setBounds(0,0, 100, 100);
    
    // Set the styling of the slider
    mGainControlSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    
    // Set the textbox style
    mGainControlSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    // Set the slider min and max values to the parameter min and max values
    mGainControlSlider.setRange(gainParameter->range.start, gainParameter->range.end);
    
    // Set the value of the slider to the value of the gain parameter
    mGainControlSlider.setValue(*gainParameter);
    
    mGainControlSlider.onDragStart = [gainParameter]
    {
        gainParameter->beginChangeGesture();
    };
    
    // We can construct the method for connecting the slider value to the gain value
    // In line as we construct the slider. We don't have to create a new function
    mGainControlSlider.onValueChange = [this, gainParameter]
    {
        *gainParameter = mGainControlSlider.getValue();
    };
    
    mGainControlSlider.onDragEnd = [gainParameter]
    {
        gainParameter->endChangeGesture();
    };
    
    //Actually put the slider in the UI
    addAndMakeVisible(mGainControlSlider);
    
}

KadenzePlugin1AudioProcessorEditor::~KadenzePlugin1AudioProcessorEditor()
{
}

//==============================================================================
void KadenzePlugin1AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void KadenzePlugin1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

