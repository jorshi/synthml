/*
  ==============================================================================

    SynthProgrammer.h
    Created: 5 Jun 2020 10:33:37pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthPlugin.h"
#include "SynthPluginFactory.h"
#include "SpiegelibConnector.h"
#include "Patch.h"

class SynthProgrammer
{
public:
    //==============================================================================
    SynthProgrammer();
    ~SynthProgrammer() {};
    
    /**
     * Get a raw pointer to the current synthesizer. Will be a nullptr if there is no
     * current active synth
     */
    SynthPlugin* getSynthRaw() { return synth.get(); };
    
    /**
     * Get a smart pointer to the current synthesizer.
     */
    std::shared_ptr<SynthPlugin> getSynth() { return synth; };
   
    /**
     * When called will open a file browser allowing the user to select
     * an audio file to be used for sound matching. Will create a new request
     * to SpiegeLib to get parameters to sound match that audio file.
     */
    void soundMatchFromFile();
    
    /**
     * Set new patch from OSC message
     */
    void setPatch(const OSCMessage& message);
    
    
    bool loadSynthFromPath(String path, double sampleRate=44100.0, int maxBlockSize=512);
    
    /**
     * Callback function when a new synthesizer has been loaded
     */
    std::function<void()> synthLoadedCallback;
    
private:
    //==============================================================================
    SynthPluginFactory synthPluginFactory;
    std::shared_ptr<SynthPlugin> synth;

    Patch currentPatch;
    SpiegelibConnector spiegelib;
    std::unique_ptr<FileChooser> fileChooser;
    
};
