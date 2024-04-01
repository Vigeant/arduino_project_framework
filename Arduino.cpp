// Arduino.cpp - File containing main function and Arduino simulation implementations

#include "Arduino.h"
#include "YourSketch.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <fcntl.h>
#include <io.h>

SerialClass Serial;
EEPROMClass EEPROM;

// Implement millis() function
unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// Implement delay() function
void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



// Main function simulating arduino behavior with platform dependant code
int main() {
    int result;
    // Set "stdin" to have binary mode:
    result = _setmode( _fileno( stdin ), _O_BINARY );

    if( result == -1 )
        perror( "Cannot set mode" );
    else
        printf( "'stdin' successfully changed to binary mode\n" );

    //setvbuf(stdout, NULL, _IONBF, 0);
    setbuf(stdout, NULL);

#ifdef _WIN32
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hInput, &mode);
    //SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT));
    //SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT ));
    SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT));
#endif

    setup(); // Call setup function

    while (true) {
        loop(); // Call loop function
    }

    //SetConsoleMode(hInput, mode);

    return 0;
}
