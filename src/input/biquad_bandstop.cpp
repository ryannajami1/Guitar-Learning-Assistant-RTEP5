#include "biquad_bandstop.hpp"
#include <cmath>
#include <cstdint>

void BiquadBandstop::config(double f_0, double f_s, double Q_factor)
{
  double const omega = 2.0 * M_PI * f_0 / f_s;
  double const alpha = sin(omega) / (2.0 * Q_factor);
  double const cos_omega = cos(omega);

  a0 = 1.0;
  a1 = -2.0 * cos_omega;
  a2 = 1.0;
  b1 = -2.0 * cos_omega;
  b2 = 1.0 - alpha;

  double const a0_norm = 1.0 + alpha;

  // Normalize
  a1 /= a0_norm;
  a2 /= a0_norm;
  b1 /= a0_norm;
  b2 /= a0_norm;
}
auto BiquadBandstop::process(int16_t new_input) -> int16_t {
    // Convert input to float
    auto input = static_cast<double>(new_input);
    double out = (a0 * input) + z1;
    z1 = a1 * input + z2 - b1 * out;
    z2 = a2 * input - b2 * out;


  // Clip result to int16_t range
  if (out > 32767.0) {
    out = 32767.0;
  } else if (out < -32768.0) {
    out = -32768.0;
  }

  return static_cast<int16_t>(out);
}