#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Encoder.h>
#include <HID.h>

const int joystickXPin = A0;  // Analog pin for X-axis
const int joystickYPin = A1;  // Analog pin for Y-axis
Encoder myEncoder(A2, A3); // DT connected to A2, CLK connected to A3
int mouseSpeed = 1; // 1-10

// Variables to track the state of the encoder
int lastCLKState; 
int currentCLKState;

void setup() {
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  //=======================joystick as mouse===================
  // Read analog values from the joystick
  int xValue = analogRead(joystickXPin);//added a bit of a dead zone
    if (xValue >= 508 && xValue <= 512) {
      xValue = 510;
  }
  int yValue = analogRead(joystickYPin);//added a bit of a dead zone
    if (yValue >= 491 && yValue <= 495) {
      yValue = 493;
  }

  //=======================Press Button if x or y is moved
  if ((xValue < 508 || xValue > 512) || (yValue < 491 || yValue > 495)) {
    // Press down Shift and Middle mouse button
    Keyboard.press(KEY_LEFT_SHIFT);
    Mouse.press(MOUSE_MIDDLE);
    // Debugging statements
    //Serial.println("Out of range - Pressing Shift and Middle Mouse Button");
  } else {
    // Release Shift and Middle mouse button if within the specified range
    Keyboard.releaseAll();
    Mouse.release(MOUSE_MIDDLE);
    // Debugging statements
    //Serial.println("Within range - Releasing Shift and Middle Mouse Button");
  }
  // Calibration adjustments for X-axis
  int xMapped = map(xValue, 510 - 50, 510 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Calibration adjustments for Y-axis
  int yMapped = map(yValue, 493 - 50, 493 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Move the mouse cursor
  Mouse.move(xMapped, yMapped);

  //=======================rotary encoder as scroll wheel===================
  static long oldPosition = -1;
  long newPosition = myEncoder.read();
  //clockwise
  if (newPosition > oldPosition) {
      Mouse.move(0, 0, 1);  // Scroll up
  //counterclockwise
  } else if (newPosition < oldPosition) {
      Mouse.move(0, 0, -1);  // Scroll up
  }
  //oldPosition = newPosition;
  if (newPosition != oldPosition) {
    Serial.println(newPosition);
    oldPosition = newPosition;
  }
  //=======================END delay=========================
  delay(10);  // Add a small delay to avoid rapid cursor movements
}