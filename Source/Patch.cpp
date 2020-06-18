/*
  ==============================================================================

    Synth.h
    Created: 10 Jun 2020 2:23:46pm
    Author:  Colin Malloy

  ==============================================================================
*/

#include "Patch.h"
using json = nlohmann::json;

Patch::Patch()
{
    
}


Patch::Patch(const String& patchJSON)
{
    auto parsed = json::parse(patchJSON.toStdString());
    auto patchStart = parsed.find("patch");
    
    if (patchStart != parsed.end())
        patchValues = patchStart->get<PatchList>();
}


Patch::~Patch()
{
    
}


void Patch::setPatchValues(PatchList newPatchValues)
{
    patchValues = newPatchValues;
}


const Patch::PatchList& Patch::getPatchValues() const
{
    return patchValues;
}
