/*
  ==============================================================================

    Synth.h
    Created: 10 Jun 2020 2:23:46pm
    Author:  Colin Malloy

  ==============================================================================
*/

#include "Patch.h"

Patch::Patch()
{
    
}

Patch::~Patch()
{
    
}

void Patch::setPatchValues(std::vector<std::pair<int, float>> newPatchValues) {
    patchValues = newPatchValues;
}

std::vector<std::pair<int, float>> Patch::getPatchValues() {
    return patchValues;
}
