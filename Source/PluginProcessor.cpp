/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthMlAudioProcessor::SynthMlAudioProcessor()
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
    programmer.synthLoadedCallback = [this]() { this->prepareNewSynth(); };
}

SynthMlAudioProcessor::~SynthMlAudioProcessor()
{
}

//==============================================================================
const String SynthMlAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthMlAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthMlAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthMlAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthMlAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthMlAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthMlAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthMlAudioProcessor::setCurrentProgram (int index)
{
}

const String SynthMlAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthMlAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SynthMlAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    bool loadedSynth = programmer.loadSynthFromPath("/Library/Audio/Plug-Ins/VST/Dexed.vst",
                                                    sampleRate,
                                                    samplesPerBlock);
    // Wasn't able to load synth - make sure the path above is correct
    jassert(loadedSynth);
    
    if (queuedSynth)
        queuedSynth->getAudioPlugin()->prepareToPlay(sampleRate, samplesPerBlock);
    
    if (currentSynth)
        currentSynth->getAudioPlugin()->prepareToPlay(sampleRate, samplesPerBlock);
}

void SynthMlAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthMlAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SynthMlAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    if (midiMessages.getNumEvents())
        DBG("Midi press");
    
    // Update the current synthesizer plugin
    updateCurrentSynth();
    if (currentSynth)
    {
        auto* plugin = currentSynth->getAudioPlugin();
        plugin->processBlock(buffer, midiMessages);
    }
}

//==============================================================================
bool SynthMlAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SynthMlAudioProcessor::createEditor()
{
    return new SynthMlAudioProcessorEditor (*this);
}

//==============================================================================
void SynthMlAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthMlAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
void SynthMlAudioProcessor::prepareNewSynth()
{
    std::shared_ptr<SynthPlugin> newSynth;
    if ((newSynth = programmer.getSynth()))
    {
        auto* plugin = newSynth->getAudioPlugin();
        plugin->prepareToPlay(this->getSampleRate(), this->getBlockSize());
        queuedSynth = newSynth;
    }
}

void SynthMlAudioProcessor::updateCurrentSynth()
{
    if (queuedSynth)
    {
        currentSynth = queuedSynth;
        queuedSynth.reset();
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthMlAudioProcessor();
}
