#include "synth_engine.h"

using namespace astellon;

SynthEngine::SynthEngine() {}
SynthEngine::~SynthEngine() {}

void SynthEngine::setPlayer(double sample_rate, int buffer_size) {
  synth_.setPlayer(sample_rate, buffer_size);
  sample_rate_ = sample_rate;
  buffer_size_ = buffer_size;
}

void SynthEngine::resetBuffer() {
  synth_.resetBuffer();
}

void SynthEngine::processMidi(MidiBuffer& midiMessages) {
  //process midi
  MidiMessage midi_message;
  int sample_ofset;
  for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(midi_message, sample_ofset);) {
    if (midi_message.isNoteOn()) {
      int note = midi_message.getNoteNumber();
      int velocity = midi_message.getVelocity();
      synth_.noteOn(note, velocity);
    } else if (midi_message.isNoteOff()) {
      int note = midi_message.getNoteNumber();
      synth_.noteOff(note);
    }
  }
}

void SynthEngine::processAudio(AudioSampleBuffer& buffer) {
  synth_.process();
  for (int channel = 0; channel < 2; channel++) {
    const AudioType* read = synth_.getOutput(channel)->getReadPointer();
    auto write = buffer.getWritePointer(channel);
    for (int index = 0; index < buffer_size_; index++) {
      write[index] = read[index];
    }
  }
}
