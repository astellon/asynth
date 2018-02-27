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

/*==============================================================================
VoiceHandler class
==============================================================================*/
VoiceHandler::VoiceHandler(int num_voice)
: Module(voice_handler_num_inputs, voice_handler_num_outputs,
         voice_handler_num_params), num_voice_(num_voice) {
  resizeVoice(num_voice);
}

VoiceHandler::~VoiceHandler() {
  clearVoice();
}

void VoiceHandler::setPlayer(double sample_rate, int buffer_size) {
  sample_rate_ = sample_rate;
  buffer_size_ = buffer_size;
  for (Output* output : outputs_) {
    output->resize(buffer_size_);
  }
  for (Voice* voice : all_voices_) {
    voice->setPlayer(sample_rate, buffer_size);
  }
}

void VoiceHandler::resizeVoice(int num_voice) {
  num_voice_ = num_voice;
  clearVoice();
  for (int i = 0; i < num_voice; i++) {
    Voice* voice = new Voice();
    all_voices_.push_back(voice);
    inactive_voices_.push_back(voice);
  }
}

void VoiceHandler::clearVoice() {
  for (Voice* voice : all_voices_) {
    delete voice;
  }
  all_voices_.clear();
  active_voices_.clear();
  inactive_voices_.clear();
}

void VoiceHandler::noteOn(int note, int velocity, int sample_ofset) {
  if (!inactive_voices_.empty()) {
    Voice* voice = inactive_voices_.back();
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

void VoiceHandler::noteOff(int note, int sample_ofset) {
  for (auto  itr = active_voices_.begin(); itr != active_voices_.end(); itr++) {
    Voice* voice = *itr;
    if (voice->getNote() == note) voice->noteOff(sample_ofset);
  }
}

void VoiceHandler::allNoteOff(int sample_ofset) {
  for (auto voice : active_voices_) {
    voice->noteOff(sample_ofset);
    inactive_voices_.push_back(voice);
  }
  active_voices_.clear();
}

void VoiceHandler::process() {
  resetBuffer();
  for (Voice* voice : active_voices_) {
    voice->process();
    for (int channel = 0; channel < 2; channel++) {
      *outputs_[channel] += *(voice->getOutput(channel));
    }
  }
  inactivateVoice();
}

void VoiceHandler::inactivateVoice() {
  std::list<Voice*>::iterator itr = active_voices_.begin();
  while (itr != active_voices_.end()) {
    Voice* voice = *itr;
    if (!voice->isActive()) {
      inactive_voices_.push_back(voice);
      itr = active_voices_.erase(itr);
    } else {
      itr++;
    }
  }
}
// ============================================================================
}  // namespace shirabe
