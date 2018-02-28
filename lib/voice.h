#pragma once

#include <list>

#include "common.h"
#include "module.h"
#include "oscillator.h"

namespace astellon {
/*==============================================================================
Voice class
- define one vice.
- has oscillator, envelope, modulators.
==============================================================================*/
class Voice : public Module {

 public:
  enum VoiceOutput {
    voice_output_L,
    voice_output_R,
    voice_num_outputs,
  };

  enum VoiceInput {
    voice_num_inputs,
  };

  enum VoiceParams {
    voice_num_params,
  };

  Voice();
  ~Voice();

  int getNote() const { return note_; }

  bool isActive() const { return activity_; }
  bool isReleased() { return activity_; };

  void setPlayer(double sample_rate, int buffer_size);

  void noteOn(int note, int velocity, int sample_ofset);
  void noteOff(int sample_ofset);

  void inactivate();

  void process();

 private:
  Oscillator* osc_;
  int note_;
  int velocity_;
  int sample_ofset_;
  bool activity_;
};
}  // namespace synth
