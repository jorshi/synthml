/*
  ==============================================================================

    SynthProgrammer.cpp
    Created: 5 Jun 2020 10:33:37pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SynthProgrammer.h"

SynthProgrammer::SynthProgrammer()
{
    fileChooser = std::make_unique<FileChooser>("Select an audio file to sound match");
    
    // Connect to spiegelib and register callbacks for specific OSC messages
    spiegelib.connect();
    spiegelib.registerCallback("/sound_match_success", [this](const OSCMessage& message){ this->setPatch(message); });
}


void SynthProgrammer::soundMatchFromFile()
{
    if (fileChooser->browseForFileToOpen())
    {
        File file(fileChooser->getResult());
        spiegelib.soundMatchRequest(file);
    }
}

void SynthProgrammer::setPatch(const OSCMessage& message)
{
    DBG(message[0].getString());
}
