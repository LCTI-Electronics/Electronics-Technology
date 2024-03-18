int leds[] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2};

const long interval = 1 * 1000;

void setup() {
  for(int led : leds) {
    pinMode(led, OUTPUT);
  }
}

void loop() {
  blink();
}

void blink() {
  int chosenLed = random(0, 12);

  digitalWrite(leds[chosenLed], HIGH);
  delay(interval);
  digitalWrite(leds[chosenLed], LOW);
}
