#include "oscillator.h"

#include <cmath>

#include "common.h"
#include "sin.h"

#define LEVEL 0.1f  // audio level adjustment

namespace astellon {

Oscillator::Oscillator()
: Module(osc_num_inputs, osc_num_outputs, osc_num_params),
  phase_(0.0), phase_delta_(0.0) {

  resetBuffer();
}

Oscillator* Oscillator::clone() const {
  Oscillator* osc = new Oscillator();
  for (int i=0; i < osc_num_params; i++) {
    osc->registerParameter(params_[i], i);
  }
  return osc;
}

void Oscillator::noteOn(int note, int sample_ofset) {
  freq_ = noteToHeltz(note);
  phase_delta_ = freq_ / sample_rate_ * 2.0 * PI;
}

void Oscillator::noteOff(int sample_ofset) {}

void Oscillator::process(int start_sample, int end_sample) {
  resetBuffer();
  writeBuffer(start_sample, end_sample);
}

void Oscillator::writeBuffer(int start_sample, int end_sample) {
  for (int index = start_sample; index < end_sample; index++) {
    outputs_[0]->at(index) = LEVEL * table::sin(phase_);
    phase_ += phase_delta_;
  }
  *outputs_[1] = *outputs_[0];
}
}  // namespace synth