// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"
#include "CommandLine.h"

CommandLine cli;
// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    //Serial.println("Hello, world!");
    cli.doCommandLine();
    delay(1);
}
