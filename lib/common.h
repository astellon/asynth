#pragma once

#include <cmath>

namespace astellon {
/*==============================================================================
constant variables difiniton
==============================================================================*/
using AudioType = double;  // alias of audio buffer type
const int LOOP_STEP = 16/sizeof(AudioType);

const double PI = 3.14159265358979323846;
const int DEFAULT_BUFFER_SIZE = 256;
const int DEFAULT_SAMPLE_RATE = 44100.0;
const int DEFAULT_NUM_VOICES = 16;
const int DEFAULT_NUM_HARMONIES = 512;
const double A0 = 440.0;
const double ATTACK_TIME = 3.0;
const double DECAY_TIME = 10.0;
const double RELEASE_TIME = 8.0;
/*==============================================================================
utility functions difiniton
==============================================================================*/
inline double noteToHeltz(int note) {
  return A0*pow(2.0, (note - 69.0)/12);
}

inline double sampleToSecond(int num_samples, double sample_rate) {
  return num_samples/sample_rate;
}

inline int secondToSample(double second, double sample_rate) {
  return static_cast<int>(second*sample_rate);
}
// ==============================================================================
}  // namespace synth