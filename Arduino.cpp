// Arduino.cpp - File containing main function and Arduino simulation implementations

#include "Arduino.h"
#include "YourSketch.h"

#include <iostream>
#include <Windows.h>

SerialClass Serial;

// Implement millis() function
unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// Implement delay() function
void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void enableRawInputMode() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT));
}

// Main function
int main() {
    enableRawInputMode();
    setup(); // Call setup function

    while (true) {
        loop(); // Call loop function
        //delay(1000); // Simulate delay
        //printf("test");
    }

    return 0;
}
