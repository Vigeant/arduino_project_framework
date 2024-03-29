#include "Arduino.h"

// Main function to simulate Arduino environment
int main() {
    setup(); // Initialize
    unsigned long lastMillis = 0;
    while (true) {
        // Simulate loop execution
        if (millis() - lastMillis >= 1000) {
            loop(); // Execute loop
            lastMillis = millis(); // Update last execution time
        }
    }
    return 0;
}
