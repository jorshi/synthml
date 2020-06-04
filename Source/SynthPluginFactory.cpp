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
    // Check for plugin given by path description and add to the plugin list
    for (int i = pluginFormatManager.getNumFormats(); --i >= 0;)
        pluginList.scanAndAddFile(String(path), false, pluginDescriptions, *pluginFormatManager.getFormat(i));
    
    // Try to get the plugin description for the requested plugin
    auto typeForFile = pluginList.getTypeForFile(path);
    if (typeForFile == nullptr)
        throw "Plugin not found.";

    // Create a new instance of plugin
    String error;
    auto plugin = pluginFormatManager.createPluginInstance (*typeForFile, sampleRate, maxBlockSize, error);
    
    if (plugin == nullptr)
        throw error;
    
    // Create a new SynthPlugin object with loaded plugin and return
    auto synthPlugin = std::make_unique<SynthPlugin>(plugin.release());
    return synthPlugin;
}
