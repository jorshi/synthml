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
     * When called will open a file browser allowing the user to select
     * an audio file to be used for sound matching. Will create a new request
     * to SpiegeLib to get parameters to sound match that audio file.
     */
    void soundMatchFromFile();
    
    /**
     * Set new patch from OSC message
     */
    void setPatch(const OSCMessage& message);
    
private:
    //==============================================================================
    SynthPluginFactory synthPluginFactory;
    std::unique_ptr<SynthPlugin> synth;

    Patch currentPatch;
    SpiegelibConnector spiegelib;
    std::unique_ptr<FileChooser> fileChooser;
};
