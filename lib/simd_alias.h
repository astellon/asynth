#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <x86intrin.h>
#endif

namespace astellon {
static __m128 _simd_load(const float* value) {
  return _mm_load_ps(value);
}

static __m128d _simd_load(const double* value) {
  return _mm_load_pd(value);
}

static void _simd_store(float* value, __m128 new_value) {
  _mm_store_ps(value, new_value);
}

static void _simd_store(double* value, __m128d new_value) {
  _mm_store_pd(value, new_value);
}

static void _simd_storezero(float* value) {
  _simd_store(value, _mm_setzero_ps());
}

static void _simd_storezero(double* value) {
  _simd_store(value, _mm_setzero_pd());
}

static __m128 _simd_add(const float* lhs, const float* rhs) {
  return _mm_add_ps(_simd_load(lhs), _simd_load(rhs));
}

static __m128d _simd_add(const double* lhs, const double* rhs) {
  return _mm_add_pd(_simd_load(lhs), _simd_load(rhs));
}

static __m128 _simd_mul(const float* lhs, const float* rhs) {
  return _mm_mul_ps(_simd_load(lhs), _simd_load(rhs));
}

static __m128d _simd_mul(const double* lhs, const double* rhs) {
  return _mm_mul_pd(_simd_load(lhs), _simd_load(rhs));
}

static __m128 _simd_mul(const float lhs, const float* rhs) {
  return _mm_mul_ps(_mm_set1_ps(lhs), _simd_load(rhs));
}

static __m128d _simd_mul(const double lhs, const  double* rhs) {
  return _mm_mul_pd(_mm_set1_pd(lhs), _simd_load(rhs));
}

}  // namespace shirabe