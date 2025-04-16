#ifndef BIQUAD_BANDSTOP
#define BIQUAD_BANDSTOP

#include <cmath>
#include <cstdint>

class BiquadBandstop
{
public:
    void config(double f0, double fs, double Q);
    int16_t process(int16_t in);

private:
    double b0, b1, b2, a1, a2;
    double z1 = 0, z2 = 0;
};

#endif
