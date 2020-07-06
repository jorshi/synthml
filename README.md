# SynthML

An audio plug-in for automatic synthesizer programming using machine learning.

Goal of this project is to develop an audio plugin that can host AU/VST audio synthesizer plugins and automatically program sounds into those synthesizers through sound matching. User's will be able to drop an example sound file into the user interface and a pre-trained deep learning model will estimate parameters to match that sound as closely as possible.

## Installation

Installation instructions will be updated throughout development.

Currently, only JUCE and the VST3/VST2 SDK are required for development. VST SDKs are included in the project folder and will link automatically. Python 3.6+ and SpiegeLib are required for running a localhost server that is used to run machine learning algorithms in the backend using Python. SynthML communicates with SpiegeLib via OSC. 

### SynthML Plugin

1) Download and install JUCE https://shop.juce.com/get-juce

2) Clone this repo: `git clone https://github.com/jorshi/synthml.git`

3) Enter the cloned project and open SynthML.jucer

4) Open and build in Xcode (or whatever IDE you are working in)

### SpiegeLib for OSC Server

1) Make sure you have Python 3.6+ installed

2) It is recommended to install SpiegeLib in a virtualenv or anaconda environment. Virtualenv is easiest if you are planning on just running existing algorithms in SpiegeLib. An Anaconda environment is preferable if you are planning on doing any development with SpiegeLib.

  * For virtualenv, create a new virtual environment and activate:
    ```
    python3 -m venv ./venv
    source ./venv/bin/activate
    ```
  
    To leave the virtualenv use ```deactivate```
   
   * For anaconda, please see detailed instructions on [SpiegeLib documentation](https://spiegelib.github.io/spiegelib/getting_started/installation.html)
 
 3) Install SpiegeLib in your environment using pip: ```pip install spiegelib```
 
  * Note, there is one required packaged for SpiegeLib that will not be installed via pip called RenderMan. RenderMan is **NOT** required to run the server for SynthML so don't worry if you are just planning on running SpiegeLib for this project. The pip installation will be adequate.
  
  * If you are interested in creating or training new models/algorithms for SynthML, please visit [SpiegeLib documentation](https://spiegelib.github.io/spiegelib/) for more information on using the library.
  
## Running with example SpiegeLib server

1) The example runs sound matching on Dexed. Install that [from here](https://asb2m10.github.io/dexed/).

2) cd into the SynthML project example directoy, this contains a sound match bundle with a trained bidirectional LSTM model for running sound matching on Dexed. A sound match bundle is just a folder containing a loader.py file and the necessary data files to create a new SoundMatch object that can perform synthesizer sound matching within SpiegeLib. loader.py is responsible for instantiating the SoundMatch object. The idea is that a sound match bundle can be created for any synthesizer plugin using any number of machine learning techniques and then these can be easily switched out for use with SynthML.

3) Start the OSC sound match server with the location of the sound match bundle as an argument. synthml.server is a command line tool that is installed with SpiegeLib that reads a sound match bundle and starts a server that listens for OSC messages. OSC messages contain a reference to an audio file to perform sound matching on. The server runs sound matching as defined in the sound match bundle and returns the resulting synthesizer parameters for that sound.

```synthml.server bilstm_sound_matcher```

  * Note, you may get the following error: *ModuleNotFoundError: No module named 'numba.decorators'*. This is a conflict with the current version of librosa and can be fixed by downgrading numba: ```pip install numba==0.48```

4) Start SynthML and test sound matching an audio file. The python server should acknowledge communication.

5) To change the ports that OSC messages are sending and receiving on run the server command with the following arguments:

```synthml.server bilstm_sound_matcher -r 9001 -s 9002```

  * Where 9001 is the port that the sound match server is receiving on and 9002 is the send port. These are the default values and can be changed as needed.
  * These port values are currently hardcoded in SynthML as default parameters in the SpiegelConnector class and will need to be adjusted manually. 

## Contributing

Please develop in your own branch and then create a pull request for your change when you are finished with your change.



