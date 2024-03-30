// Arduino.cpp - File containing main function and Arduino simulation implementations

#include "Arduino.h"
#include "YourSketch.h"

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

// Main function
int main() {
    setup(); // Call setup function

    while (true) {
        loop(); // Call loop function
        //delay(1000); // Simulate delay
        //printf("test");
    }

    return 0;
}
