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
    // Loading a plugin here just for test. Ultimately this will be initiated from the UI
    auto newPlugin = synthPluginFactory.createSynthPluginFromPath("/Library/Audio/Plug-Ins/VST/Dexed.vst");
    synth.swap(newPlugin);
    
    // If you hit this the plugin wasn't loaded correctly. Make sure path above is correct.
    jassert(synth != nullptr);
    
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
        synth->setNewPatch(currentPatch);
}
