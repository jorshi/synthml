#!/usr/bin/env python
"""
Sound match import file
"""

import os
import spiegelib as spgl
import spiegelib.features.vggish
from spiegelib.estimator import TFEstimatorBase

def load(root):
    """
    Creates a SoundMatch object and returns it
    """

    # Load the synthesizer state, which includes the list of overridden
    # parameters. This allows sound match to skip loading the actual VST
    synth_state = os.path.join(root, 'dexed_simple_fm.json')

    # Setup Conv5s TensorFlow model
    model_dir = os.path.join(root, 'conv5s.h5')
    conv5s = TFEstimatorBase.load(model_dir)

    # Mel Spectrogram Extractor
    features = spgl.features.vggish.VGGishMelInput(time_major=True, scale=True)
    features.add_modifier(lambda data: data.reshape(data.shape[0], data.shape[2]), type='output')
    features.add_modifier(lambda data: data.reshape(data.shape[0], data.shape[1], 1), type='output')
    scaler = os.path.join(root, 'data_scaler.pkl')
    features.load_scaler(scaler)

    # Add an input modifier to slice the input audio to the correct size
    def resize_audio(audio, new_size):
        audio.resize(new_size)
        return audio

    input_segment = lambda audio: resize_audio(audio, 65024)
    features.add_modifier(input_segment, 'input')

    # Create the sound matcher
    sound_match = spgl.SoundMatch(synth_state, conv5s, features)

    return sound_match
