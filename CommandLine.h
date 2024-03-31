#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "Arduino.h"
#include <vector>

// #define MAX_COMMAND_LENGTH 64
// #define MAX_ARGUMENTS 5

// Command structure
typedef struct
{
    const char *command;
    void (*function)(int argc, char *argv[]);
    const char *help;
} Command;

// Define CliCommand interface
class CliCommand
{
public:
    CliCommand(const std::string &name, const std::string &description)
        : name(name), description(description) {}

    virtual int doCommand(int argc, std::vector<std::string> argv) = 0;
    virtual std::string doAutoComplete(std::string arg) = 0;

    const std::string &getName() const { return name; }
    const std::string &getDescription() const { return description; }

protected:
    std::string name;
    std::string description;
};

// Forward declaration
// void doCommandLine();

// declare commands
//  Define CommandHelp class
class CommandHelp : public CliCommand
{
public:
    CommandHelp(std::vector<CliCommand *> *cliCommands)
        : CliCommand("help", "show this help"), cliCommands(cliCommands) {}

    // Implement doCommand and doAutoComplete methods
    int doCommand(int argc, std::vector<std::string> argv) override
    {
        // Your implementation here
        Serial.printf("Available commands are:\n");
        for (const auto cmd : *cliCommands){
            Serial.printf("%15s : %s\n",cmd->getName().c_str(),cmd->getDescription().c_str());
        }
        return 0;
    }

    std::string doAutoComplete(std::string arg) override
    {
        // Your implementation here
        return arg;
    }

private:
    std::vector<CliCommand *> *cliCommands;
};

class Command1 : public CliCommand
{
public:
    Command1()
        : CliCommand("command1", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    int doCommand(int argc, std::vector<std::string> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return 0;
    }

    std::string doAutoComplete(std::string arg) override
    {
        // Your implementation here
        return arg;
    }
};

class Command2 : public CliCommand
{
public:
    Command2()
        : CliCommand("command2", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    int doCommand(int argc, std::vector<std::string> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return 0;
    }

    std::string doAutoComplete(std::string arg) override
    {
        // Your implementation here
        return arg;
    }
};

class CommandLine
{
public:
    CommandLine();
    void doCommandLine();
    // static const uint32_t NUMBER_OF_COMMANDS = 6;
    static const uint32_t MAX_COMMAND_LENGTH = 64; // length of serial buffer for incoming commands

private:
    char cmdLine[MAX_COMMAND_LENGTH + 1]; // Read commands into this buffer from Serial.  +1 in length for a termination char
    CommandHelp commandHelp;
    Command1 command1;
    Command2 command2;

    std::vector<CliCommand *> cliCommands;
    const char *delimiters = ", \n";
    bool getCommandLineFromSerialPort(char *commandLine);
    std::string prompt = "cli$ ";

    std::vector<CliCommand *> commandCandidates;
    std::vector<std::string> stringsVector;
};

#endif /* COMMANDLINE_H */