/*
  ==============================================================================

    Synth.h
    Created: 3 Jun 2020 5:52:46pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Patch.h"

class SynthPlugin
{
public:
    //==============================================================================
    SynthPlugin() = delete;
    SynthPlugin(AudioPluginInstance* p);
    ~SynthPlugin();
    
    const Array<AudioProcessorParameter*>& getParameters();
    AudioPluginInstance* getAudioPlugin() { return plugin.get(); };
    
    void setNewPatch(const Patch& newPatch);
    void updateParameter(int parameterIndex, const float& value);
    
private:
    //==============================================================================
    std::unique_ptr<AudioPluginInstance> plugin;
    std::map<int, AudioProcessorParameter*> synthParameters;
};
