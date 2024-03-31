#include "Arduino.h"
#include <vector>
#include <string.h>
#include "CommandLine.h"

std::string prompt = "cli$ ";

// forward declare all command functions
void commandHelp(int argc, char *argv[]);
void commandFunction1(int argc, char *argv[]);
void commandFunction2(int argc, char *argv[]);
void commandPrintArgs(int argc, char *argv[]);

// List of available commands
std::vector<Command> commands = {
    {"help", commandHelp, "this help"},
    {"command1", commandFunction1, "command1 help"},
    {"command2", commandFunction2, "command2 help"},
    {"printArgs", commandPrintArgs, "print arguments passed to a function"}
    // Add more commands here as needed
};

std::vector<Command> commandCandidates;
std::vector<std::string> stringsVector;

// Define your command functions here
void commandHelp(int argc, char *argv[])
{
    // Find and execute the command
    Serial.println("Available commands are: ");
    for (const auto &cmd : commands)
    {
        Serial.printf("%15s :\t%s\n", cmd.command, cmd.help);
    }
}

void commandFunction1(int argc, char *argv[])
{
    Serial.println("Executing commandFunction1");
}

void commandFunction2(int argc, char *argv[])
{
    Serial.println("Executing commandFunction2");
}

void commandPrintArgs(int argc, char *argv[])
{
    Serial.println("Executing commandPrintArgs");
    for (int i = 0; i < argc; i++)
    {
        Serial.printf("argv[%d] = %s\n", i, argv[i]);
    }
}

// helper function
std::string longestCommonPrefix(const std::vector<std::string> &strs)
{
    if (strs.empty())
        return ""; // If the vector is empty, there's no common prefix

    // Iterate through the characters of the first string
    for (int i = 0; i < strs[0].size(); ++i)
    {
        // Check if the character at position i is the same across all strings
        for (int j = 1; j < strs.size(); ++j)
        {
            // If we've reached the end of any string or find a differing character, return the common prefix
            if (i >= strs[j].size() || strs[j][i] != strs[0][i])
            {
                return strs[0].substr(0, i);
            }
        }
    }

    // If we reached this point, the entire first string is the common prefix
    return strs[0];
}

// Function to parse and execute commands
void doCommandLine()
{
    static char buffer[MAX_COMMAND_LENGTH];
    static char *args[MAX_ARGUMENTS];
    static int argc = 0;
    static int buffindex = 0;
    static bool first_prompt = true;

    if (first_prompt)
    {
        Serial.print(prompt);
        first_prompt = false;
    }

    if (Serial.available() > 0)
    {

        char c = Serial.read();

        // if (c == '\n' || c == '\r') {
        if (c == '\r')
        {
            Serial.putc('\n');
            buffer[buffindex] = '\0';
            // Serial.println(buffer);
            argc = 0;
            char *token = strtok(buffer, " ");

            for (int i = 0; i < MAX_ARGUMENTS; i++)
            {
                args[i] = NULL;
            }

            while (token != NULL && argc < MAX_ARGUMENTS)
            {
                args[argc++] = token;
                token = strtok(NULL, " ");
            }

            // Find and execute the command
            for (const auto &cmd : commands)
            {
                if (args[0] == NULL)
                {
                    break;
                }

                if (strcmp(args[0], cmd.command) == 0)
                {
                    cmd.function(argc, args);
                    break;
                }
            }

            // Clear buffer
            memset(buffer, 0, sizeof(buffer));
            buffindex = 0;
            Serial.printf("%s", prompt.c_str());
        }
        else if (c == '\t')
        {
            // tab autocomplete.
            // if a single match is found, autocompletes the command
            // else print candidates
            commandCandidates.clear();
            stringsVector.clear();
            for (const auto &cmd : commands)
            {
                if (strncmp(buffer, cmd.command, buffindex) == 0)
                {
                    commandCandidates.push_back(cmd);
                    stringsVector.push_back(cmd.command);
                }
            }

            // single match is found
            if (commandCandidates.size() == 1)
            {
                // auto complete the command and add a trailing space
                for (; buffindex < strlen(commandCandidates[0].command) && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                {
                    buffer[buffindex] = commandCandidates[0].command[buffindex];
                    Serial.putc(buffer[buffindex]);
                }
                buffer[buffindex++] = ' ';
                Serial.putc(' ');

                // multiple matches
            }
            else if (commandCandidates.size() > 1)
            {
                std::string longestPrefix = longestCommonPrefix(stringsVector);

                // if buffer == longest common string, list all posible commands
                if (strcmp(buffer, longestPrefix.c_str()) == 0)
                {
                    Serial.putc('\n');
                    for (const auto &cmd : commandCandidates)
                    {
                        Serial.printf("%s\n", cmd.command);
                    }
                    Serial.printf("%s%s", prompt.c_str(), buffer);

                    // append longest common string
                }
                else
                {
                    for (; buffindex < longestPrefix.size() && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                    {
                        buffer[buffindex] = longestPrefix[buffindex];
                        Serial.putc(buffer[buffindex]);
                    }
                }
            }

            //} else if (c == '\r') {
            // ignore \r
        }
        else if (buffindex < MAX_COMMAND_LENGTH - 1)
        {
            buffer[buffindex++] = c;
            Serial.putc(c);
        }
    }
}
