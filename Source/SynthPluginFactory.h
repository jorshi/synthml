/*
  ==============================================================================

    SynthPluginFactory.h
    Created: 3 Jun 2020 6:00:11pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthPlugin.h"

class SynthPluginFactory
{
public:
    //==============================================================================
    SynthPluginFactory();
    ~SynthPluginFactory();
    
    std::unique_ptr<SynthPlugin> createSynthPluginFromPath(String path, double sampleRate=44100.0, int maxBlockSize=1024);
    
    
private:
    //==============================================================================
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList pluginList;
    AudioPluginFormatManager pluginFormatManager;
};
