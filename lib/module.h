#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common.h"

namespace astellon {

/*=============================================================================
Output class
=============================================================================*/

class Output {
 public:
  explicit Output(int buffer_size = DEFAULT_BUFFER_SIZE);
  Output(const Output& out);
  virtual ~Output();

  void resize(int buffer_size);
  int size() const;

  AudioType* getWritePointer();
  const AudioType* getReadPointer() const;

  void resetBuffer();
  void clearBuffer();

  const AudioType& output(int index) const;
  AudioType& operator[](int index);
  AudioType& at(int index);

  Output& operator=(const Output& out);
  Output& operator+=(const Output& out);
  Output& operator*=(const Output& out);

 protected:
  int buffer_size_;
  AudioType* buffer_;
};

Output& operator+(const Output& lhs, const Output& rhs);
Output& operator*(const Output& lhs, const Output& rhs);
Output& operator*(const AudioType& lhs, const Output& rhs);

/*=============================================================================
Input class
=============================================================================*/

class Input {
 public:
  Input();

  void connect(Output* output);
  void disconnect();

  Output* getSource() const;
  bool hasSource() const;

  const AudioType input(int index) const;
  const AudioType operator[](int index) const;
  const AudioType at(int index) const;

 private:
  Output* source_;
};

/*=============================================================================
Parameter class
=============================================================================*/

class Parameter {
 public:
  Parameter
    (std::string id, float min_value, float max_value, float default_value);

  std::string getID() const;

  float get() const;
  void set(float value);

  float getMaxValue();
  float getMinValue();
  float getDefaultValue();

  void reset();

 private:
  std::string id_;
  float min_value_;
  float max_value_;
  float default_value_;
  float value_;
};

/*==============================================================================
Mudule class
==============================================================================*/

class Module {
 public:
  Module(int num_inputs, int num_outputs, int num_params);
  ~Module();

  virtual Module* clone();

  virtual void setPlayer(double sample_rate, int buffer_size);

  void resetBuffer();

  void plug(Output* source, int target_index);
  void plug(Input* target, int source_index);
  void unplug(int index);

  int numOutputs() const;
  int numInputs() const;
  int numParams() const;

  AudioType output(int channel, int index = 0) const;
  AudioType input(int channel, int index = 0) const;
  Output* getOutput(const int index);

  void createParameter(int index, std::string id,
    float min_value, float max_value, float default_value);
  void registerParameter(std::shared_ptr<Parameter> parm, int index);
  Parameter* getParameter(int index);

  float get(int index) const;
  void set(float value, int index);

 protected:
  double sample_rate_;
  int buffer_size_;
  std::vector<Input*> inputs_;
  std::vector<Output*> outputs_;
  std::vector<std::shared_ptr<Parameter>> params_;
};
// ==============================================================================
}  // namespace synth
