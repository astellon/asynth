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

/*==============================================================================
VoiceHandler class.
==============================================================================*/
class VoiceHandler : public Module {

 public:
  enum VoiceHandlerOutput {
    voice_handler_output_L,
    voice_handler_output_R,
    voice_handler_num_outputs,
  };

  enum VoiceHandlerInput {
    voice_handler_num_inputs,
  };

  enum VoiceHandlerParams {
    voice_handler_num_params,
  };

  explicit VoiceHandler(int num_voice = DEFAULT_NUM_HARMONIES);
  ~VoiceHandler();

  void setPlayer(double sample_rate, int buffer_size) override;

  int getNumVoice() const { return num_voice_; }
  void resizeVoice(int num_voice);
  void clearVoice();

  void noteOn(int note, int velocity, int sample_ofset);
  void noteOff(int note, int sample_ofset);
  void allNoteOff(int sample_ofset);

  void process();

 private:
  void inactivateVoice();
  int num_voice_;
  std::list<Voice*> all_voices_;
  std::list<Voice*> active_voices_;
  std::list<Voice*> inactive_voices_;
};
// ==============================================================================
}  // namespace synth
