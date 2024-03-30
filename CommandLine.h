#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "Arduino.h"
#include <vector>

#define MAX_COMMAND_LENGTH 64
#define MAX_ARGUMENTS 5

// Command structure
typedef struct {
  const char* command;
  void (*function)(int argc, char* argv[]);
  const char* help;
} Command;

// Forward declaration
void doCommandLine();

#endif /* COMMANDLINE_H */
