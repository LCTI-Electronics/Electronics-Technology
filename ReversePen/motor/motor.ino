#include <PID_v1.h>
#include "AS5600.h"

bool debug = false;

const int directionPin_right = 12;
const int directionPin_left = 13;
const int pwmPin_right = 3;
const int pwmPin_left = 11;
const int brakePin_right = 9;
const int brakePin_left = 8;
bool direction;

double setpoint, error, output;
PID driveController(&error, &output, &setpoint, 100, 0, 0.5, DIRECT); // try i 0.5 and d 0.5
AS5600 encoder;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  encoder.begin(4);
  encoder.setDirection(AS5600_CLOCK_WISE);

  encoder.setZPosition(800);

  error = 0;
  setpoint = 0; // 0 degrees

  pinMode(directionPin_right, OUTPUT);
  pinMode(directionPin_left, OUTPUT);
  pinMode(pwmPin_right, OUTPUT);
  pinMode(pwmPin_left, OUTPUT);
  pinMode(brakePin_right, OUTPUT);
  pinMode(brakePin_left, OUTPUT);

  driveController.SetOutputLimits(0, 100);
  driveController.SetMode(AUTOMATIC);

  brake_motors();

  direction = false;
  delay(3000);

  release_motors();
}

void loop() {
  float normalizedAngle = normalize(encoder.readAngle() * AS5600_RAW_TO_RADIANS, -PI, PI); // normalize -pi to pi for direction

  error = -abs(normalizedAngle);

  driveController.Compute();
  Serial.println(normalizedAngle);

  if (!debug) {
    if (normalizedAngle < 0) {
      digitalWrite(directionPin_right, LOW);
      digitalWrite(directionPin_left, LOW);
    } else if (normalizedAngle > 0) {
      digitalWrite(directionPin_right, HIGH);
      digitalWrite(directionPin_left, HIGH);
    }

    delayMicroseconds(10);

    // now do stuff
    analogWrite(pwmPin_left, 90);
    analogWrite(pwmPin_right, 90);
  }

  delay(5);
}

float normalize(float input, float minimumInput, float maximumInput) {
    float modulus = maximumInput - minimumInput;

    int numMax = (int) ((input - minimumInput) / modulus);
    input -= numMax * modulus;

    int numMin = (int) ((input - maximumInput) / modulus);
    input -= numMin * modulus;

    return input;
  }

void brake_motors() {
  digitalWrite(brakePin_right, HIGH);
  digitalWrite(brakePin_left, HIGH);
}

void release_motors() {
  digitalWrite(brakePin_right, LOW);
  digitalWrite(brakePin_left, LOW);
}