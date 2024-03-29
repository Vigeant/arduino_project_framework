#include "Arduino.h"

// Your Arduino code goes here
void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Hello, world!");
    delay(1000);
}
