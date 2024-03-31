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

    void putc(char c) {
        // Output the character to the standard output
        //putchar(c);
        std::cout << c;
    }

    void print(const std::string& message) {
        std::cout << message;
    }

    void println(const std::string& message) {
        std::cout << message << std::endl;
    }

    template<typename... Args>
    void printf(const char* format, Args... args) {
        std::printf(format, args...);
    }

    int available() {
        // Simulate available characters to read from stdin
        return std::cin.peek() == EOF ? 0 : 1;
    }

    int read() {
        // Read and remove the first character from stdin
        char c;
        std::cin.get(c);
        return c;
    }
};

class EEPROMClass {
    public:
    int put(uint32_t address, uint32_t value){
        return 0;
    }
    int get(uint32_t address, uint32_t value){
        return 0;
    }
};

extern SerialClass Serial;
extern EEPROMClass EEPROM;

// Define millis() and delay() functions
unsigned long millis();
void delay(unsigned long ms);

#endif /* ARDUINO_H */