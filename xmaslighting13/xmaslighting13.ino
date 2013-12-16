#define delayTime 5
#define potPin A0    // select the input pin for the potentiometer
#define elPin 13   // select the pin for the EL wire
#define redPin 5
#define greenPin 6
#define bluePin 3
#define pirPin 7
//#define activeLimit 4000;
int blinkTime = 0; // variable to store the value coming from the sensor
int countFromBlink = 0;
int elVal = 1;
int fadeMode = 0;
int fadeCount = 0;
int active = false;
int activeCount = 0;
int activeLimit = 4000;

void setup() {
  pinMode(elPin, OUTPUT);  // declare the ledPin as an OUTPUT
  pinMode(pirPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  if (active) {
    digitalWrite(elPin, elVal);
    blinkTime = analogRead(potPin);
    if (fadeCount >= 256) {
      fadeCount = 0;
      fadeMode = (fadeMode + 1) % 6;
    }
    if (countFromBlink >= blinkTime) {
      countFromBlink = 0;
      elVal = !elVal;
    }
    if (activeCount >= activeLimit) {
      active = false;
      activeCount = 0;
    }
    switch (fadeMode) {
      case 0: analogWrite(redPin, fadeCount); break;
      case 1: analogWrite(bluePin, 255-fadeCount); break;
      case 2: analogWrite(greenPin, fadeCount); break;
      case 3: analogWrite(redPin, 255 - fadeCount); break;
      case 4: analogWrite(bluePin, fadeCount); break;
      case 5: analogWrite(greenPin, 255 - fadeCount); break;
    }
    activeCount++;
    countFromBlink++;
    fadeCount++;
  } else {
    digitalWrite(elPin, LOW);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);
    if (digitalRead(pirPin) == HIGH)
      active = true;
  }
  delay(delayTime);
}
