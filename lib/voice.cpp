#include "voice.h"

namespace astellon {
/*==============================================================================
Voice class
==============================================================================*/
Voice::Voice()
: Module(voice_num_inputs, voice_num_outputs, voice_num_params) {
  note_ = 0;
  velocity_ = 0;
  sample_ofset_ = 0;
  activity_ = false;

  osc_ = new Oscillator();
}

Voice::~Voice() {
  delete osc_;
  osc_ = nullptr;
}

void Voice::setPlayer(double sample_rate, int buffer_size) {
  sample_rate_ = sample_rate;
  buffer_size_ = buffer_size;
  for (Output* output : outputs_) {
    output->resize(buffer_size_);
  }
  osc_->setPlayer(sample_rate, buffer_size);
}

void Voice::noteOn(int note, int velocity, int sample_ofset) {
  note_ = note;
  velocity_ = velocity;
  sample_ofset_ = sample_ofset;
  activity_ = true;
  osc_->noteOn(note, sample_ofset);
}

void Voice::noteOff(int sample_ofset) {
  activity_ = false;
  sample_ofset_ = sample_ofset;
  osc_->noteOff(sample_ofset);
}

void Voice::inactivate() {
  note_ = 0;
  velocity_ = 0;
  sample_ofset_ = 0;
  activity_ = false;
}

void Voice::process() {
  if (!activity_) return;
  resetBuffer();
  osc_->process(0, buffer_size_);
  for (int channel = 0; channel < VoiceOutput::voice_num_outputs; channel++) {
    *outputs_[channel] = *(osc_->getOutput(channel));
  }
}
}  // namespace shirabe
