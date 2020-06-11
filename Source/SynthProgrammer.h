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
#include "SpiegelibConnector.h"

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
    
private:
    //==============================================================================
    SpiegelibConnector spiegelib;
    std::unique_ptr<FileChooser> fileChooser;
};
