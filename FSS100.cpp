#include "FSS100.h"
#include <Arduino.h>
#include <Wire.h>

FSS100::FSS100(int address)
{
	_addr = address;
	_control_reg = 0;
}

void FSS100::init()
{
	default_config();
}

void FSS100::default_config(void)
{
	setContinuousSampling(false);
	setSamplingRate(FSS100_SR_16HZ);
	setSampleBit(true);
	uint8_t current_control = get_control();
	if (current_control != _control_reg)  update_control();
}

void FSS100::continuous_config(void)
{
	setContinuousSampling(true);
	setSamplingRate(FSS100_SR_16HZ);
	setSampleBit(false);
	uint8_t current_control = get_control();
	if (current_control != _control_reg) update_control();
}

void FSS100::set_one_shot(void)
{
	set_control(get_control());
	setSampleBit(true);
	update_control();
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

void FSS100::getSample(int16_t *theta, int16_t *phi, int16_t *temp)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x00);
	Wire.endTransmission();

	int16_t theta_out = 0;
	int16_t phi_out = 0;
	int16_t temp_out = 0;

	int i = 0;

	Wire.requestFrom(_addr, 6);
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
		else if (i == 4)
		{
			temp_out = c;
		}
		else if (i == 5)
		{
			temp_out |= (c << 8);
		}

		i++;
	}
	*theta = theta_out;
	*phi = phi_out;
	*temp = temp_out;
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

int16_t FSS100::getTemp()
{
	Wire.beginTransmission(_addr);
	Wire.write(0x04);
	Wire.endTransmission();

	int16_t temp = 0;
	int i = 0;
	Wire.requestFrom(_addr, 2);
	while(Wire.available())
	{
		char c = Wire.read();
		if (i == 0)
		{
			temp |= c;
		}
		else if (i == 1)
		{
			temp |= (c << 8);
		}
		i++;
	}

	return temp;
}

void FSS100::setI2CAddress(int newAddress)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0B);
	Wire.write(newAddress);
	Wire.endTransmission();

	_addr = newAddress;
}

uint8_t FSS100::get_control(void)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0A);
	Wire.endTransmission();

	uint8_t control_reg = 0;
	int i = 0;
	Wire.requestFrom(_addr, 1);
	while(Wire.available())
	{
		control_reg = Wire.read();
	}

	return control_reg;
}

void FSS100::update_control(void)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0A);
	Wire.write(_control_reg);
	Wire.endTransmission();
}

void FSS100::set_control(uint8_t new_control)
{
	_control_reg = new_control;
}

uint8_t FSS100::getAddress(void)
{
	Wire.beginTransmission(_addr);
	Wire.write(0x0B);
	Wire.endTransmission();

	uint8_t addr_reg = 0;
	int i = 0;
	Wire.requestFrom(_addr, 1);
	while(Wire.available())
	{
		addr_reg = Wire.read();
	}

	return addr_reg;
}

void FSS100::setContinuousSampling(bool continuousIsEnabled)
{
	if (continuousIsEnabled) _control_reg |= FSS100_control_CONTINUOUS;
	else _control_reg &= FSS100_control_ONESHOT;
}

void FSS100::setSamplingRate(int samplingRate)
{
	_control_reg &= FSS100_SR_CLEAR;
	_control_reg |= samplingRate;
}

void FSS100::setCommitBit(bool commitBit)
{
	if (commitBit) _control_reg |= FSS100_control_COMMIT;
	else _control_reg &= FSS100_control_COMMIT_CLEAR;
}

void FSS100::setSampleBit(bool sampleBit)
{
	if (sampleBit) _control_reg |= FSS100_control_SAMPLEREQUEST;
	else _control_reg &= FSS100_control_SAMPLEDONE;
}