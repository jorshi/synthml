#!/usr/bin/env python
"""
Sound match import file
"""

import os
import spiegelib as spgl
from spiegelib.estimator import TFEstimatorBase

def load(root):
    """
    Creates a SoundMatch object and returns it
    """

    # Load the synthesizer state, which includes the list of overridden
    # parameters. This allows us sound match to skip loading the actual VST
    synth_state = os.path.join(root, 'dexed_simple_fm.json')

    # Setup BiLSTM TensorFlow model
    model_dir = os.path.join(root, 'simple_fm_bi_lstm.h5')
    bi_lstm = TFEstimatorBase.load(model_dir)

    # LSTM & LSTM++ feature extractor -- time series of MFCC frames
    lstm_extractor = spgl.features.MFCC(num_mfccs=13, time_major=True,
                                        hop_size=1024, scale=True)
    scaler = os.path.join(root, 'data_scaler.pkl')
    lstm_extractor.load_scaler(scaler)

    # Add an input modifer to slice the input audio to the correct size
    def resize_audio(audio, new_size):
        audio.resize(new_size)
        return audio

    input_segment = lambda audio: resize_audio(audio, 44544)
    lstm_extractor.add_modifier(input_segment, 'input')

    # Create the sound matcher
    sound_match = spgl.SoundMatch(synth_state, bi_lstm, lstm_extractor)

    return sound_match
