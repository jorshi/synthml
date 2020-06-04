/*
  ==============================================================================

    Synth.h
    Created: 3 Jun 2020 5:52:46pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthPlugin
{
public:
    //==============================================================================
    SynthPlugin(AudioPluginInstance* p);
    ~SynthPlugin();
    
private:
    //==============================================================================
    std::unique_ptr<AudioPluginInstance> plugin;
    
};
