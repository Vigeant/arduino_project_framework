// Arduino.h - Header file containing Arduino simulation functionalities

#ifndef ARDUINO_H
#define ARDUINO_H

// Include necessary libraries
#include <cstdio>
#include <cstdint>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <ncurses.h>
#endif


// Define Serial class
class SerialClass {
public:
    void begin(int baudrate) {
        std::printf("Serial communication initialized with baudrate: %d\n", baudrate);
    }

    void putc(char c) {
        // Output the character to the standard output
        std::putchar(c);
    }

    void print(const char* message) {
        std::printf("%s", message);
    }

    void println(const char* message) {
        std::printf("%s\n", message);
    }

    template<typename... Args>
    void printf(const char* format, Args... args) {
        std::printf(format, args...);
    }

    int available() {
        #ifdef _WIN32
            // Windows-specific code
            return _kbhit();
        #else
            // Unix/Linux-specific code
            fd_set readfds;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            select(STDIN_FILENO+1, &readfds, NULL, NULL, &tv);
            return FD_ISSET(STDIN_FILENO, &readfds);
        #endif
    }

    int read() {
        #ifdef _WIN32
            // Windows-specific code
            return getchar();
        #else
            return getch();
        #endif
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