#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

float RF95_FREQ = 870.0;

RH_RF95 rf95(RFM95_CS, RFM95_INT);

long timestamp;

const byte interruptPin = 3;     
const byte ledGreen =  8; // 870mHz Channel 3 - Green
const byte ledRed =  7;   // 915mHz Channel 2 - Red
const byte ledBlue =  6;   // 890mHz Channel 1 - Blue

long debouncing_time = 500; //Debouncing Time in Milliseconds
volatile unsigned long last_micros; 

float airtime;

// Change between different egg frequency channels
void changeFreq() {
  if (RF95_FREQ == 890.0) 
  {
    RF95_FREQ = 915.0;
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledBlue,LOW);
  }
  else if (RF95_FREQ == 915.0)
  {
    RF95_FREQ = 870.0;
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledBlue,LOW);
  }
  else
  {
    RF95_FREQ = 890.0;
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledBlue,HIGH);
  }
  rf95.init();
  rf95.setFrequency(RF95_FREQ);
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
}

// Clean up the button press interrupt timing
void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    changeFreq();
    last_micros = micros();
  }
}

void setup() 
{
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), debounceInterrupt, CHANGE);

  // Startup channel Green 870
  digitalWrite(ledRed,LOW);
  digitalWrite(ledGreen,HIGH);
  digitalWrite(ledBlue,LOW);
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //while (!Serial);
  Serial.begin(115200);
  //Serial.println("Serial Connection Start");
  delay(50);
  
  // Send Data To start the EHub data logging
  timestamp = millis();
  Serial.print(timestamp);
  Serial.println(",gforce,1.00,1"); 
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  rf95.init();
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);
}



void loop()
{

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.available())
  {    
    if (rf95.recv(buf, &len))
    {

      airtime = (* (float *)&buf[0]);
      Serial.println(airtime);

      Serial.print("'distance,");
      Serial.print(distance(airtime));
      Serial.print(",velocity,");
      Serial.print(velocity(airtime));
      Serial.println("");
    }
    else
    {

    }
  }
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
