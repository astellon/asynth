#pragma once

#include <cstdint>

#include "common.h"

namespace astellon {

using std::uint32_t;

class Phase {
 public:
  Phase();

  Phase(const uint32_t&);
  Phase(const float&);
  Phase(const double&);

  ~Phase();

  double toDouble();
  float toFloat();

  uint32_t getRawValue() const;

  Phase& operator=(const Phase& p);
  Phase& operator+=(Phase& p);
  Phase& operator-=(Phase& p);
  Phase& operator*=(Phase& p);
  Phase& operator/=(Phase& p);

  const static uint32_t DOUBLE_PI = 0xffffffff;
  const static uint32_t PI = 0x8fffffff;
  const static uint32_t HALf_PI = 0x4fffffff;

 private:
  uint32_t phase_;
};

Phase& operator+(Phase& p1, Phase& p2);
Phase& operator-(Phase& p1, Phase& p2);
Phase& operator*(Phase& p1, Phase& p2);
Phase& operator/(Phase& p1, Phase& p2);
}  // namespace approx