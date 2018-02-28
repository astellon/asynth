#pragma once

#include "common.h"
#include "voice.h"

namespace astellon {
/*==============================================================================
VoiceHandler class.
==============================================================================*/
template<typename T>
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

  explicit VoiceHandler(int num_voice = DEFAULT_NUM_VOICES)
  : Module(voice_handler_num_inputs, voice_handler_num_outputs,
         voice_handler_num_params), num_voice_(num_voice) {
    resizeVoice(num_voice);
  }

  ~VoiceHandler() {
    clearVoice();
  }

  void setPlayer(double sample_rate, int buffer_size) override {
    sample_rate_ = sample_rate;
    buffer_size_ = buffer_size;

    for (auto voice : all_voices_) {
      voice->setPlayer(sample_rate, buffer_size);
    }

    Module::setPlayer(sample_rate, buffer_size);
  }

  int getNumVoice() const { return num_voice_; }

  void resizeVoice(int num_voice) {
    num_voice_ = num_voice;
    clearVoice();
    for (int i = 0; i < num_voice; i++) {
      auto voice = new T();
      all_voices_.push_back(voice);
      inactive_voices_.push_back(voice);
    }
  }

  void clearVoice() {
    for (auto voice : all_voices_) {
      delete voice;
    }

    all_voices_.clear();
    active_voices_.clear();
    inactive_voices_.clear();
  }

  void noteOn(int note, int velocity, int sample_ofset) {
    if (!inactive_voices_.empty()) {
      auto voice = inactive_voices_.back();
      voice->noteOn(note, velocity, sample_ofset);
      active_voices_.push_back(voice);
      inactive_voices_.pop_back();
    } else {
      for (auto voice : active_voices_) {
        if (voice->isReleased()) {
          voice->noteOn(note, velocity, sample_ofset);
          return;
        }
      }
    }
  }

  void noteOff(int note, int sample_ofset) {
    for (auto  itr = active_voices_.begin(); itr != active_voices_.end(); itr++) {
      auto voice = *itr;
      if (voice->getNote() == note) voice->noteOff(sample_ofset);
    }
  }

  void allNoteOff(int sample_ofset) {
    for (auto voice : active_voices_) {
      voice->noteOff(sample_ofset);
      inactive_voices_.push_back(voice);
    }
    active_voices_.clear();
  }

  void process() {
    resetBuffer();
    for (auto voice : active_voices_) {
      voice->process();
      for (int channel = 0; channel < 2; channel++) {
        *outputs_[channel] += *(voice->getOutput(channel));
      }
    }
    inactivateVoice();
  }

 private:
  void inactivateVoice() {
    auto itr = active_voices_.begin();
    while (itr != active_voices_.end()) {
      auto voice = *itr;
      if (!voice->isActive()) {
        inactive_voices_.push_back(voice);
        itr = active_voices_.erase(itr);
      } else {
        itr++;
      }
    }
  }

  int num_voice_;
  std::list<T*> all_voices_;
  std::list<T*> active_voices_;
  std::list<T*> inactive_voices_;
};
}  // namespace astellon