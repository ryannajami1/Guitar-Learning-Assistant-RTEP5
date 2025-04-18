#ifndef BIQUAD_BANDSTOP
#define BIQUAD_BANDSTOP

#include <cmath>
#include <cstdint>

class BiquadBandstop
{
public:
    void config(double f_0, double f_s, double Q_factor);
    auto process(int16_t new_input) -> int16_t;

  private:
    double a0, a1, a2, b1, b2;
    double z1 = 0, z2 = 0;
};

#endif
