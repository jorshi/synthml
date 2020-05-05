# SynthML

An audio plug-in for automatic synthesizer programming using machine learning.

Goal of this project is to develop an audio plugin that can host AU/VST audio synthesizer plugins and automatically program sounds into those synthesizers through sound matching. User's will be able to drop an example sound file into the user interface and a pre-trained deep learning model will estimate parameters to match that sound as closely as possible.

## Installation

Installation instructions will be updated throughout development.

Currently, only JUCE and the VST3 SDK are required for development.

1) Download and install JUCE https://shop.juce.com/get-juce

2) Download the VST3 SDK : https://www.steinberg.net/en/company/developers.html - Save folder in location were it will be handy, you will need to link to this later.

3) Clone this repo: `git clone https://github.com/jorshi/synthml.git`

4) Enter the cloned project and open SynthML.jucer

5) Make sure you set the VST3SDK path in the export tab

6) Open and build in Xcode (or whatever IDE you are working in)

## Contributing

Please develop in your own branch and then create a pull request for your change when you are finished with your change.



