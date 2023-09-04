#ifndef FSS100_h
#define FSS100_h

#include "Energia.h"

#define FSS100_SR_32HZ (1 << 4) | (1 << 3)
#define FSS100_SR_16HZ (1 << 4) | (0 << 3)
#define FSS100_SR_8HZ (0 << 4) | (1 << 3)
#define FSS100_SR_4HZ (0 << 4) | (0 << 3)
#define FSS100_CONL_CONTINUOUS (1 << 6)
#define FSS100_CONL_ONESHOT ~FSS100_CONL_CONTINUOUS
#define FSS100_CONL_SAMPLEREQUEST (1 << 7)
#define FSS100_CONL_SAMPLEDONE ~FSS100_CONL_SAMPLEREQUEST

class FSS100
{
public:
    FSS100(int address);
    void init();
    void getSample(int16_t *theta, int16_t *phi);
    int16_t getTheta();
    int16_t getPhi();
    void setI2CAddress(int address);
    void update_conl();
    void setContinuousSampling(bool continuous);
    void setSamplingRate(int rate);
	bool sample_wait(void);
	void setSampleBit(bool sample);
	void default_config(void);

private:
    int _addr;
    uint8_t _conl_reg;
};

#endif // FSS100
