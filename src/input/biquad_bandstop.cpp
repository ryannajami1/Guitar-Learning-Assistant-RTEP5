#include "biquad_bandstop.hpp"

void BiquadBandstop::config(double f0, double fs, double Q)
{
    double omega = 2.0 * M_PI * f0 / fs;
    double alpha = sin(omega) / (2.0 * Q);
    double cos_omega = cos(omega);

    double a0 = 1.0 + alpha;
    a1 = -2.0 * cos_omega;
    a2 = 1.0 - alpha;
    b0 = 1.0;
    b1 = -2.0 * cos_omega;
    b2 = 1.0;
    
    // normalise by a0
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

int16_t BiquadBandstop::process(int16_t input)
{
    // Convert input to float
    double in = static_cast<double>(input);

    // filter processing
    double out = b0 * in + z1;
    z1 = b1 * in + z2 - a1 * out;
    z2 = b2 * in - a2 * out;

    // Clip result to int16_t range
    if (out > 32767.0) out = 32767.0;
    else if (out < -32768.0) out = -32768.0;

    return static_cast<int16_t>(out);
}