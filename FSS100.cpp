#include "Energia.h"
#include "FSS100.h"
#include <Wire.h>

// Find some way to periodically set control register.

FSS100::FSS100(int address)
{
	_addr = address;
	_conl_reg = 0;
}

void FSS100::init()
{
	//Wire.begin();
	default_config();
}

void FSS100::default_config(void)
{
	setContinuousSampling(false);
	setSamplingRate(FSS100_SR_16HZ);
	setSampleBit(true);
	update_conl();
}

void FSS100::continuous_config(void)
{
	setContinuousSampling(true);
	setSamplingRate(FSS100_SR_16HZ);
	update_conl();
}

bool FSS100::sample_wait()
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0A);
	Wire.endTransmission();
	Wire.requestFrom(_addr, 1);
	if (Wire.available())
	{
		char c = Wire.read();
		if (!(c & (1 << 7)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void FSS100::getSample(int16_t *theta, int16_t *phi)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x00);
	Wire.endTransmission();

	uint16_t theta_out = 0;
	uint16_t phi_out = 0;

	int i = 0;

	Wire.requestFrom(_addr, 4);
	while(Wire.available())
	{
		char c = Wire.read();
		if (i == 0)
		{
			theta_out = c;
		}
		else if (i == 1)
		{
			theta_out |= (c << 8);
		}
		else if (i == 2)
		{
			phi_out = c;
		}
		else if (i == 3)
		{
			phi_out |= (c << 8);
		}

		i++;
	}
	*theta = theta_out;
	*phi = phi_out;
}

int16_t FSS100::getTheta() {
	Wire.beginTransmission(_addr);
	Wire.write(0x00);
	Wire.endTransmission();

	int16_t theta = 0;
	int i = 0;
	Wire.requestFrom(_addr, 2);
	while(Wire.available())
	{
		char c = Wire.read();
		if (i == 0)
		{
			theta |= c;
		}
		else if (i == 1)
		{
			theta |= (c << 8);
		}
		i++;
	}

	return theta;
}

int16_t FSS100::getPhi()
{
	Wire.beginTransmission(_addr);
	Wire.write(0x02);
	Wire.endTransmission();

	int16_t phi = 0;
	int i = 0;
	Wire.requestFrom(_addr, 2);
	while(Wire.available())
	{
		char c = Wire.read();
		if (i == 0)
		{
			phi |= c;
		}
		else if (i == 1)
		{
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

void FSS100::update_conl(void)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0A);
	Wire.write(_conl_reg);
	Wire.endTransmission();
}

void FSS100::setContinuousSampling(bool continuous)
{
	if (continuous) _conl_reg |= FSS100_CONL_CONTINUOUS;
	else _conl_reg &= FSS100_CONL_ONESHOT;
}

void FSS100::setSamplingRate(int samplingRate)
{
	_conl_reg |= samplingRate;
}

void FSS100::setSampleBit(bool sample)
{
	if (sample) _conl_reg |= FSS100_CONL_SAMPLEREQUEST;
	else _conl_reg &= FSS100_CONL_SAMPLEDONE;
}