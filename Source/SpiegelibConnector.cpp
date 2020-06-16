/*
  ==============================================================================

    SpiegelibConnector.cpp
    Created: 5 Jun 2020 10:34:06pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "SpiegelibConnector.h"

//==============================================================================
SpiegelibConnector::SpiegelibConnector(String host, int receivePort, int sendPort)
: host(host), receivePort(receivePort), sendPort(sendPort), isConnected(false)
{
    oscReceiver.addListener(this);
}


SpiegelibConnector::~SpiegelibConnector()
{
    oscSender.disconnect();
    oscReceiver.disconnect();
}


//==============================================================================
bool SpiegelibConnector::connect()
{
    oscSender.disconnect();
    oscReceiver.disconnect();
    isConnected = false;
    
    if (!oscSender.connect(host, sendPort))
    {
        return false;
    }
    
    if (!oscReceiver.connect(receivePort))
    {
        oscSender.disconnect();
        return false;
    }
    
    isConnected = true;
    return true;
}


bool SpiegelibConnector::soundMatchRequest(File target)
{
    if (!isConnected) return false;
    
    OSCMessage oscMsg(OSCAddressPattern("/sound_match"), target.getFullPathName());
    if (oscSender.sendToIPAddress(host, sendPort, oscMsg))
        return true;
    
    return false;
}


void SpiegelibConnector::oscMessageReceived(const OSCMessage& message)
{
    String address = message.getAddressPattern().toString();
    auto callback = callbacks.find(address);
    
    if (callback != callbacks.end())
    {
        callback->second(message);
    }
}


void SpiegelibConnector::registerCallback(const String &address, OSCCallback callback)
{
    auto currentCallback = callbacks.find(address);
    
    // Callback has not been registered if find returns a pointer to the end of the map
    if (currentCallback == callbacks.end())
    {
        callbacks.insert(std::pair<String, OSCCallback>(address, callback));
    }
    
    // Otherwise, replace the currently registered callback
    else
    {
        callbacks.erase(currentCallback);
        callbacks.insert(std::pair<String, OSCCallback>(address, callback));
    }
}
