// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"

// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Hello, world!");
    delay(1000);
}
