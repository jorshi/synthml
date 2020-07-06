/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthMlAudioProcessorEditor::SynthMlAudioProcessorEditor (SynthMlAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    // Create a button that allows user to select an audio file for a sound match target
    auto& programmer = processor.getProgrammer();
    loadTargetButton.setButtonText("Load Audio Target");
    loadTargetButton.setBounds(100, 75, 100, 50);
    loadTargetButton.onClick = [&programmer] () { programmer.soundMatchFromFile(); };
    addAndMakeVisible(&loadTargetButton);
    
    setSize (300, 200);
}

SynthMlAudioProcessorEditor::~SynthMlAudioProcessorEditor()
{
}

//==============================================================================
void SynthMlAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void SynthMlAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
