#include <vector>
#include "CommandLine.h"
#include "Arduino.h"
#include "StaticString.h"

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

    cmdLine = "";
}

// helper function
void longestCommonPrefix(const StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> strs, StaticString<MAX_COMMAND_ARG_LENGTH> &longestCommonPrefixString)
{
    if (strs.empty())
    {
        longestCommonPrefixString = "";
        return; // If the vector is empty, there's no common prefix
    }

    // Iterate through the characters of the first string
    for (auto i = 0; i < strs[0].length(); ++i)
    {
        // Check if the character at position i is the same across all strings
        for (int j = 1; j < strs.size(); ++j)
        {
            // If we've reached the end of any string or find a differing character, return the common prefix
            bool test = strs[j].getData()[i] != strs[0].getData()[i];
            if (i >= strs[j].length() || test)
            {
                longestCommonPrefixString = strs[0].substring(longestCommonPrefixString, 0, i);
                return;
            }
        }
    }

    // If we reached this point, the entire first string is the common prefix
    longestCommonPrefixString = strs[0];
    return;
}

// Function to parse and execute commands
void CommandLine::doCommandLine()
{
    static StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> args;
    static int argc = 0;
    // static int buffindex = 0;
    static bool first_prompt = true;
    StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> complete;
    Error err;

    if (first_prompt)
    {
        Serial.print(prompt.c_str());
        first_prompt = false;
    }

    if (Serial.available() > 0)
    {
        char c = Serial.read();
        strncpy(shadowCmdLine, cmdLine.c_str(), MAX_COMMAND_LENGTH);

        if (c == '\n' || c == '\r' || c == '\t')
        {
            // if (c == '\r' || c == '\t')

            char *token = strtok(shadowCmdLine, " "); // kills the space at the end.
            args.clear();
            while (token != NULL)
            {
                args.push_back(token);
                token = strtok(NULL, " ");
            }
            // add an empty arg if last character is a space.
            if (cmdLine.c_str()[cmdLine.length() - 1] == ' ')
            {
                args.push_back("");
            }
        }

        // if (c == '\n' || c == '\r') {
        if (c == '\r')
        {
            Serial.putc('\n');
            // Find and execute the command
            for (const auto &cmd : cliCommands)
            {
                if (args.size() == 0)
                {
                    break;
                }

                if (args[0].compare(cmd->getName()) == 0)
                {
                    err = cmd->doCommand(args);
                    if (err != SUCCESS)
                    {
                        printErrorln(err);
                    }
                    break;
                }
            }

            // Clear buffer
            cmdLine = "";
            Serial.printf("%s", prompt.c_str());
        }
        // tab
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
                    if (args.size() == 0 || strncmp(args[0].c_str(), cmd->getName().c_str(), args[0].length()) == 0)
                    {
                        commandCandidates.push_back(cmd);
                        stringsVector.push_back(cmd->getName());
                    }
                }

                // single match is found
                if (commandCandidates.size() == 1)
                {
                    // auto complete the command and add a trailing space
                    StaticString<MAX_COMMAND_ARG_LENGTH> singleCommand(commandCandidates.front()->getName());
                    singleCommand = singleCommand + " ";

                    int arg1len = args[0].length();
                    int arg2len = singleCommand.length();

                    StaticString<MAX_COMMAND_ARG_LENGTH> str2append;
                    singleCommand.substring(str2append, arg1len, arg2len - arg1len);

                    cmdLine += str2append.c_str();
                    Serial.printf("%s", str2append.c_str());

                } // multiple matches
                else if (commandCandidates.size() > 1)
                {
                    StaticString<MAX_COMMAND_ARG_LENGTH> longestPrefix = "";
                    longestCommonPrefix(stringsVector, longestPrefix);

                    // if buffer == longest common string, list all posible commands
                    if (strcmp(cmdLine.c_str(), longestPrefix.c_str()) == 0)
                    {
                        Serial.putc('\n');
                        for (const auto cmd : commandCandidates)
                        {
                            Serial.printf("%s\n", cmd->getName().c_str());
                        }
                        Serial.printf("%s%s", prompt.c_str(), cmdLine.c_str());

                    } // append longest common string
                    else
                    {

                        int arg1len = args[0].length();
                        int arg2len = longestPrefix.length();

                        StaticString<MAX_COMMAND_ARG_LENGTH> str2append;
                        longestPrefix.substring(str2append, arg1len, arg2len - arg1len);

                        cmdLine += str2append.c_str();
                        Serial.printf("%s", str2append.c_str());
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
                        complete.clear();
                        // command found call its autocomplete function to get candidates.
                        for (int a = 1; a < args.size(); a++)
                        {
                            stringsVector.push_back(args[a]);
                        }
                        complete = cmd->doAutoComplete(stringsVector, complete);

                        // if only 1 match autocomplete append to last argument
                        if (complete.size() == 1)
                        {
                            // auto complete the arg and add a trailing space
                            StaticString<MAX_COMMAND_ARG_LENGTH> singleArg = complete[0] + " ";

                            int arg1len = stringsVector.back().length();
                            int arg2len = singleArg.length();

                            StaticString<MAX_COMMAND_ARG_LENGTH> str2append;
                            singleArg.substring(str2append, arg1len, arg2len - arg1len);

                            cmdLine += str2append.c_str();
                            Serial.printf("%s", str2append.c_str());
                        }
                        else
                        {
                            StaticString<MAX_COMMAND_ARG_LENGTH> longestPrefix;
                            longestCommonPrefix(complete, longestPrefix);

                            // if last arg == longest common string, list all posible commands
                            if (stringsVector.back().compare(longestPrefix) == 0)
                            {
                                Serial.putc('\n');
                                for (const auto a : complete)
                                {
                                    Serial.printf("%s\n", a.c_str());
                                }
                                Serial.printf("%s%s", prompt.c_str(), cmdLine.c_str());

                            } // append longest common string
                            else
                            {

                                int arg1len = stringsVector.back().length();
                                int arg2len = longestPrefix.length();

                                StaticString<MAX_COMMAND_ARG_LENGTH> str2append;
                                longestPrefix.substring(str2append, arg1len, arg2len - arg1len);

                                cmdLine += str2append.c_str();
                                Serial.printf("%s", str2append.c_str());
                            }
                        }
                    }
                }
            }
        }
        // backspace
        else if (c == '\x08')
        {
            cmdLine.removeCharAt(cmdLine.length() - 1);
            // cmdLine[--buffindex] = '\x00';
            Serial.putc(c);
            Serial.putc(' ');
            Serial.putc(c);
        }
        // ignore other control characters
        else if (c < 32)
        {
            // ignore
        }
        // normal character
        else if (cmdLine.length() < MAX_COMMAND_LENGTH - 1)
        {
            cmdLine += c;
            // cmdLine[buffindex++] = c;
            Serial.putc(c);
        }
    }
}
