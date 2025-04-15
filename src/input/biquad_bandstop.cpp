#include "biquad_bandstop.hpp"

void BiquadBandstop::config(double f0, double fs, double Q)
{
    double omega = 2.0 * M_PI * f0 / fs;
    double alpha = sin(omega) / (2.0 * Q);
    double cos_omega = cos(omega);

    a0 = 1.0;
    a1 = -2.0 * cos_omega;
    a2 = 1.0;
    b1 = -2.0 * cos_omega;
    b2 = 1.0 - alpha;

    double a0_norm = 1.0 + alpha;

    // Normalize
    a1 /= a0_norm;
    a2 /= a0_norm;
    b1 /= a0_norm;
    b2 /= a0_norm;
}

int16_t BiquadBandstop::process(int16_t input)
{
    // Convert input to float
    double in = static_cast<double>(input);
    double out = a0 * in + z1;
    z1 = a1 * in + z2 - b1 * out;
    z2 = a2 * in - b2 * out;

    // Clip result to int16_t range
    if (out > 32767.0) out = 32767.0;
    else if (out < -32768.0) out = -32768.0;

    return static_cast<int16_t>(out);
}