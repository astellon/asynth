#pragma once

#include <vector>

#include "module.h"
#include "phase.h"

namespace astellon {
class Oscillator : public Module {

 public:
  enum OscInput {
    osc_num_inputs,
  };

  enum OscOutput {
    osc_output_L,
    osc_output_R,
    osc_num_outputs,
  };

  enum OscParams {
    osc_num_params,
  };

  Oscillator();
  Oscillator* clone() const;

  void noteOn(int note, int sample_ofset);
  void noteOff(int sample_ofset);
  void process(int start_sample, int end_sample);

 private:
  double freq_;
  Phase phase_;
  Phase phase_delta_;
  void writeBuffer(int start_sample, int end_sample);
};
}  // namespace synth