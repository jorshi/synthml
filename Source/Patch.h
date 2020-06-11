/*
  ==============================================================================

    Synth.h
    Created: 10 Jun 2020 2:23:46pm
    Author:  Colin Malloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Patch
{
public:
    //==============================================================================
    Patch();
    ~Patch();
    
    std::vector<std::pair<int, float>> getPatchValues();
    void setPatchValues(std::vector<std::pair<int, float>>);
    
private:
    //==============================================================================
    std::vector<std::pair<int, float>> patchValues;
};
