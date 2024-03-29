#ifndef ARDUINO_H
#define ARDUINO_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

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

unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Define setup and loop functions as well
void setup();
void loop();

#endif /* ARDUINO_H */
