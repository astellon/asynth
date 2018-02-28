#pragma once

#include "module.h"
#include "voice.h"
#include "voice_handler.h"

namespace astellon {

class Synthesizer : public Module {
 public:
  Synthesizer();

  void setPlayer(double sample_rate, int buffer_size);

  void setNumVoice(int num);
  int getNumVoice() const;

  void resetBuffer();

  void noteOn(int note, int velocity);
  void noteOff(int note);

  // void setParameter();
  // float getParameter();

  void process();

 private:
  VoiceHandler<Voice> voice_handler_;
};

}  // namespace synth