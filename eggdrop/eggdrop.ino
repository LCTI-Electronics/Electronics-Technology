// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 870.0

Adafruit_MPU6050 mpu;
RH_RF95 rf95(RFM95_CS, RFM95_INT);

bool falling = false;

long timer;
float airtime;

void setup(void) {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setTxPower(13, false);
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

int16_t packetnum = 0;
char radiopacket[8];

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  float xa = a.acceleration.x;
  float ya = a.acceleration.y;
  float za = a.acceleration.z;

  float at = sqrt(xa*xa + ya*ya + za*za);

  if(at < 1 && !falling) {
    timer = millis();
    falling = true;
  }

  if(falling) {
    Serial.println("Falling");
    if(at > 1) {
      airtime = millis() - timer;
      float dist = distance(airtime);
      float speed = velocity(airtime);

      float data[2] = {airtime, at};

      Serial.print(airtime);
      Serial.println(" ms");
      Serial.println("");
      Serial.print(dist);
      Serial.println(" m");

      memcpy(radiopacket, data, sizeof(data));
      Serial.println("Sending");
      rf95.send(radiopacket, sizeof(radiopacket));
      rf95.waitPacketSent();
      Serial.println("Sent");
      falling = false;
    }
  }

  delay(20);
}

float distance(float time) {
  float sec = time/1000;
  float d = (0.5 * 9.8 * sq(sec));
  return d;
}

float velocity(float time) {
  float sec = time/1000;
  float spd = (9.8 * sec);
  return spd;
}