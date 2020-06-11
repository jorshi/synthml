/*
  ==============================================================================

    SynthProgrammer.cpp
    Created: 5 Jun 2020 10:33:37pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SynthProgrammer.h"

SynthProgrammer::SynthProgrammer()
{
    fileChooser = std::make_unique<FileChooser>("Select an audio file to sound match");
}


void SynthProgrammer::soundMatchFromFile()
{
    if (fileChooser->browseForFileToOpen())
    {
        File file(fileChooser->getResult());
        spiegelib.soundMatchRequest(file);
    }
}
