/*
  ==============================================================================

    SynthPlugin.cpp
    Created: 3 Jun 2020 5:52:46pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SynthPlugin.h"

SynthPlugin::SynthPlugin(AudioPluginInstance* p)
{
    plugin.reset(p);
}


SynthPlugin::~SynthPlugin()
{
    
}
