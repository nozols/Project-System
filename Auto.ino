#define CALIBRATE_SENSORS false
#define LOGGER_SD_ENABLED false

#include <Wire.h>
#include <Servo.h>
#include <QTRSensors.h>
#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "ultrasone_sensor.c"

MPU6050 gyro(Wire);
Servo steerServo;
Servo motorServo;

#define SERVOPIN 11
#define MOTORPIN 23
#define TEMPPIN A9

#define MAXTEMP 100

#define QTR_NUM_SENSORS             8  // number of sensors used
#define QTR_NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define QTR_EMITTER_PIN             2  // emitter is controlled by digital pin 2
#define QTR_MIDDLE_LINE             3500 // puts the value of the middle of the line at 3500

QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8},
  QTR_NUM_SENSORS, QTR_NUM_SAMPLES_PER_SENSOR, QTR_EMITTER_PIN);
unsigned int qtr_sensorValues[QTR_NUM_SENSORS];
int qtr_lastPosition;

bool canStart = true;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire.h");
  gyro.begin();
  #if CALIBRATE_SENSORS
  Logger::info("Calibrating gyro. Do not move car.");
  gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");
  #endif
  Logger::info("Initializing ultrasone sensor");
  us_initialize();
  Logger::info("Finished initializing ultrasone sensor");

  Logger::info("Initializing steering servo");
  steerServo.attach(SERVOPIN);
  motorServo.attach(MOTORPIN);
  pinMode(TEMPPIN, INPUT);
  Logger::info("Finished initializing steering servo");

  #if CALIBRATE_SENSORS
  Logger::info("Calibrating linetracker");
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  Logger::info("Finished calibrating linetracker");
  #endif
  qtr_lastPosition = qtra.readLine(qtr_sensorValues) - QTR_MIDDLE_LINE;

  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }

  Logger::info("End of setup");
}

void loop()
{
  Logger::data("us", us_getDistance());
  Logger::data("gx", gyro.getAngleX());
  Logger::data("temp", temp_isTemperatureTooHigh());
  Logger::data("steer", qtr_getSteerAmount());
}

/**
 * Control the steering servo
 * TODO check min and max steering angle
 * @param int turnPercentage 0-50 left, 50-100 right
 */
void controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 0, 180);//convert procents to Servo value
  steerServo.write(turnServoValue);
}

/**
 * Control the main motor
 * @param int speedPercentage 0 is stop, 100 full throttle
 */
void controlMotor(int speedPercentage)
{
  if(temp_isTemperatureTooHigh())
  {//check if the temperature is too high
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }

  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  //analogWrite(MOTORPIN, speedPWMValue);
  Logger::info(speedPWMValue);
  motorServo.write(speedPWMValue);
}

/**
 * @return bool is the temperature of the motor too high?
 */
bool temp_isTemperatureTooHigh()
{
  return analogRead(TEMPPIN) > MAXTEMP;
}

/**
 * Get the steering direction => 0-50 left, 50-100 right
 * @return int
 */
int qtr_getSteerAmount()
{
  int position = qtra.readLine(qtr_sensorValues) - QTR_MIDDLE_LINE; //MIDDLE_LINE zorgt ervoor dat het midden van de lijn wordt aangegeven met 0 i.p.v. 3500.
  int difference = position - qtr_lastPosition;
  qtr_lastPosition = position;

  if (((position > 0) && (difference > 0))  || ((position) < 0 && (difference < 0)))
  {
    return position / 70 + 50; //levert een waarde van 0 tot 100 op. (<50 = links & >50 = rechts)
  }
  else if (position < (difference * -1))
  {
    return difference / 70 + 50; //levert een waarde van 0 tot 100 op. (<50 = links & >50 = rechts)
  }
}
