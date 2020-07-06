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
    jassert(p != nullptr);
    plugin.reset(p);
    
    // Setup a mapping to the paramaters keyed on parameter index
    auto parameters = plugin->getParameters();
    for (auto param = parameters.begin(); param != parameters.end(); ++param)
        synthParameters.insert(std::pair<int, AudioProcessorParameter*>((*param)->getParameterIndex(), *param));

}


SynthPlugin::~SynthPlugin()
{
    
}


const Array<AudioProcessorParameter*>& SynthPlugin::getParameters()
{
    return plugin->getParameters();
}


void SynthPlugin::setNewPatch(const Patch& newPatch)
{
    auto patch = newPatch.getPatchValues();
    for (auto param = patch.begin(); param != patch.end(); ++param)
        updateParameter(param->first, param->second);
}


void SynthPlugin::updateParameter(int parameterIndex, const float& value)
{
    auto parameter = synthParameters.find(parameterIndex);
    if (parameter != synthParameters.end())
    {
        float clampedValue = std::min(std::max(value, 0.0f), 1.0f);
        parameter->second->setValue(clampedValue);
    }
    
}
