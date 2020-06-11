/*
  ==============================================================================

    SpiegelibConnector.h
    Created: 5 Jun 2020 10:34:06pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "utils/json.hpp"

class SpiegelibConnector
{
public:
    
    enum ConnectionType
    {
        HTTP,
        SOCKET
    };
    
    //==============================================================================
    SpiegelibConnector(ConnectionType type=HTTP, String host="localhost", int port=8000);
    ~SpiegelibConnector();
    
    //==============================================================================
    /**
     * Attempt to make a GET request to SpiegeLib to get parameters to sound match
     * an audio file. The location of the audio file on disk is passed to SpiegeLib
     * in the GET query parameters
     */
    void soundMatchRequest(File target);
    
private:
    
    ConnectionType connectionType;
    String host;
    int port;
    URL url;
    
    std::unique_ptr<StreamingSocket> socket;
};
