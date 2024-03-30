// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"
#include "CommandLine.h"

// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    //Serial.println("Hello, world!");
    doCommandLine();
    delay(1);
}
