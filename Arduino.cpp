// Arduino.cpp - File containing main function and Arduino simulation implementations

#include "Arduino.h"
#include "YourSketch.h"

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <chrono>
#include <thread>
#include <fcntl.h>

SerialClass Serial;
EEPROMClass EEPROM;

// Implement millis() function
unsigned long millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

// Implement delay() function
void delay(unsigned long ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Main function simulating arduino behavior with platform dependant code
int main()
{
    int result;
// Set "stdin" to have binary mode:
#ifdef _WIN32
    result = _setmode(_fileno(stdin), _O_BINARY);
    if (result == -1)
        perror("Cannot set mode");
    else
        printf("'stdin' successfully changed to binary mode\n");
#else
    // setvbuf(stdin, NULL, _IONBF, 0);
    struct termios old_settings, new_settings;


    initscr();
    cbreak();
    noecho();
    nonl();

    tcgetattr(0, &old_settings);
    new_settings = old_settings;
    new_settings.c_oflag = new_settings.c_oflag | ONLCR;
    tcsetattr(0, TCSANOW, &new_settings);
#endif

    // setvbuf(stdout, NULL, _IONBF, 0);
    setbuf(stdout, NULL);

#ifdef _WIN32
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hInput, &mode);
    // SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT));
    // SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT ));
    SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT));
#endif

    setup(); // Call setup function

    while (true)
    {
        loop(); // Call loop function
    }
    return 0;
}
