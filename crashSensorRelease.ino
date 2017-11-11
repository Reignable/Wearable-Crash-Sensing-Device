/**
 * crashSensorRelease.ino
 * Purpose: Provides functionality and user interaction to StackSense.
 *          Can calculate angles from accelerometer and gyroscope data
 *
 * @author Joe Barrett
 * @version 0.4 01/04/2016
 */
#include <vector>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include <i2c_t3.h>

enum DeviceCommand {
  START = '1',
  STOP = '0',
  CALIBRATE = 'c',
  DO_NOTHING = ' ',
  ALERT = 's'
};

/**
 * Sensor objects
 */
Accelerometer accH("AccH", A7, A8, A9, 3.00, (pow(2, 10) - 1), 1.50, 0.66);//High G accelerometer
//X and Y axis swapped due to chip's orientation on board in relation to other chips.
Accelerometer accL("AccL", A1, A0, A2, 3.00, (pow(2, 10) - 1), 1.50, 0.30); //Low G accelerometer
Gyroscope gyro;

std::vector <Sensor*> sensorList;

/**
 * Characters for user commands
 */
//char command = 0;
DeviceCommand command = DO_NOTHING;

/**
 * LED pin on Teensy 3.2
 */
const int LED = 13;

/**
 * Times used for calculating gyroscope angles
 */
unsigned long lastReadTime,
         timeNow;

/**
 * Data members for storing and calculating angles
 */
double delta, //Difference between current time and last measured time, used for Gyroscope angles
       alpha = 0.96, //Used for Complementary filter. Represents the proportion of the Gyroscope angle to use.
       //Angles of each axis
       xAngle,
       yAngle,
       zAngle,//Only used for processing visuals
       //Accelerometer angles
       yAccAngle,
       xAccAngle,
       zAccAngle,
       //Gyroscope Angles
       xGyrAngle,
       yGyrAngle,
       zGyrAngle,
       //Last measured angles
       xLastAngle,
       yLastAngle,
       zLastAngle,
       //Last measured Gyroscope angles
       xGyrLastAngle,
       yGyrLastAngle,
       zGyrLastAngle,
       //Used for processing visuals
       xGyrAngleUnfiltered,
       yGyrAngleUnfiltered,
       zGyrAngleUnfiltered;

const double ANGLE_THRESHOLD = 100.00, G_THRESHOLD = 5.00;

/**
 * Sets the lase angle data for use when processing new angles.
 *
 * @param time - unsigned long - the time of the last set of calculations
 * @param x - double - last X axis angle
 * @param y - double - last Y axis angle
 * @param z - double - last Z axis angle
 * @param xGyro - double - last X axis angle from the gyroscope
 * @param yGyro - double - last Y axis angle from the gyroscope
 * @param zGyro - double - last Z axis angle from the gyroscope
 */
void setLastAngleData(unsigned long time, double x, double y, double z, double xGyro, double yGyro, double zGyro) {
  lastReadTime = time;
  xLastAngle = x;
  yLastAngle = y;
  zLastAngle = z;
  xGyrLastAngle = xGyro;
  yGyrLastAngle = yGyro;
  zGyrLastAngle = zGyro;
}

/**
 * Calibrates the sensors. Reads each sensor over a second to create an average of readings when idle
 * then sets each sensor's zero data members using the average.
 * Sets last angle data as zero.
 */
void calibrate() {
  double startTime = millis();
  while (millis() - startTime <= 1000.00) {
    for (unsigned int i = 0; i < sensorList.size(); i++) {
      sensorList[i]->readPins();
    }
  }
  for (unsigned int i = 0; i < sensorList.size(); i++) {
    sensorList[i]->setZero();
  }
  setLastAngleData(millis(), 0, 0, 0, 0, 0, 0);
}

/**
 * Calculates angles (Pitch and Roll) using accelerometer and gyroscope data.
 * Applies complimentery filter using angles from accelerometer and gyroscope.
 */
void calculateAngles() {

  timeNow = millis();

  //Accelerometer Pitch: ArcTan(Ay / √(Ax ^ 2 + Az ^ 2))
  xAccAngle = -atan(accL.yCalculated / sqrt(pow(accL.xCalculated, 2) + pow(accL.zCalculated, 2))) * RAD_TO_DEG;
  //Accelerometer Roll: ArcTan(-Ax / √(Ay ^ 2 + Az ^ 2))
  yAccAngle = atan(-1 * accL.xCalculated / sqrt(pow(accL.yCalculated, 2) + pow(accL.zCalculated, 2))) * RAD_TO_DEG;
  //Yaw not needed
  zAccAngle = 0;

  //Elapsed time since last reading in microseconds
  delta = (timeNow - lastReadTime) / 1000.0;

  //Gyro Angles: Rate(deg/s) * elapsed time + last measured angle
  xGyrAngle = gyro.xCalculated * delta + xLastAngle;
  yGyrAngle = gyro.yCalculated * delta + yLastAngle;
  zGyrAngle = gyro.zCalculated * delta + zLastAngle;

  //Unfiltered gyro angles for processing visualisation
  xGyrAngleUnfiltered = gyro.xCalculated * delta + xGyrLastAngle;
  yGyrAngleUnfiltered = gyro.yCalculated * delta + yGyrLastAngle;
  zGyrAngleUnfiltered = gyro.zCalculated * delta + zGyrLastAngle;

  //Comp filter
  xAngle = alpha * xGyrAngle + (1.0 - alpha) * xAccAngle;
  yAngle = alpha * yGyrAngle + (1.0 - alpha) * yAccAngle;
  zAngle = alpha * zGyrAngle + (1.0 - alpha) * zAccAngle;

  //Set angle data for next calculation round
  setLastAngleData(timeNow, xAngle, yAngle, zAngle, xGyrAngleUnfiltered, yGyrAngleUnfiltered, zGyrAngleUnfiltered);
}

/**
 * Run when device is turned on. Initialises sensor vector and begins serial communications.
 */
void setup() {
  sensorList.push_back(&accL);
  sensorList.push_back(&accH);
  sensorList.push_back(&gyro);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  command = DO_NOTHING;
}

/**
 * Runs forever
 */
void loop() {
  //Capture user command from either serial monitor or over Bluetooth
  if (Serial.available() || Serial1.available()) {
    command = Serial.available() ? (DeviceCommand)Serial.read() : (DeviceCommand)Serial1.read();
  }
  switch (command) {

    case 'c':
      gyro.setupGyro();
      calibrate();
      command = STOP;
      break;

    case '1':
      for (unsigned int i = 0; i < sensorList.size(); i++) {
        sensorList[i]->calculate();
      }
      calculateAngles();
      if (xAngle > ANGLE_THRESHOLD ||
          yAngle > ANGLE_THRESHOLD ||
          xAngle < -ANGLE_THRESHOLD ||
          yAngle < -ANGLE_THRESHOLD ||
          accH.xCalculated >= G_THRESHOLD ||
          accH.yCalculated >= G_THRESHOLD ||
          accH.zCalculated >= G_THRESHOLD ||
          accH.xCalculated <= -G_THRESHOLD ||
          accH.yCalculated <= -G_THRESHOLD ||
          accH.zCalculated <= -G_THRESHOLD) {
        digitalWrite(LED, HIGH);
        Serial1.println("s");
        Serial.println("s");
        command = STOP;
      }
      break;

    case '0':
      setLastAngleData(timeNow, 0, 0, 0, 0, 0, 0);
      digitalWrite(LED, LOW);
      command = DO_NOTHING;
      break;

    default:
      break;
  }
  delay(5);
}
