/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include <memory>

//==============================================================================
KDelayAudioProcessor::KDelayAudioProcessor()
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
    /*===============================*/
    //Initialize AudioParameters
    
    addParameter (mDryWetParameter = new AudioParameterFloat("drywet",
                                                   "Dry Wet",
                                                   0.0,
                                                   1.0,
                                                   0.5));
    
    addParameter (mFeedbackParameter = new AudioParameterFloat("feedback",
                                                      "Feedback",
                                                      0,
                                                      0.98,
                                                      0.5));
    
    addParameter (mDelayTimeParameter = new AudioParameterFloat("delaytime",
                                                                "Delay Time",
                                                                0.01,
                                                                MAX_DELAY_TIME,
                                                                0.5));
    
    
    
    /*===============================*/
    
    //Initialize delay buffers to be nullptr
    //We want them to be nullptr because we don't know our sample rate
    //And we're not ready to instantiate the audio data
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    mDelayReadHead = 0;
    mDelayTimeInSamples = 0;
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    mDelayTimeSmoothed = 0;

}

KDelayAudioProcessor::~KDelayAudioProcessor()
{
    if (mCircularBufferLeft!=nullptr) {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight!=nullptr) {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const String KDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String KDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void KDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    /*============ Set the Delay Time ===========*/
    
    mDelayTimeInSamples = sampleRate * *mDelayTimeParameter;
    
    /*============ Initialize the length of the circular buffers ===========*/
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    /*============ Reset the circular buffers ===========*/
    
//    mCircularBufferLeft.reset(new float[mCircularBufferLength]);
//    mCircularBufferLeft.reset(new float[mCircularBufferLength]);
    
    if (mCircularBufferLeft==nullptr) {
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    
    // Clean out the buffers
    zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    
    if (mCircularBufferRight==nullptr) {
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    
    zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));
    
    
    
    /*============ Initialize the write head ===========*/
    mCircularBufferWriteHead = 0;
    
    //Initialize smoothing parameter
    mDelayTimeSmoothed = *mDelayTimeParameter;
}

void KDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    

    // Create the write head variables to process the samples in the buffer
    // This gives us access to the data in the buffer
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);

    /*=======================*/
    // For every sample in the buffer, go through them one by one
    /*=======================*/
    
    for (int i = 0 ; i < buffer.getNumSamples(); i++)
    {
        //Setup the smoothed delay time parameter
        mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001 * (mDelayTimeSmoothed - *mDelayTimeParameter);
        mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;
        
        
        /*=======================*/
         /*
          For every incoming sample from the buffer, assign it to the current write head position of the
          circular buffer
         */
         /*=======================*/
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        /*========= Get the Read Head Position ==============*/
        
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        
        /*=======================*/
        /*
         Here we add the data from the circular buffer into the output signal
         */
        /*=======================*/
        
        
        
        // Check to make sure the read head is not out of bounds of the buffer
        if (mDelayReadHead < 0)
        {
            mDelayReadHead += mCircularBufferLength;
        }
        /*=======================*/
        //Linear interpolation
        //Get the position of the read head and find out what portion is after the decimal
        
        int readHead_x = (int)mDelayReadHead;              //Represents integer value. x0(sample_x) value of Lerp function
        int readHead_x1 = readHead_x + 1;
        
        //Make sure readHead_x1 does not wrap around the buffer.
        if (readHead_x1 > mCircularBufferLength)
        {
            readHead_x1 -= mCircularBufferLength;
        }
        
        float readHeadfloat = mDelayReadHead - readHead_x;   // Represents decimal remainder of that value. inPhase value of Lerp function
        
        /*=======================*/
        // Get the samples at the delay read head
        // These are our output samples
        /*=======================*/
        
        float delay_sample_left =
            lin_interp(mCircularBufferLeft[readHead_x],mCircularBufferLeft[readHead_x1], readHeadfloat);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHead_x],mCircularBufferRight[readHead_x1], readHeadfloat);
        
        /*=======================*/
        //Feedback processing
        //We use a scalar to lower the gain on the delay samples each time
        /*=======================*/
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;
        
        /*=======================*/
        /*
         Each time we write to the circular buffer we need to increment the write head
         However, because this is a circular buffer we need to check if the write head is at the end of the
         circular buffer. If it is, then the write head should go back to the beginning position.
         */
        /*=======================*/
        
        //Each time we write to the circular buffer we need to increment the write head
        mCircularBufferWriteHead++;
        
        /*=======================*/
        
        buffer.addSample(0, i, buffer.getSample(0, i) * (1 - *mDryWetParameter) + delay_sample_left *  *mDryWetParameter);
        
        buffer.addSample(1, i, buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right *  *mDryWetParameter);
        
        /*=======================*/
       /*
        Each time we write to the circular buffer we need to increment the write head
        However, because this is a circular buffer we need to check if the write head is at the end of the
        circular buffer. If it is, then the write head should go back to the beginning position.
        */
       /*=======================*/
        
        if (mCircularBufferWriteHead >= mCircularBufferLength)
        {
            mCircularBufferWriteHead = 0;
        }
        
    }
}

//==============================================================================
bool KDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KDelayAudioProcessor::createEditor()
{
    return new KDelayAudioProcessorEditor (*this);
}

//==============================================================================
void KDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KDelayAudioProcessor();
}
//==============================================================================
//My function definitions below
//==============================================================================

//Lerp function
float KDelayAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase)
{
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
