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


class SpiegelibConnector : public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    // Callback function type
    using OSCCallback = std::function<void(const OSCMessage&)>;
    
    //==============================================================================
    SpiegelibConnector(String host="127.0.0.1", int receivePort=9002, int sendPort=9001);
    ~SpiegelibConnector();
    
    //==============================================================================
    /**
     * Attempt to connect to receiving and sending ports. Returns boolean indicating success.
     */
    bool connect();
    
    /**
     * Attempt to make a GET request to SpiegeLib to get parameters to sound match
     * an audio file. The location of the audio file on disk is passed to SpiegeLib
     * in the GET query parameters
     */
    bool soundMatchRequest(File target);
    
    /**
     * Add a callback associated to an OSC address. Will replace a callback for that
     * address is one has already been added.
     */
    void registerCallback(const String& address, OSCCallback callback);
    
    /**
     * Overrides the OSCReceiver base class method and is called when an OSC message
     * is recieved on the specified port. Will attempt to find a registered callback
     * function and pass the OSC message to that callback.
     */
    void oscMessageReceived(const OSCMessage& message) override;
    
private:
    //==============================================================================
    String host;
    int receivePort;
    int sendPort;
    
    OSCSender oscSender;
    OSCReceiver oscReceiver;
    bool isConnected;
    
    std::map<String, OSCCallback> callbacks;
};
