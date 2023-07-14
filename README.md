# FSS100 Sun Sensor library 

## Table of Contents

FSS100(int address);
void init();
void getSample(int *theta, int *phi);
int16 getTheta();
int16 getPhi();
void setI2CAddress(int address);
void updCtrl(uint8_t ctrl);
void setContinuousSampling(bool continuous);
void setSamplingRate(int rate);

## Description

## Functions

### Constructor

Takes the I2C address of the FSS100 Sun Vector Sensor.

### Init

Begins the Wire library. It also sets the FSS100 Sun Vector Sensor Control Register 

### getSample

Takes pointers to theta and phi variables. The pointers are then given the current values of the theta and phi registers.

### getTheta

Returns the currently stored theta value.

### getPhi

Returns the currently stored phi value. 

### setI2CAddress

Takes an unsigned 8 bit integer, which will be set as the new I2C address of the FSS100 sun sensor. 

### updCtrl

Takes an unsigned 8 bit integer which will be assigned to control register. The control register is described below.

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|-|-|-|-|-|-|-|-|
| SAMP | MODE | CMT | SR | SR | LOCK | -- | -- |


**SAMP: Sample Request Bit**
1 = Requests a new sample which will overwrite the current Theta and Phi data registers. This bit will be set to 0 after the registers are updated.
0 = The data has been retrieved. The data registers will not be updated unless the MODE bit is set to continuous mode.

**MODE: Operating Mode**
1 = Continuous Mode, the data registers will be continually updated at the rate set in the SR bits.
0 = One-Shot Mode, the data registers will only be updated if the SAMP bit is set to 1.

**CMT: Commit Setting**
1 = Store the current Control register setting and I2C address in non-volatile memory. Read as 1 while the commit has not been handled.
0 = Settings remain in random access memory.

**SR: Sampling Rate**
0b11 = 32 Hz (Not available on the FSS100 (or at least not the model we are using))
0b10 = 16 Hz
0b01 = 8 Hz
0b00 = 4 Hz

**LOCK: Extend Register Lock**
1 = Unlocks the extend registers. Extend register contain factory calibration data, please consult Tensor Tech before modifying them.
0 = Extend registers locked.

### setContinuousSampling

Takes a boolean which, if true, will put the FSS100 in continuous mode. If the input is false, then the sun sensor will be put into one-shot mode, where the data registers will not be updated.

### setSamplingRate

Takes an integer, which will be assigned as the sampling rate. The input value will be rounded up the closest available sampling rate of 4, 8, or 16Hz. The sampling rate will then be updated on the FSS100 sun sensor.

## Setup for Energia with MSP430 FR5969
