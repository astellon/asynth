#include "phase.h"

#include <cmath>

namespace astellon {
Phase::Phase() :phase_(0) {}

Phase::Phase(const std::uint32_t& p) {
  phase_ = p;
}

Phase::Phase(const double& p) {
  double dpi = 2*3.14159265358979323846;
  phase_ = std::uint32_t(p / dpi * UINT32_MAX);
}

Phase::Phase(const float& p) {
  double dpi = 2*3.14159265358979323846;
  phase_ = std::uint32_t(p / dpi * UINT32_MAX);
}

Phase::~Phase() {}

double Phase::toDouble() {
  return double(phase_)/UINT32_MAX * 2 * 3.14159265358979323846;
}

float Phase::toFloat() {
  return (float)toDouble();
}

std::uint32_t Phase::getRawValue() const {
  return phase_;
}

Phase& Phase::operator=(const Phase& p) {
  phase_ = p.getRawValue();
  return *this;
}

Phase& Phase::operator+=(Phase& p) {
  phase_ += p.getRawValue();
  return *this;
}

Phase& Phase::operator-=(Phase& p) {
  phase_ -= p.getRawValue();
  return *this;
}

Phase& Phase::operator*=(Phase& p){
  phase_ *= p.getRawValue();
  return *this;
}

Phase& Phase::operator/=(Phase& p) {
  phase_ /= p.getRawValue();
  return *this;
}

Phase& operator+(Phase& p1, Phase& p2) {
  return Phase(p1) += p2;
}

Phase& operator-(Phase& p1, Phase& p2) {
  return Phase(p1) -= p2;
}

Phase& operator*(Phase& p1, Phase& p2) {
  return Phase(p1) *= p2;
}

Phase& operator/(Phase& p1, Phase& p2) {
  return Phase(p1) /= p2;
}
}  // namespace synth