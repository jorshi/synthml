/*
  ==============================================================================

    Synth.h
    Created: 10 Jun 2020 2:23:46pm
    Author:  Colin Malloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "utils/json.hpp"

class Patch
{
public:
    //==============================================================================
    using Parameter = std::pair<int, float>;
    using PatchList = std::vector<Parameter>;
    
    //==============================================================================
    Patch();
    Patch(const String& patchJSON);
    ~Patch();
    
    const PatchList& getPatchValues() const;
    void setPatchValues(PatchList);
    
private:
    //==============================================================================
    PatchList patchValues;
};
