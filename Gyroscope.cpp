/**
 * Gyroscope.cpp
 * Purpose: Allows for reading of gyroscope data from I2C bus.
 * 			Converts ADC data from an ITG-3200 to degrees per second.
 *
 * @author Joe Barrett
 * @version 0.3 24/03/2016
 */

#include "Gyroscope.h"

/**
 * Constructor. Sets constants for register addresses of the
 *				I2C sensor. Sets the scale factor for data conversion.
 */
Gyroscope::Gyroscope() : Sensor(),
  WHO_AM_I(0x00),
  SMPLRT_DIV(0x15),
  DLPF_FS(0x16),
  DLPF_CFG_0(1 << 0),
  DLPF_CFG_1(1 << 1),
  DLPF_CFG_2(1 << 2),
  DLPF_FS_SEL_0(1 << 3),
  DLPF_FS_SEL_1(1 << 4),
  GYRO_XOUT_H(0x1D),
  GYRO_XOUT_L(0x1E),
  GYRO_YOUT_H(0x1F),
  GYRO_YOUT_L(0x20),
  GYRO_ZOUT_H(0x21),
  GYRO_ZOUT_L(0x22),
  itgAddress(0x69),
  SCALE_FACTOR(14.375) {name = "Gyro";}

/**
 *	Initializes communication between the I2C sensor and the microcontroller
 *	by reading the WHO_AM_I register. Configures the gyroscope for data collection
 *	by setting the output scale to +/- 2000 degrees per second and the sample rate
 *	to 100 hz.
 */
void Gyroscope::setupGyro() {
  Wire.begin();
  //char id = itgRead(itgAddress, 0x00);
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0 | DLPF_FS_SEL_1 | DLPF_CFG_0));
  itgWrite(itgAddress, SMPLRT_DIV, 9);
}

/**
 *	Allows for writing to I2C device registers. Begins a transmission with the
 *	device address. Writes the register address to be written to, then writes the
 *	data. Ends the transmission afterwards.
 *
 *	@param address - char - Address of the device to be written to
 *	@param registerAddress - char - Address of the register to be written to
 *	@param data - char - The data to be written to the register
 */
void Gyroscope::itgWrite(char address, char registerAddress, char data) {
  //Serial.println("GYR IGT Write");
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 *	Allow for reading from I2C device registers. Begins a transmission with the
 *	device address. Writes the register address to be read from then ends this
 *	transmission sequence. Begins a new transmission and requests data from the
 *	I2C device. If there is data available it is read, the transmission ended
 *	and the data is returned.
 *
 *	@param address - char - Address of the device to be written to
 *	@param registerAddress - char - Address of the register to be written to
 *	@return unsigned char - The data which is read from the
 */
unsigned char Gyroscope::itgRead(char address, char registerAddress) {
  unsigned char data = 0;
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  if (Wire.available()) {
    data = Wire.read();
  }
  Wire.endTransmission();
  return data;
}

/**
 * 	Read raw ADC data from a gyroscope. Takes the data from the
 *	high register shifted left by 8 bits. Then takes the data from
 *	the low register if available, making a 16bit reading.
 *
 * @param high - char - H register for the gyroscope
 * @param low - char - L register for the gyroscope
 * @return int16_t - reading from the gyroscope register. changed from int to int16_t
 * when moving from UNO to teensy due to Arduino using 8 bit ints and teensy using 32.
 */
int16_t Gyroscope::readRaw(char high, char low) {
  int16_t data = 0;
  data = itgRead(itgAddress, high) << 8;
  data |= itgRead(itgAddress, low);
  return data;
}

/**
 *	Reads the raw data from each axis register and sends it to
 *	the super readPins method to be smoothed.
 */
void Gyroscope::readPins() {
  //Serial.println("GYR reading pins");
  Sensor::readPins(readRaw(GYRO_XOUT_H, GYRO_XOUT_L), readRaw(GYRO_YOUT_H, GYRO_YOUT_L), readRaw(GYRO_ZOUT_H, GYRO_ZOUT_L));
}

/**
 *	Converts ADC data to degrees per second. The zero value is taken
 *	from the ADC average and the result is divided by the sensor's
 *	scale factor, provided by the datasheet.
 */
void Gyroscope::calculate() {
  //Serial.println("GYR calculating");
  readPins();
  xCalculated = ((xAverage - xZero) / SCALE_FACTOR);
  yCalculated = ((yAverage - yZero) / SCALE_FACTOR);
  zCalculated = ((zAverage - zZero) / SCALE_FACTOR);
}

/**
 *	Appends the sensor type to the printRaw function
 *	in the Sensor class.
 */
void Gyroscope::printRaw() {
  Serial.print("GYR =\t");
  Sensor::printRaw();
}

/**
 *	Appends the sensor type to the printCalculated function
 *	in the Sensor class.
 */
void Gyroscope::printCalculated() {
  Serial.print("GYR =\t");
  Sensor::printCalculated();
}


