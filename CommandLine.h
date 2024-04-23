#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "Arduino.h"
#include "StaticVector.h"
#include "StaticString.h"
#include "Settings.h"
#include "Errors.h"
#include <string>

#define MAX_COMMAND_LENGTH 256
#define MAX_COMMANDS 50
#define MAX_COMMAND_ARGS MAX_SETTING_NAME_LENGTH
#define MAX_COMMAND_ARG_LENGTH MAX_SETTING_ARG_LENGTH
#define MAX_COMMAND_NAME_LENGTH MAX_SETTING_NAME_LENGTH
#define MAX_COMMAND_DESCRIPTION_LENGTH 100
#define MAX_COMMAND_PROMPT_LENGTH MAX_COMMAND_ARGS*(MAX_COMMAND_ARG_LENGTH+2)

// forward declare helper function
StaticString<MAX_COMMAND_ARG_LENGTH> longestCommonPrefix(const StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS>& strs);

// Define CliCommand interface
class CliCommand
{
public:
    CliCommand(StaticString<MAX_COMMAND_NAME_LENGTH> name, StaticString<MAX_COMMAND_DESCRIPTION_LENGTH> description)
        : name(name), description(description) {}
    virtual Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv)
    {
        return SUCCESS;
    }
    StaticString<MAX_COMMAND_NAME_LENGTH> getName() const
    {
        return name;
    }
    StaticString<MAX_COMMAND_DESCRIPTION_LENGTH> getDescription() const
    {
        return description;
    }
    virtual StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>,MAX_COMMAND_ARGS> doAutoComplete(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv, StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> complete)
    {
        return complete;
    }


protected:
    StaticString<MAX_COMMAND_NAME_LENGTH> name;
    StaticString<MAX_COMMAND_DESCRIPTION_LENGTH> description;
};

// declare commands
//  Define CommandHelp class
class CommandHelp : public CliCommand
{
public:
    CommandHelp(StaticVector<CliCommand *,MAX_COMMANDS> *cliCommands)
        : CliCommand(StaticString<MAX_COMMAND_NAME_LENGTH>("help"), StaticString<MAX_COMMAND_DESCRIPTION_LENGTH>("show this help")), cliCommands(cliCommands) {}

    // Implement doCommand and doAutoComplete methods
    Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        // Your implementation here
        Serial.printf("Available commands are:\n");
        for (const auto cmd : *cliCommands)
        {
            Serial.printf("%15s : %s\n", cmd->getName().c_str(), cmd->getDescription().c_str());
        }
        return SUCCESS;
    }
private:
    StaticVector<CliCommand *,MAX_COMMANDS> *cliCommands;
};

class Command1 : public CliCommand
{
public:
    Command1()
        : CliCommand("command1", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return SUCCESS;
    }
};

class Command2 : public CliCommand
{
public:
    Command2()
        : CliCommand("command2", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return SUCCESS;
    }
};

class SettingsCommand : public CliCommand
{
public:
    protected:
        Settings *settings;

    public:
        SettingsCommand(StaticString<MAX_COMMAND_NAME_LENGTH> name, StaticString<MAX_COMMAND_DESCRIPTION_LENGTH> desc, Settings *sett)
            : CliCommand(name, desc), settings(sett) {}

    virtual StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> doAutoComplete(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv, StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> complete) override
    {
        // Your implementation here
        complete.clear();
        if (argv.size() > 0)
        {
            // StaticVector<Setting*> matchingSettings;
            // StaticVector<StaticString<25>> stringsVector;
            for (Setting *setting : settings->getSettings())
            {
                if (strncmp(setting->settingName.c_str(), argv.back().c_str(), argv.back().length()) == 0)
                {
                    // matchingSettings.push_back(setting);
                    complete.push_back(setting->settingName.c_str());
                }
            }
        }
        return complete;
    }
};

class SettingsShow : public SettingsCommand
{
public:
    SettingsShow(Settings *sett)
        : SettingsCommand("settings-show", "show all configuration settings", sett) {}

    Error doCommand(const StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        Error err;
        if (argv.size() == 1 || (argv.size() == 2 && argv[1].compare("") == 0))
        {
            Serial.print("GENERAL SYSTEM CONFIGURATION\n\n");
            settings->printSettings();
            return SUCCESS;
        }
        else
        {
            err = SETTING_NOT_FOUND;
            //argv.erase(argv.begin());
            for (auto arg = argv.begin() + 1; arg != argv.end(); ++arg)
            {
                auto param = settings->getSetting(arg->c_str());
                if (param == 0)
                {
                    if (arg->compare("") != 0)
                    {
                        printError(SETTING_NOT_FOUND);
                        Serial.printf(" (%s)\n", arg->c_str());
                    }
                }
                else
                {
                    err = SUCCESS;
                    param->prettyPrint();
                }
            }
            return err;
        }
    }
};

class SettingsResetDefaultValues : public SettingsCommand
{
public:
    SettingsResetDefaultValues(Settings *sett)
        : SettingsCommand("settings-reset-default-values", "Reset all default configuration values", sett) {}

    Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        settings->reloadDefaultSettings();
        return SUCCESS;
    }
};

class SettingsSet : public SettingsCommand
{
public:
    SettingsSet(Settings *sett)
        : SettingsCommand("settings-set", "set a setting. eg.: settings-set aaaa 123", sett) {}

    Error doCommand(StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>, MAX_COMMAND_ARGS> argv) override
    {
        if (argv.size() < 3)
        {
            return TOO_FEW_ARGUMENTS;
        }
        else if (argv.size() == 3 || (argv.size() == 4 && argv.back().compare("") == 0))
        {
            auto param = settings->getSetting(argv[1].c_str());
            if (param == 0)
            {
                return SETTING_NOT_FOUND;
            }
            else
            {
                return param->setVal(argv[2]);
            }
        }
        else
        {
            return TOO_MANY_ARGUMENTS;
        }
    }
};

class CommandLine
{
public:
    CommandLine();
    void doCommandLine(const char c);
    void init();
    //static const uint32_t MAX_COMMAND_LENGTH = 128; // length of serial buffer for incoming commands

private:
    StaticString<MAX_COMMAND_LENGTH + 1> cmdLine;       // Read commands into this buffer from Serial.  +1 in length for a termination char
    char shadowCmdLine[MAX_COMMAND_LENGTH + 1]; // Copy of cmdLine for parsing
    //StaticString<MAX_COMMAND_LENGTH + 1> shadowCmdLine; // Copy of cmdLine for parsing
    CommandHelp commandHelp;
    Command1 command1;
    Command2 command2;
    SettingsShow showSettings;
    SettingsResetDefaultValues settingsResetDefaultValues;
    SettingsSet settingsSet;

    StaticVector<CliCommand *,MAX_COMMANDS> cliCommands;
    StaticString<10> delimiters = ", \n";
    bool getCommandLineFromSerialPort(char *commandLine);
    StaticString<10> prompt = "cli$ ";

    StaticVector<CliCommand *,MAX_COMMANDS> commandCandidates;
    StaticVector<StaticString<MAX_COMMAND_ARG_LENGTH>,MAX_COMMAND_ARGS> stringsVector;
};

#endif /* COMMANDLINE_H */