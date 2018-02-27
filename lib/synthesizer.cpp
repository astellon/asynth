#include "synthesizer.h"

namespace astellon {

Synthesizer::Synthesizer() : Module(0, 2, 0), voice_handler_(DEFAULT_NUM_VOICES) {}

void Synthesizer::setPlayer(double sample_rate, int buffer_size) {
  voice_handler_.setPlayer(sample_rate, buffer_size);
  Module::setPlayer(sample_rate, buffer_size);
}

void Synthesizer::setNumVoice(int num) {
  voice_handler_.resizeVoice(num);
}

int Synthesizer::getNumVoice() const {
  return voice_handler_.getNumVoice();
}

void Synthesizer::resetBuffer() {
  voice_handler_.resetBuffer();
}

void Synthesizer::noteOn(int note, int velocity) {
  voice_handler_.noteOn(note, velocity, 0);
}

void Synthesizer::noteOff(int note) {
  voice_handler_.noteOff(note, 0);
}

void Synthesizer::process() {
  resetBuffer();
  voice_handler_.process();
  *outputs_[0] = *(voice_handler_.getOutput(0));
  *outputs_[1] = *(voice_handler_.getOutput(1));
}

}