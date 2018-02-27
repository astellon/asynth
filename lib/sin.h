#pragma once

#include <cmath>

#include "phase.h"

namespace astellon {

namespace pade {
double sinZeroToHalfPi(double angle) {
  // return -7.0/3.0*angle + 200.0/3.0*angle/(20.0 + angle*angle);
  return angle*(11.0/60.0*angle*angle - 137.0/10.0
                 + 37044.0/60.0/(angle*angle+42.0));
}

double sin(double angle) {
  if (angle < 0) {
    return -1.0 * sin(-1.0 * angle);
  } else if (angle < PI/2) {
    return sinZeroToHalfPi(angle);
  } else if (angle < PI) {
    return sinZeroToHalfPi(PI - angle);
  } else if (angle < 3*PI/2) {
    return -1.0 * sinZeroToHalfPi(angle - PI);
  } else if (angle < 2*PI) {
    return -1.0 * sinZeroToHalfPi(2*PI - angle);
  } else {
    angle = std::fmod(angle, PI * 2);
    return sin(angle);
  }
}

double sin(Phase p) {
  uint32_t p32 = p.getRawValue();
  uint64_t p64 = p32;  // extend
  p64 &= 0x000000003fffffff;  // mask lower 30bits
  p64 = 0x3ff0000000000000 | (p64 << 22);  // remove top 2bits and make double
  if (p32 & 0x40000000) p64 ^= 0x000fffffffffffff;
  double dp = (*(reinterpret_cast<double*>(&p64)) - 1.0) * PI / 2.0;
  return (p32 & 0x80000000) ? -sinZeroToHalfPi(dp) : sinZeroToHalfPi(dp);
}
}  // namespace pade

namespace table {
// table maker
namespace {
class SinTable {
 public:
  SinTable() : table_(new double[sample_num_+1]) {
    for (int i = 0; i < sample_num_; i++) {
      table_[i] = std::sin(i * PI / 2 / sample_num_);
    }
    table_[sample_num_] = 1.0;
  }

  ~SinTable() {
    delete[] table_;
    table_ = nullptr;
  }

  double sin(double angle) const {
    if (angle < 0) {
      return -1.0 * sin(-1.0 * angle);
    } else if (angle < PI/2) {
      return sinZeroToHalfPi(angle);
    } else if (angle < PI) {
      return sinZeroToHalfPi(PI - angle);
    } else if (angle < 3*PI/2){
      return -1.0 * sinZeroToHalfPi(angle - PI);
    } else if (angle < 2*PI) {
      return -1.0 * sinZeroToHalfPi(2*PI - angle);
    } else {
      angle = std::fmod(angle, PI * 2);
      return sin(angle);
    }
  }

  double sin(Phase angle) const {
    uint32_t p32 = angle.getRawValue();
    uint32_t p32_masked = p32 & 0x3fffffff;  // mask top 2bit
    if (p32 & 0x40000000) p32_masked ^= 0x3fffffff;  // bit flip
    std::uint8_t p8 = p32_masked >> 22;  // use 8bit;

    uint64_t p64 = p32_masked;
    p64 = 0x3ff0000000000000 | (p64 << 30);
    double inter = (*((double*)&p64) - 1.0);

    return (p32 & 0x80000000)
    ? -((table_[p8+1] - table_[p8])*inter + table_[p8])
    : (table_[p8+1] - table_[p8])*inter + table_[p8];
  }

 private:
  double sinZeroToHalfPi(double angle) const {
    double inter = angle * sample_num_ * 2 / PI;
    int index = static_cast<int>(inter);
    return (table_[index + 1] - table_[index])*(inter - index) + table_[index];
  }
  const int sample_num_ = 256;
  double* table_;
};

SinTable table;
}  // unmaned namespace

double sin(double angle) {
  return table.sin(angle);
}

double sin(Phase angle) {
  return table.sin(angle);
}
}  // namespace table

}  // namespace approx