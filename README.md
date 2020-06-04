# SynthML

An audio plug-in for automatic synthesizer programming using machine learning.

Goal of this project is to develop an audio plugin that can host AU/VST audio synthesizer plugins and automatically program sounds into those synthesizers through sound matching. User's will be able to drop an example sound file into the user interface and a pre-trained deep learning model will estimate parameters to match that sound as closely as possible.

## Installation

Installation instructions will be updated throughout development.

Currently, only JUCE and the VST3/VST2 SDK are required for development. VST SDKs are included in the project folder and will link automatically.

1) Download and install JUCE https://shop.juce.com/get-juce

2) Clone this repo: `git clone https://github.com/jorshi/synthml.git`

3) Enter the cloned project and open SynthML.jucer

4) Open and build in Xcode (or whatever IDE you are working in)

## Contributing

Please develop in your own branch and then create a pull request for your change when you are finished with your change.



