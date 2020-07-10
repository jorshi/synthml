/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthPlugin.h"
#include "SynthPluginFactory.h"
#include "SynthProgrammer.h"
#include "SpiegelibConnector.h"

//==============================================================================
/**
*/
class SynthMlAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SynthMlAudioProcessor();
    ~SynthMlAudioProcessor();

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
    SynthProgrammer& getProgrammer() { return programmer; };
    void prepareNewSynth();
    void updateCurrentSynth();

private:
    //==============================================================================
    
    SynthProgrammer programmer;
    std::shared_ptr<SynthPlugin> currentSynth;
    std::shared_ptr<SynthPlugin> queuedSynth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthMlAudioProcessor)
};
