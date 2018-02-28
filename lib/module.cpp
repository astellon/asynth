#include "module.h"

#include <algorithm>
#include <malloc.h>

#include "simd_alias.h"

namespace astellon {
/*=============================================================================
Output class
=============================================================================*/

Output::Output(int buffer_size) : buffer_size_(buffer_size) {
  buffer_
  = reinterpret_cast<AudioType*>(_mm_malloc(sizeof(AudioType)*buffer_size_, 16));
}

Output::Output(const Output& out) {
  buffer_size_ = out.size();
  buffer_
  = reinterpret_cast<AudioType*>(_mm_malloc(sizeof(AudioType)*buffer_size_, 16));

  auto copied_buffer = out.getReadPointer();

  for (int i = 0; i < buffer_size_; i += LOOP_STEP) {
    _simd_store(&buffer_[i], _simd_load(&copied_buffer[i]));
  }
}

Output::~Output() {
  clearBuffer();
}

void Output::resize(int buffer_size) {
  clearBuffer();
  buffer_size_ = buffer_size;
  buffer_
  = reinterpret_cast<AudioType*>(_mm_malloc(sizeof(AudioType)*buffer_size_, 16));
}

int Output::size() const { return buffer_size_; }

AudioType* Output::getWritePointer() { return buffer_; }

const AudioType* Output::getReadPointer() const {
  return buffer_;
}

void Output::resetBuffer() {
  for (int i = 0; i < buffer_size_; i += LOOP_STEP) {
    _simd_storezero(&buffer_[i]);
  }
}

void Output::clearBuffer() {
  _mm_free(buffer_);
  buffer_ = nullptr;
  buffer_size_ = 0;
}

const AudioType& Output::output(int index) const { return buffer_[index]; }

AudioType& Output::operator[](int index) { return buffer_[index]; }

AudioType& Output::at(int index) { return buffer_[index]; }

Output& Output::operator=(const Output& out) {
  auto copying_buffer_size = std::min(buffer_size_, out.size());
  auto copied_buffer = out.getReadPointer();
  for (int i = 0; i < copying_buffer_size; i += LOOP_STEP) {
    _simd_store(&buffer_[i], _simd_load(&copied_buffer[i]));
  }
  return *this;
}

Output& Output::operator+=(const Output& out) {
  auto copying_buffer_size = std::min(buffer_size_, out.size());
  auto copied_buffer = out.getReadPointer();
  for (int i = 0; i < copying_buffer_size; i += LOOP_STEP) {
    _simd_store(&buffer_[i], _simd_add(&copied_buffer[i], &buffer_[i]));
  }
  return *this;
}

Output& Output::operator*=(const Output& out) {
  auto copying_buffer_size = std::min(buffer_size_, out.size());
  auto copied_buffer = out.getReadPointer();
  for (int i = 0; i < copying_buffer_size; i += LOOP_STEP) {
    _simd_store(&buffer_[i], _simd_mul(&copied_buffer[i], &buffer_[i]));
  }
  return *this;
}

// non-menber functions
Output& operator+(const Output& lhs, const Output& rhs) {
  return Output(lhs) += rhs;
}

Output& operator*(const Output& lhs, const Output& rhs) {
  return Output(lhs) *= rhs;
}

Output& operator*(const float& lhs, const Output& rhs) {
  Output out(rhs);
  auto copying_buffer_size = rhs.size();

  auto buffer = out.getWritePointer();
  auto copied_buffer = out.getReadPointer();

  for (auto i = 0; i < copying_buffer_size; i += LOOP_STEP) {
    _simd_store(&buffer[i], _simd_mul(lhs, &copied_buffer[i]));
  }
  return out;
}

/*=============================================================================
Input class
=============================================================================*/

Input::Input() : source_(nullptr) {}

void Input::connect(Output* output) { source_ = output; }

void Input::disconnect() { source_ = nullptr; }

const Output* Input::getSource() const { return source_; }

bool Input::hasSource() const { return source_ == nullptr; }

const AudioType Input::input(int index) const { return source_->output(index); }

const AudioType Input::operator[](int index) const {
  return source_->operator[](index);
}

const AudioType Input::at(int index) const { return source_->at(index); }

/*=============================================================================
Parameter class
=============================================================================*/

Parameter::Parameter(std::string id, float min_value,
                     float max_value, float default_value)
: id_(id), min_value_(min_value), max_value_(max_value),
  default_value_(default_value), value_(default_value) {}

std::string Parameter::getID() const { return id_; }

float Parameter::get() const { return value_; }

void Parameter::set(float value) {
  //value_ = fclamp(value, min_value_, max_value_);
}

float Parameter::getMaxValue() { return max_value_; }

float Parameter::getMinValue() { return min_value_; }

float Parameter::getDefaultValue() { return default_value_; }

void Parameter::reset() { value_ = default_value_; }

/*==============================================================================
Mudule class
==============================================================================*/

Module::Module(int num_inputs, int num_outputs, int num_params)
: sample_rate_(DEFAULT_SAMPLE_RATE), buffer_size_(DEFAULT_BUFFER_SIZE) {
  inputs_.clear(); outputs_.clear(); params_.clear();

  inputs_.reserve(num_inputs);
  outputs_.reserve(num_outputs);

  for (int i = 0; i < num_inputs; i++) {
    Input* input = new Input();
    inputs_.push_back(input);
  }

  for (int i = 0; i < num_outputs; i++) {
    Output* output = new Output(buffer_size_);
    outputs_.push_back(output);
  }

  inputs_.shrink_to_fit();
  outputs_.shrink_to_fit();

  params_.resize(num_params);
}

Module::~Module() {
  for (Output* output : outputs_) {
    delete output;
  }

  for (Input* input : inputs_) {
    delete input;
  }
}

Module* Module::clone() { return nullptr; }

void Module::setPlayer(double sample_rate, int buffer_size) {
  sample_rate_ = sample_rate;
  buffer_size_ = buffer_size;
  for (Output* output : outputs_) {
    output->resize(buffer_size_);
  }
}

void Module::resetBuffer() {
  for (Output* output : outputs_) {
    output->resetBuffer();
  }
}

void Module::plug(Output* source, int target_index) {
  inputs_[target_index]->connect(source);
}

void Module::plug(Input* target, int source_index) {
  target->connect(getOutput(source_index));
}

void Module::unplug(int index) { inputs_[index]->disconnect(); }

int Module::numOutputs() const { return outputs_.size(); }

int Module::numInputs() const { return inputs_.size(); }

int Module::numParams() const { return params_.size(); }

AudioType Module::output(int channel, int index) const {
  return outputs_[channel]->at(index);
}

AudioType Module::input(int channel, int index) const {
  return inputs_[channel]->at(index);
}

Output* Module::getOutput(int index) { return outputs_[index]; }

void Module::createParameter(int index, std::string id,
  float min_value, float max_value, float default_value) {
  params_[index]
  = std::make_shared<Parameter>(id, min_value, max_value, default_value);
}

void Module::registerParameter(std::shared_ptr<Parameter> param, int index) {
  params_[index] = param;
}

Parameter* Module::getParameter(int index) {
  return params_[index].get();
}

float Module::get(int index) const { return params_[index]->get(); }

void Module::set(float value, int index) { params_[index]->set(value); }

//=============================================================================
}  // namespace synth
