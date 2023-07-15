#ifndef FSS100_h
#define FSS100_h

#include "Energia.h"

class FSS100
{
public:
    FSS100(int address);
    void init();
    void getSample(int *theta, int *phi);
    int16_t getTheta();
    int16_t getPhi();
    void setI2CAddress(int address);
    void updCtrl(uint8_t ctrl);
    void setContinuousSampling(bool continuous);
    void setSamplingRate(int rate);

private:
    int _addr;
    uint8_t _ctrlReg;
};

#endif // FSS100
