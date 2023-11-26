#include <Arduino.h>
#include <Encoder.h>
#include <HID-Project.h>
#include <HID-Settings.h>

void toggleLayer();
void layerOne();
void layerTwo();
void layerThree();

//buttonz
const int rotaryButton = 15; //rotary encoder button
const int layerButton = rotaryButton;
const int joystickButton = 14; //joystick button
const int leftButton = 3; //left red minus button
const int middleButton = 2; //middle green mute button
const int rightButton = 4; //left yellow plus button

//led - pin8 -> green middle | pin9 -> yellow right | pin7 -> red left
const int LEDLayerOne = 8;
const int LEDLayerTwo = 9; 
const int LEDLayerThree = 7; 

int layer = 3;  // Initial layer

//Analog
const int joystickXPin = A0;  // Analog pin for X-axis
const int joystickYPin = A1;  // Analog pin for Y-axis
Encoder myEncoder(A2, A3); // DT connected to A2, CLK connected to A3
int mouseSpeed = 1; // 1-10

// Variables to track the state of the encoder
int lastCLKState; 
int currentCLKState;

void setup() {
  pinMode(layerButton, INPUT_PULLUP);
  pinMode(joystickButton, INPUT_PULLUP);
  pinMode(LEDLayerOne, OUTPUT);
  pinMode(LEDLayerTwo, OUTPUT);
  pinMode(LEDLayerThree, OUTPUT);
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  // check if layerButton is pressed and change the layer
  if (digitalRead(layerButton) == LOW){
    delay(50);  // Debouncing delay
    while (digitalRead(layerButton) == LOW) {}  // Wait for button release
    toggleLayer();
  }
  // reset to layer one with 2 buttons at the same time (the rotary encoder and the joystick buttons)
  //if (digitalRead(joystickButton) == LOW && digitalRead(layerButton) == LOW) {
  //  delay(50);  // Debouncing delay
  //  while (digitalRead(joystickButton) == LOW && digitalRead(layerButton) == LOW) {}  // Wait for button release
  //  layer = 3;
  //}
  //======================call the layer functions based on the pressed button======================
  //layerz code
  if (layer == 1){
    layerOne();
    digitalWrite(LEDLayerOne, HIGH); 
    digitalWrite(LEDLayerTwo, LOW);
    digitalWrite(LEDLayerThree, LOW);
  } else if (layer == 2){
    layerTwo();
    digitalWrite(LEDLayerTwo, HIGH);
    digitalWrite(LEDLayerOne, LOW);
    digitalWrite(LEDLayerThree, LOW);
  } else if (layer == 3){
    layerThree();
    digitalWrite(LEDLayerThree, HIGH);
    digitalWrite(LEDLayerOne, LOW);
    digitalWrite(LEDLayerTwo, LOW);
  }
}

void toggleLayer(){
  layer = (layer == 3) ? 1 : (layer + 1);
}

void layerOne(){
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

  //=======================Rotary encoder as scroll wheel===================
  static long oldPosition = -1;
  long newPosition = myEncoder.read();
  //clockwise
  if (newPosition > oldPosition) {
      Mouse.move(0, 0, 1);  // Scroll up
  //counterclockwise
  } else if (newPosition < oldPosition) {
      Mouse.move(0, 0, -1);  // Scroll down
  }
  //oldPosition = newPosition;
  if (newPosition != oldPosition) {
    Serial.println(newPosition);
    oldPosition = newPosition;
  }
  //=======================END delay=========================
  Serial.println("Layer One is Active at the moment");
  delay(10);  // Add a small delay to avoid rapid cursor movements
}

void layerTwo(){
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
    // Press down Middle mouse button
    Mouse.press(MOUSE_MIDDLE);
  } else {
    Mouse.release(MOUSE_MIDDLE);
  }
  // Calibration adjustments for X-axis
  int xMapped = map(xValue, 510 - 50, 510 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Calibration adjustments for Y-axis
  int yMapped = map(yValue, 493 - 50, 493 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Move the mouse cursor
  yMapped *= -1;
  Mouse.move(xMapped, yMapped);

  //=======================Rotary encoder as scroll wheel===================
  static long oldPosition = -1;
  long newPosition = myEncoder.read();
  //clockwise
  if (newPosition > oldPosition) {
      Mouse.move(0, 0, 1);  // Scroll up
  //counterclockwise
  } else if (newPosition < oldPosition) {
      Mouse.move(0, 0, -1);  // Scroll down
  }
  //oldPosition = newPosition;
  if (newPosition != oldPosition) {
    Serial.println(newPosition);
    oldPosition = newPosition;
  }

  //=======================END delay=========================
  Serial.println("Layer Two is Active at the moment");
  delay(10);  // Add a small delay to avoid rapid cursor movements
}

void layerThree(){
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

  // Calibration adjustments for X-axis
  int xMapped = map(xValue, 510 - 50, 510 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Calibration adjustments for Y-axis
  int yMapped = map(yValue, 493 - 50, 493 + 50, -mouseSpeed, mouseSpeed);  // Adjust the second and third parameters based on your preference
  // Move the mouse cursor
  Mouse.move(xMapped, yMapped);

  //=======================Rotary encoder as scroll wheel===================
  static long oldPosition = -1;
  long newPosition = myEncoder.read();
  //clockwise
  if (newPosition > oldPosition) {
      //Mouse.move(0, 0, 1);  // Scroll up
      Keyboard.press(KEY_VOLUME_UP);
      delay(100);
      Keyboard.releaseAll();
  //counterclockwise
  } else if (newPosition < oldPosition) {
      //Mouse.move(0, 0, -1);  // Scroll down
      Keyboard.press(KEY_VOLUME_DOWN);
      delay(100);
      Keyboard.releaseAll();
  }
  //oldPosition = newPosition;
  if (newPosition != oldPosition) {
    Serial.println(newPosition);
    oldPosition = newPosition;
  }

  //=======================END delay=========================
  Serial.println("Layer Three is Active at the moment");
  delay(10);  // Add a small delay to avoid rapid cursor movements
}