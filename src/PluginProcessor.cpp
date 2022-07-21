/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiViewerAudioProcessor::MidiViewerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MidiViewerAudioProcessor::~MidiViewerAudioProcessor()
{
}

//==============================================================================
const juce::String MidiViewerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiViewerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiViewerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiViewerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiViewerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiViewerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiViewerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiViewerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiViewerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiViewerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiViewerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiViewerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiViewerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void MidiViewerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
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
    // ...

    for(auto m = midiMessages.begin(); m != midiMessages.end(); ++m)
    {
        handleMidiMessage((*m).getMessage());
    }
}

void MidiViewerAudioProcessor::handleMidiMessage(const juce::MidiMessage& message){
    if(message.isNoteOn()){
        int note = message.getNoteNumber();
        if( note < 0 || note > 127){
            return;
        }

        // float pitch = (message.getNoteNumber() - 69) / 12.0f;
        float velocity = message.getVelocity() / 127.0f;
        notes[note] = {velocity};

        if( sustain ){
            for(auto itr = notes_to_stop.begin(); itr != notes_to_stop.end(); ++itr){
                if( *itr == note ){
                    notes_to_stop.erase(itr);
                    //break; //commenting out just in case note off's duplicate ig ??
                }
            }
        }
    }

    else if(message.isNoteOff()){
        int note = message.getNoteNumber();
        if( note < 0 || note > 127){
            return;
        }

        if( sustain ){
            notes_to_stop.push_back(note);
        }
        else{
            notes[note] = {0.0f};
        }
    }

    else if(message.isSustainPedalOn()){
        sustain = true;
    }
    else if(message.isSustainPedalOff()){
        sustain = false;
        for(auto note : notes_to_stop){
            notes[note] = {0.0f};
        }
    }

    else if(message.isPitchWheel()) {
        pitch_bend = message.getPitchWheelValue() / 8192.0f;
    }
}

//==============================================================================
bool MidiViewerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiViewerAudioProcessor::createEditor()
{
    return new MidiViewerAudioProcessorEditor ( *this );
}

//==============================================================================
void MidiViewerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiViewerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiViewerAudioProcessor();
}
