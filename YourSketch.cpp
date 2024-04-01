// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"
#include "CommandLine.h"
#include "Settings.h"
#include "Controller.h"

CommandLine cli;
Settings settings;
Context context(std::make_unique<InitialState>()); // Initial state

// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    //Serial.println("Hello, world!");
    cli.doCommandLine();
    context.request();
    delay(25);
}
