#ifndef FSS100_h
#define FSS100_h

#include <Arduino.h>

#define FSS100_SR_CLEAR ~((1 << 4) | (1 << 3))
#define FSS100_SR_32HZ (1 << 4) | (1 << 3)
#define FSS100_SR_16HZ (1 << 4) | (0 << 3)
#define FSS100_SR_8HZ (0 << 4) | (1 << 3)
#define FSS100_SR_4HZ (0 << 4) | (0 << 3)

#define FSS100_control_COMMIT (1 << 5)
#define FSS100_control_COMMIT_CLEAR ~FSS100_control_COMMIT

#define FSS100_control_CONTINUOUS (1 << 6)
#define FSS100_control_ONESHOT ~FSS100_control_CONTINUOUS

#define FSS100_control_SAMPLEREQUEST (1 << 7)
#define FSS100_control_SAMPLEDONE ~FSS100_control_SAMPLEREQUEST

class FSS100
{
	public:
	/*
	 * Constructor
	 * @param address I2C address of the FSS100 sensor (default is 0x11)
	 */
	FSS100(int address);

	/*
	 * Begins the Wire library. It also sets the FSS100 Sun Vector Sensor
	 * Control Register to default values.
	 */
	void init();

	/*
	 * Gets the values of theta, phi, and temperature from the FSS100 sensor.
	 * @param theta Pointer to the variable that will hold the theta value
	 * @param phi Pointer to the variable that will hold the phi value
	 * @param temp Pointer to the variable that will hold the temperature value
	 */
	void getSample(int16_t *theta, int16_t *phi, int16_t *temp);

	/*
	 * Returns the currently stored theta value.
	 */
	int16_t getTheta();

	/*
	 * Returns the currently stored phi value.
	 */
	int16_t getPhi();
	
	/*
	 * Returns the currently stored temperature value.
	 */
	int16_t getTemp();

	/*
	 * Returns the current control byte. Check the README.md and the FSS100 datasheet for more information on the control register.
	 */
	uint8_t get_control(void);
	void set_control(uint8_t new_control);
	void set_one_shot(void);
	uint8_t getAddress(void);

	/*
	 * Takes an unsigned 8 bit integer, which will be set as the new I2C address of the FSS100 sun sensor.
	 */
	void setI2CAddress(int address);

	/*
	 * Takes an unsigned 8 bit integer which will be assigned to control register. The control register is described in README.md and in the FSS100 datasheet.
	 */
	void update_control();

	/* 
	 * Sets the continuous sampling bit in the control register. If set to true, the sensor will continuously sample data. If set to false, the sensor will only sample data when requested.
	 * @param continuous True to set the continuous sampling bit, false for one-shot sampling.
	 */
	void setContinuousSampling(bool continuous);

	/*
	 * Sets the sampling rate of the sensor. 
	 * @param rate The desired sampling rate in Hz. Must be one of the following: 4, 8, 16, or 32Hz.
	 */
	void setSamplingRate(int rate);

	/*
	 * Monitors the sample bit in CONL for 5 tries and returns true if sample bit goes low, or false is sample bit does not go low.
	 * If this function returns true, then the needed data register can be collected.
	 */
	bool sample_wait(void);

	void setSampleBit(bool sample);
	void setCommitBit(bool commit);
	
	/*
	 * Sets the expected default configuration, which is 16 Hz sampling rate and one shot sampling mode.
	 */
	void default_config(void);
	void continuous_config(void);

	private:
	int _addr;
	uint8_t _control_reg;
};

#endif // FSS100
