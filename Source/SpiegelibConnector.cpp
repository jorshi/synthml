/*
  ==============================================================================

    SpiegelibConnector.cpp
    Created: 5 Jun 2020 10:34:06pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SpiegelibConnector.h"

//==============================================================================
SpiegelibConnector::SpiegelibConnector(String host, String port) : host(host), port(port)
{
    url = URL("http://" + host + ":" + port);
}


SpiegelibConnector::~SpiegelibConnector()
{
    
}


//==============================================================================
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
        if (statusCode == 200)
        {
            DBG(inputStream->readEntireStreamAsString());
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
