// YourSketch.cpp - File containing your actual Arduino code

#include "Arduino.h"
#include "YourSketch.h"
#include "CommandLine.h"
#include "Settings.h"
#include "Controller.h"

CommandLine cli;
Settings settings;
Context context; // Initial state

// Your Arduino code goes here
void setup()
{
    Serial.begin(9600);
    setupController(context);
    cli.init();
}

void loop()
{
    // Serial.println("Hello, world!");
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        cli.doCommandLine(c);
    }
    context.request();
    delay(25);
}
