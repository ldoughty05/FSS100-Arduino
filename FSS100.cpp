
#include "Energia.h"
#include "FSS100.h"
#include <Wire.h>

// Find some way to periodically set control register.

FSS100::FSS100(int address)
{
    _addr = address;
    _ctrlReg = 0;
}

void FSS100::init()
{
    _ctrlReg |= 0x40; // Sampling Mode = Continuous
    _ctrlReg |= 0x10; // Sampling Rate = 16 Hz

    Wire.begin();

    Wire.beginTransmission(_addr);
    Wire.write(0x0A);
    Wire.write(_ctrlReg);
    Wire.endTransmission();
}

void FSS100::getSample(int *theta, int *phi)
{
    Wire.beginTransmission(_addr);
    Wire.write(0x00);
    Wire.endTransmission();

    *theta = 0
    *phi = 0;

    int i = 0;

    Wire.requestFrom(_addr, 4);
    while(Wire.available()) {
        char c = Wire.read();
        if (i == 0) {
            *theta = c;
        } else if (i == 1) {
            *theta |= (c << 8);
        } else if (i == 2) {
            *phi = c;
        } else if (i == 3) {
            *phi |= (c << 8);
        }

        i++;
    }
}

int16_t FSS100::getTheta() {
    Wire.beginTransmission(_addr);
    Wire.write(0x00);
    Wire.endTransmission();

    int16_t theta = 0;
    int i = 0;
    Wire.requestFrom(_addr, 2);
    while(Wire.available()) {
        char c = Wire.read();
        if (i == 0) {
            theta |= c;
        } else if (i == 1) {
            theta |= (c << 8);
        }
        i++;
    }

    return theta;
}

int16_t FSS100::getPhi() {
    Wire.beginTransmission(_addr);
    Wire.write(0x02);
    Wire.endTransmission();

    int16_t phi = 0;
    int i = 0;
    Wire.requestFrom(_addr, 2);
    while(Wire.available()) {
        char c = Wire.read();
        if (i == 0) {
            phi |= c;
        } else if (i == 1) {
            phi |= (c << 8);
        }
        i++;
    }

    return phi;
}

void FSS100::setI2CAddress(int newAddress)
{
    Wire.beginTransmission(_addr);
    Wire.write(0x0B);
    Wire.write(newAddress);
    Wire.endTransmission();

    _addr = newAddress;
}

void FSS100::updCtrl(uint8_t ctrl)
{
    _ctrlReg = ctrl;

    Wire.beginTransmission(_addr);
    Wire.write(0x0A);
    Wire.write(_ctrlReg);
    Wire.endTransmission();
}

void FSS100::setContinuousSampling(bool continuous)
{
    if (continuous) {
        _ctrlReg |= 0x40;
    } else {
        _ctrlReg &= 0xBF;
    }

    this->updCtrl(_ctrlReg);
}

void FSS100::setSamplingRate(int samplingRate)
{
    _ctrlReg &= 0xE7;

    if (samplingRate <= 4) {
        _ctrlReg |= 0x00; // 4 Hz
    } else if (samplingRate <= 8) {
        _ctrlReg |= 0x08; // 8 Hz
    } else {
        _ctrlReg |= 0x10; // 16 Hz
    }

    this->updCtrl(_ctrlReg);
}
