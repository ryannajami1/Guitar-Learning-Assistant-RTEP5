#ifndef BIQUAD_BANDSTOP
#define BIQUAD_BANDSTOP

#include <cmath>
#include <cstdint>

class BiquadBandstop
{
public:
    void config(double f_0, double f_s, double Q_factor);
    int16_t process(int16_t new_input);

private:
    double a0, a1, a2, b1, b2;
    double z1 = 0, z2 = 0;
};

#endif
