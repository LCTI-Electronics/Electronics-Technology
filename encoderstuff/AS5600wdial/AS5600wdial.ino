#include "AS5600.h"

AS5600 encoder;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  encoder.begin();
  
  Serial.print("Encoder status: ");
  Serial.println(encoder.isConnected());
  // 161
  //encoder.setZPosition(800);
  delay(1000);
}

void loop() {
  double angle = inputModulus(encoder.readAngle() * AS5600_RAW_TO_RADIANS, -PI, PI); //normalize
  //float angle = encoder.rawAngle();
  Serial.println(angle);
}

double inputModulus(double input, double minimumInput, double maximumInput) {
    double modulus = maximumInput - minimumInput;

    int numMax = (int) ((input - minimumInput) / modulus);
    input -= numMax * modulus;

    int numMin = (int) ((input - maximumInput) / modulus);
    input -= numMin * modulus;

    return input;
  }
