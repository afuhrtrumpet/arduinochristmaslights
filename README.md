Arduino Powered Christmas Lights
================================

This project uses a variety of inputs--two buttons, a potentiometer, and a PIR sensor--to control an RGB LED strip and EL wire simultaneously.

Here's the wiring diagram:
![Christmas Lights Wiring Diagram](http://afuhrtrumpet.github.io/images/christmaslights.png "Wiring diagram")

# Files

* The first program (without the v3) does not utilize the buttons or LCD. It gradually fades the LED strip through the entire color spectrum and blinks the EL wire in an interval specified by the position of the potentiometer. After doing this for a certain amount of time, both components shut off to save power. They are reactivated when the PIR sensor detects motion.
* The second program (V3) performs similarly to the first program, but at the start it asks the user to input a sequence of colors by twisting the potentiometer to specify levels of red, green, and blue. After each color has been entered, the user can hit the bottom button to add another color or the top button to finish and start the sequence. Then, the EL wire performs as described in the first program while the LED strip switches between the user-defined sequence of colors in the same time interval the EL wire blinks. Both lights shut off after a certain period of time to wait for motion. The user can also press the top button during the light sequence to enter a new sequence of colors.

# Possible future improvements
* Two potentiometers should be used: one to set the colors, and one to control the blink delay. In the current configuration, for example, if blue is set to 0 in the last color added and the user forgets to set the potentiometer back to a non-zero value, the blinking occurs faster than the relay can handle, causing a buzzing sound.
* The option to have the LED strip fade could still exist, or an option to have the strip fade between colors could be added.
* For some reason when setting colors, a low blue value makes the LED strip blink rapidly between a blue and non-blue color.
* The relay used to control the EL wire is noisy. Alternate control methods could be looked into for this.
