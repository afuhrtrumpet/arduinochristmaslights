//Include libraries and namespace
#include <LiquidCrystal.h>
#include <unwind-cxx.h>
#include <system_configuration.h>
#include <utility.h>
#include <StandardCplusplus.h>
#include <vector>

using namespace std;

//Define constants for delay time and pin numbers
#define delayTime 5
#define potPin A0
#define elPin 13
#define redPin 5
#define greenPin 6
#define bluePin 3
#define pirPin 7
#define bottomButtonPin 4
#define topButtonPin 2

//Define varialbes for timer value, color index, EL wire state
int blinkCount = 0;
int currentColor = 0;
int elVal = 1;

//Define variables representing button values and previous values, active state, active timer, and counts of activity
int topButtonVal = 1;
int bottomButtonVal = 1;
int topButtonValP = 1;
int bottomButtonValP = 1;
int active = 0;
int activeCount = 0;
int activeLimit = 4000;

//Setup LCD
LiquidCrystal lcd(12, 11, 10, 8, 1, 0);

//Define color class that holds ints representing red, blue, and green
class Color {
  public:
  Color(int red=255, int green=255, int blue=255);
  int r;
  int g;
  int b;
};

//Color class constructor
Color::Color(int red, int green, int blue) {
  r = red;
  g = green;
  b = blue;
}

//Define vector of colors to hold color sequence
vector<Color> colors;

//Function prototype for color setup sequence.
void setupColors();

void setup() {
  //Setup inputs and outputs
  lcd.begin(16, 2);
  pinMode(elPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(bottomButtonPin, INPUT);
  pinMode(topButtonPin, INPUT);
  //Call color setup method
  setupColors();
}

void loop() {
  //Set top button value to previous and read in new value
  topButtonValP = topButtonVal;
  topButtonVal = digitalRead(topButtonPin);
  //If top button is pressed, go back to color setup
  if (!topButtonVal && topButtonValP)
    setupColors();
  if (active) {
    //Write the current values to each of the outputs
    digitalWrite(elPin, elVal);
    analogWrite(redPin, colors.at(currentColor).r);
    analogWrite(greenPin, colors.at(currentColor).g);
    analogWrite(bluePin, colors.at(currentColor).b);
    //Read in blink time from potentiometer
    int blinkTime = analogRead(potPin);
    //If the count is greater than/equal to the potentiometer value
    if (blinkCount >= blinkTime) {
      //Change color and wire value
      blinkCount = 0;
      elVal = !elVal;
      //Increment current color and set it to 0 if greater then the size of colors vector
      currentColor = (currentColor + 1) % colors.size();
    }
    //Set active to false if count has hit active limit
    if (activeCount >= activeLimit) {
      active = false;
      activeCount = 0;
    }
    //Increment counts
    activeCount++;
    blinkCount++;
  } else {
    //Write everything to 0
    digitalWrite(elPin, LOW);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);
    //If siginal is read from the PIR sensor, switch back to active
    if (digitalRead(pirPin) == HIGH)
      active = true;
  }
  //Delay (for exact count measurement)
  delay(delayTime);
}

void setupColors() {
  //Set current color index and resize color vector to 0 to avoid any out of bounds issues
  currentColor = 0;
  colors.resize(0);
  //Loop control variable
  int done = 0;
  //Main loop
  do {
    //Define variables for RGB values selected by the user
    int currentRed = 0;
    int currentBlue = 0;
    int currentGreen = 0;
    //Set the colors of the LED strip to 0
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    //Start by having user set amount of red
    lcd.clear();
    lcd.print("Set red value.");
    do {
      //Set current button value to previous and read new one
      bottomButtonValP = bottomButtonVal;
      bottomButtonVal = digitalRead(bottomButtonPin);
      //Read potentiometer value, save it to current red and red on LCD strip
      currentRed = analogRead(potPin) / 1023.0 * 255;
      analogWrite(redPin, currentRed);
      //Check if bottom button is pressed (and wasn't in the previous iteration)
    } while (!(!bottomButtonVal &&  bottomButtonValP));
    //Now have user set green
    lcd.clear();
    lcd.print("Set green value.");
    do {
      //Set current button value to previous and read new one
      bottomButtonValP = bottomButtonVal;
      bottomButtonVal = digitalRead(bottomButtonPin);
      currentGreen = analogRead(potPin) / 1023.0 * 255;
      analogWrite(greenPin, currentGreen);
      //Check if bottom button is pressed (and wasn't in the previous iteration)
    } while (!(!bottomButtonVal &&  bottomButtonValP));
    //Now have user set blue
    lcd.clear();
    lcd.print("Set blue value.");
    do {
      //Set current button value to previous and read new one
      bottomButtonValP = bottomButtonVal;
      bottomButtonVal = digitalRead(bottomButtonPin);
      currentBlue = analogRead(potPin) / 1023.0 * 255;
      analogWrite(bluePin, currentBlue);
      //Check if bottom button is pressed (and wasn't in the previous iteration)
    } while (!(!bottomButtonVal &&  bottomButtonValP));
    //Begin options menu
    lcd.clear();
    lcd.print("Top -- finish");
    lcd.setCursor(0, 1);
    lcd.print("Bottom -- add");
    //Control variable for options loop
    int optionsDone = 0;
    do {
      //Set current button values to previous and read new ones
      bottomButtonValP = bottomButtonVal;
      bottomButtonVal = digitalRead(bottomButtonPin);
      topButtonValP = topButtonVal;
      topButtonVal = digitalRead(topButtonPin);
      //If bottom button is pressed, exit options menu
      if (!bottomButtonVal && bottomButtonValP)
        optionsDone = 1;
      //Ig top button is pressed, exit options menu and main loop
      else if (!topButtonVal && topButtonValP) {
        optionsDone = 1;
        done = 1;
      }
    } while (!optionsDone);
    //Resize vector to fit another color and add it.
    colors.resize(colors.size() + 1);
    colors.at(colors.size() - 1) = Color(currentRed, currentGreen, currentBlue);
  } while (!done);
  //Display new message, set active to true
  lcd.clear();
  lcd.write("Press top");
  lcd.setCursor(0, 1);
  lcd.write("to set colors.");
  active = 1;
}
