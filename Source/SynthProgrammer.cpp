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
    if (message.size() > 0 && message[0].isString())
        currentPatch = Patch(message[0].getString());
    
    if (synth != nullptr)
    {
        synth->setNewPatch(currentPatch);
        DBG("Successfully loaded new patch");
    }
}


bool SynthProgrammer::loadSynthFromPath(String path)
{
    auto newPlugin = synthPluginFactory.createSynthPluginFromPath(path);
    
    // If nullptr then the plugin wasn't loaded successfully
    if (newPlugin == nullptr)
        return false;
    
    // Transfer plugin ownership to the shared pointer in this SynthProgrammer
    synth.reset(newPlugin.release());
    if (synthLoadedCallback) synthLoadedCallback();
    
    return true;
}
