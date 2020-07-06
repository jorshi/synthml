/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SynthProgrammer.h"

//==============================================================================
/**
*/
class SynthMlAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SynthMlAudioProcessorEditor (SynthMlAudioProcessor&);
    ~SynthMlAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthMlAudioProcessor& processor;
    
    TextButton loadTargetButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthMlAudioProcessorEditor)
};
