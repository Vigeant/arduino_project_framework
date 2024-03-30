// Arduino.h - Header file containing Arduino simulation functionalities

#ifndef ARDUINO_H
#define ARDUINO_H

// Include necessary libraries
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

// Define Serial class
class SerialClass {
public:
    void begin(int baudrate) {
        std::cout << "Serial communication initialized with baudrate: " << baudrate << std::endl;
    }

    void println(const std::string& message) {
        std::cout << message << std::endl;
    }
};

extern SerialClass Serial;
//SerialClass Serial;

// Define millis() and delay() functions
unsigned long millis();
void delay(unsigned long ms);




#endif /* ARDUINO_H */
