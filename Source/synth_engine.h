#pragma once

#include "../lib/synthesizer.h"

#include "../JuceLibraryCode/JuceHeader.h"

class SynthEngine {

 public:
  enum SynthChannels {
    synth_channel_L,
    synth_channel_R,
    synth_num_channels,
  };

  SynthEngine();
  ~SynthEngine();

  void setPlayer(double sample_rate, int buffer_size);

  void resetBuffer();

  void processMidi(MidiBuffer& midiMessages);

  void processAudio(AudioSampleBuffer& buffer);

 private:
  double sample_rate_;
  int buffer_size_;

  astellon::Synthesizer synth_;
};
