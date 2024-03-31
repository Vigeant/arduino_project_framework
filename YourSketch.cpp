// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"
#include "CommandLine.h"
#include "Settings.h"

CommandLine cli;
Settings settings;
// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    //Serial.println("Hello, world!");
    cli.doCommandLine();
    delay(1);
}
