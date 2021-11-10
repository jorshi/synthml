/*
  ==============================================================================

    SynthPluginFactory.cpp
    Created: 3 Jun 2020 6:00:11pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SynthPluginFactory.h"

SynthPluginFactory::SynthPluginFactory()
{
    pluginFormatManager.addDefaultFormats();
}


SynthPluginFactory::~SynthPluginFactory()
{
    
}


std::unique_ptr<SynthPlugin> SynthPluginFactory::createSynthPluginFromPath(String path, double sampleRate, int maxBlockSize)
{
    OwnedArray<PluginDescription> newPluginList;
    // Check for plugin given by path description and add to the plugin list
    for (int i = pluginFormatManager.getNumFormats(); --i >= 0;)
        pluginList.scanAndAddFile(String(path), false, newPluginList, *pluginFormatManager.getFormat(i));

    if (newPluginList.size() == 0)
        return nullptr;
    
    // Create a new instance of plugin
    String error;
    auto* desc = newPluginList[0];
    auto plugin = pluginFormatManager.createPluginInstance (*desc, sampleRate, maxBlockSize, error);
    
    if (plugin == nullptr)
        DBG(error);
    
    // Create a new SynthPlugin object with loaded plugin and return
    auto synthPlugin = std::make_unique<SynthPlugin>(plugin.release());
    return synthPlugin;
}
