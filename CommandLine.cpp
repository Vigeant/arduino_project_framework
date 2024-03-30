#include "Arduino.h"
#include <vector>
#include <string.h>
#include "CommandLine.h"


char* prompt = "cli$ ";

//forward declare all command functions
void commandHelp(int argc, char* argv[]);
void commandFunction1(int argc, char* argv[]);
void commandFunction2(int argc, char* argv[]);
void commandPrintArgs(int argc, char* argv[]);

// List of available commands
std::vector<Command> commands = {
  {"help", commandHelp, "this help"},
  {"command1", commandFunction1, "command1 help"},
  {"command2", commandFunction2, "command2 help"},
  {"printArgs", commandPrintArgs, "print arguments passed to a function"}
  // Add more commands here as needed
};


// Define your command functions here
void commandHelp(int argc, char* argv[]) {
    // Find and execute the command
    Serial.println("Available commands are: ");
    for (const auto& cmd : commands) {
        Serial.printf("%15s :\t%s\n",cmd.command,cmd.help);
    }
}

void commandFunction1(int argc, char* argv[]) {
  Serial.println("Executing commandFunction1");
}

void commandFunction2(int argc, char* argv[]) {
  Serial.println("Executing commandFunction2");
}

void commandPrintArgs(int argc, char* argv[]) {
  Serial.println("Executing commandPrintArgs");
  for (int i = 0 ; i < argc ; i++){
    Serial.printf("argv[%d] = %s\n", i, argv[i]);
  }
}

// Function to parse and execute commands
void doCommandLine() {
  static char buffer[MAX_COMMAND_LENGTH];
  static char* args[MAX_ARGUMENTS];
  static int argc=0;
  static bool first_prompt = true;

  if (first_prompt) {
    Serial.print(prompt);
    first_prompt = false;
  }

  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      buffer[argc] = '\0';
      //Serial.println(buffer);
      argc = 0;
      char* token = strtok(buffer, " ");

      for (int i=0 ; i < MAX_ARGUMENTS ; i++){
        args[i] = NULL;
      }

      while (token != NULL && argc < MAX_ARGUMENTS) {
        args[argc++] = token;
        token = strtok(NULL, " ");
      }

      // Find and execute the command
      for (const auto& cmd : commands) {
        if (args[0] == NULL){
            break;
        }

        if (strcmp(args[0], cmd.command) == 0) {
          cmd.function(argc, args);
          break;
        }
      }

      // Clear buffer
      memset(buffer, 0, sizeof(buffer));
      argc = 0;
      Serial.print(prompt);

    } else if (c == '\t') {
      // tab autocomplete.
      // if a single match is found, autocompletes the command
      // else print candidates
      int numCommandCandidates = 0;
      Command* commandCandidate;
      for (const auto& cmd : commands) {
        if (strncmp(buffer, cmd.command, argc) == 0) {
          numCommandCandidates++;
          commandCandidate = (Command*)&cmd;
          break;
        }
      }
      if (numCommandCandidates == 1){
        //auto complete the command
        strcpy(buffer, commandCandidate->command);
        argc = strlen(buffer);
        Serial.printf("\r%s%s",prompt,commandCandidate->command);
      }

    } else if (argc < MAX_COMMAND_LENGTH - 1) {
      buffer[argc++] = c;
      Serial.putc(c);
    }
  }
}
