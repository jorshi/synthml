/*
  ==============================================================================

    SpiegelibConnector.cpp
    Created: 5 Jun 2020 10:34:06pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SpiegelibConnector.h"

//==============================================================================
SpiegelibConnector::SpiegelibConnector(ConnectionType type, String host, int port)
: connectionType(type), host(host), port(port)
{
    if (connectionType == HTTP)
        url = URL("http://" + host + ":" + String(port));

    else if (connectionType == SOCKET)
    {
        socket = std::make_unique<StreamingSocket>();
        if (socket->connect(host, port))
            DBG("Successfully connected to SpiegeLib");
        
        else
            DBG("Unable to connect to SpiegeLib");
    }
    
}


SpiegelibConnector::~SpiegelibConnector()
{
    
}


//==============================================================================
// TODO this should return true or false depending on the success of the request
void SpiegelibConnector::soundMatchRequest(File target)
{
    URL requestURL = url.withNewSubPath("sound_match");
    requestURL = requestURL.withParameter("target", target.getFullPathName());
    
    StringPairArray responseHeaders;
    int statusCode = 0;
    std::unique_ptr<InputStream> inputStream(requestURL.createInputStream(false, nullptr, nullptr, String(),
                                                                          -1, &responseHeaders, &statusCode));

    if (inputStream != nullptr)
    {
        // Successfully retrieved sound match parameters
        // TODO figure out how to parse this into a Patch
        if (statusCode == 200)
        {
            auto result = inputStream->readEntireStreamAsString();
            DBG(result);
            auto json = nlohmann::json::parse(result.toStdString());
            auto patch = json.at("patch");
        }
        
        // Failed retrieving sound match parameters
        else
        {
            DBG(statusCode);
            DBG(inputStream->readEntireStreamAsString());
        }
    }
    
    // Wasn't able to connect to server
    else
    {
        DBG("Failed to connect to SpiegeLib server");
    }
}
