/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzePlugin1AudioProcessor::KadenzePlugin1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Here is where we add parameters to the constructor
    
    // Add and initialize gain parameter
    addParameter(mGainParameter = new AudioParameterFloat("gain",
                                                          "Gain",
                                                          0.0f,
                                                          1.0f,
                                                          0.5f));
    
    //Set the smoothed gain float to same value as mGain
    mGainSmoothed = mGainParameter->get();
}

KadenzePlugin1AudioProcessor::~KadenzePlugin1AudioProcessor()
{
}

//==============================================================================
const String KadenzePlugin1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzePlugin1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzePlugin1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzePlugin1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzePlugin1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzePlugin1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzePlugin1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzePlugin1AudioProcessor::setCurrentProgram (int index)
{
}

const String KadenzePlugin1AudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzePlugin1AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzePlugin1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KadenzePlugin1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzePlugin1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

/*
*=============================== PROCESS BLOCK =======================================*
*/

void KadenzePlugin1AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    
    float* channelLeft = buffer.getWritePointer(0);
    float* channelRight = buffer.getWritePointer(1);
    
   
    
    /* The following for loop iterates through each sample in the buffer */
    
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        //smooth our gain variable as we iterate through each sample
        // Formula used here is smoothed value - scalar * (smoothed value - target value)
        mGainSmoothed = mGainSmoothed - 0.004 * (mGainSmoothed - mGainParameter->get());
        
        // Apply the gain to each sample in each channel in the buffer
        channelLeft[sample] *= mGainSmoothed;
        channelRight[sample] *= mGainSmoothed;
        
        // Apply same gain to each channel at the same time
        
    }
    
}

//==============================================================================
bool KadenzePlugin1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzePlugin1AudioProcessor::createEditor()
{
    return new KadenzePlugin1AudioProcessorEditor (*this);
//    return nullptr;
}

//==============================================================================
void KadenzePlugin1AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KadenzePlugin1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzePlugin1AudioProcessor();
}
