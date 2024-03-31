#include "Arduino.h"
#include <vector>
#include <string.h>
#include "CommandLine.h"

CommandLine::CommandLine()
    : commandHelp(&cliCommands),
      command1(),
      command2(),
      showSettings(&settings),
      settingsResetDefaultValues(&settings),
      settingsSet(&settings)
{

    cliCommands.clear();
    cliCommands.push_back(&commandHelp);
    cliCommands.push_back(&command1);
    cliCommands.push_back(&command2);
    cliCommands.push_back(&showSettings);
    cliCommands.push_back(&settingsResetDefaultValues);
    cliCommands.push_back(&settingsSet);

    memset(cmdLine, 0, sizeof(cmdLine));
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
void CommandLine::doCommandLine()
{
    static std::vector<std::string> args;
    static int argc = 0;
    static int buffindex = 0;
    static bool first_prompt = true;
    std::vector<std::string> complete;

    if (first_prompt)
    {
        Serial.print(prompt);
        first_prompt = false;
    }

    if (Serial.available() > 0)
    {

        char c = Serial.read();

        if (c == '\r' || c == '\t')
        {
            char *token = strtok(cmdLine, " ");
            args.clear();
            while (token != NULL)
            {
                args.push_back(token);
                token = strtok(NULL, " ");
            }
        }

        // if (c == '\n' || c == '\r') {
        if (c == '\r')
        {
            Serial.putc('\n');
            cmdLine[buffindex] = '\0';
            /*char *token = strtok(cmdLine, " ");

            //args.clear();

            while (token != NULL)
            {
                args.push_back(token);
                token = strtok(NULL, " ");
            }*/

            // Find and execute the command
            for (const auto &cmd : cliCommands)
            {
                if (args.size() == 0)
                {
                    break;
                }

                if (args[0].compare(cmd->getName()) == 0)
                {
                    cmd->doCommand(args);
                    break;
                }
            }

            // Clear buffer
            memset(cmdLine, 0, sizeof(cmdLine));
            buffindex = 0;
            Serial.printf("%s", prompt.c_str());
        }
        else if (c == '\t')
        {
            // tab autocomplete.
            // if a single match is found, autocompletes the command
            // else print candidates

            // if still working on initial command
            if (args.size() <= 1)
            {

                commandCandidates.clear();
                stringsVector.clear();
                for (const auto cmd : cliCommands)
                {
                    if (strncmp(cmdLine, cmd->getName().c_str(), buffindex) == 0)
                    {
                        commandCandidates.push_back(cmd);
                        stringsVector.push_back(cmd->getName());
                    }
                } // single match is found
                if (commandCandidates.size() == 1)
                {
                    // auto complete the command and add a trailing space
                    for (; buffindex < commandCandidates[0]->getName().length() && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                    {
                        cmdLine[buffindex] = commandCandidates[0]->getName()[buffindex];
                        Serial.putc(cmdLine[buffindex]);
                    }
                    cmdLine[buffindex++] = ' ';
                    Serial.putc(' ');

                } // multiple matches
                else if (commandCandidates.size() > 1)
                {
                    std::string longestPrefix = longestCommonPrefix(stringsVector);

                    // if buffer == longest common string, list all posible commands
                    if (strcmp(cmdLine, longestPrefix.c_str()) == 0)
                    {
                        Serial.putc('\n');
                        for (const auto cmd : commandCandidates)
                        {
                            Serial.printf("%s\n", cmd->getName().c_str());
                        }
                        Serial.printf("%s%s", prompt.c_str(), cmdLine);

                    } // append longest common string
                    else
                    {
                        for (; buffindex < longestPrefix.size() && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                        {
                            cmdLine[buffindex] = longestPrefix[buffindex];
                            Serial.putc(cmdLine[buffindex]);
                        }
                    }
                }
            }
            else if (args.size() > 1)
            {
                // working on arguments here
                //  find command if it exists
                for (const auto cmd : cliCommands)
                {
                    if (args[0].compare(cmd->getName()) == 0)
                    {
                        stringsVector.clear();
                        // command found call its autocomplete function to get candidates.
                        for (int a = 1; a < args.size(); a++)
                        {
                            stringsVector.push_back(args[a]);
                        }
                        complete = cmd->doAutoComplete(stringsVector, complete);

                        // if only 1 match autocomplete append to last argument
                        if (complete.size() == 1)
                        {
                            // stringsVector.back().append(complete[0].substr(stringsVector.back().length(), complete[0].length() - stringsVector.back().length()));
                            //  auto complete the command and add a trailing space

                            for (int maxlen = buffindex + complete[0].length() - stringsVector.back().length(); buffindex < maxlen && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                            {
                                cmdLine[buffindex] = complete[0].c_str()[buffindex];
                                Serial.putc(cmdLine[buffindex]);
                            }
                        }
                        else
                        {
                            std::string longestPrefix = longestCommonPrefix(complete);

                            // if buffer == longest common string, list all posible commands
                            if (strcmp(stringsVector.back().c_str(), longestPrefix.c_str()) == 0)
                            {
                                Serial.putc('\n');
                                for (const auto a : complete)
                                {
                                    Serial.printf("%s\n", a.c_str());
                                }
                                Serial.printf("%s%s", prompt.c_str(), cmdLine);

                            } // append longest common string
                            else
                            {
                            for (int maxlen = buffindex + longestPrefix.length(); buffindex < maxlen && buffindex < MAX_COMMAND_LENGTH - 1; buffindex++)
                            {
                                cmdLine[buffindex] = longestPrefix.c_str()[buffindex];
                                Serial.putc(cmdLine[buffindex]);
                            }
                            }
                        }
                    }
                }
            }

            //} else if (c == '\r') {
            // ignore \r
        }
        else if (buffindex < MAX_COMMAND_LENGTH - 1)
        {
            cmdLine[buffindex++] = c;
            Serial.putc(c);
        }
    }
}
