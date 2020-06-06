/*
  ==============================================================================

    SpiegelibConnector.h
    Created: 5 Jun 2020 10:34:06pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SpiegelibConnector
{
public:
    //==============================================================================
    SpiegelibConnector(String host="localhost", String port="8000");
    ~SpiegelibConnector();
    
    //==============================================================================
    void soundMatchRequest(File target);
    
private:
    
    String host;
    String port;
    URL url;
};
