#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "Arduino.h"
#include <vector>
#include "Settings.h"
#include "Errors.h"
#include <string>

// forward declare helper function
std::string longestCommonPrefix(const std::vector<std::string> &strs);

// Define CliCommand interface
class CliCommand
{
public:
    CliCommand(const std::string &name, const std::string &description)
        : name(name), description(description) {}

    virtual Error doCommand(std::vector<std::string> argv)
    {
        return SUCCESS;
    }
    virtual std::vector<std::string> doAutoComplete(std::vector<std::string> argv, std::vector<std::string> complete)
    {
        return complete;
    }

    const std::string &getName() const { return name; }
    const std::string &getDescription() const { return description; }

protected:
    std::string name;
    std::string description;
};

// declare commands
//  Define CommandHelp class
class CommandHelp : public CliCommand
{
public:
    CommandHelp(std::vector<CliCommand *> *cliCommands)
        : CliCommand("help", "show this help"), cliCommands(cliCommands) {}

    // Implement doCommand and doAutoComplete methods
    Error doCommand(std::vector<std::string> argv) override
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
    std::vector<CliCommand *> *cliCommands;
};

class Command1 : public CliCommand
{
public:
    Command1()
        : CliCommand("command1", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    Error doCommand(std::vector<std::string> argv) override
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
    Error doCommand(std::vector<std::string> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return SUCCESS;
    }
};

class SettingsCommand : public CliCommand
{
public:
    SettingsCommand(std::string name, std::string desc, Settings *sett)
        : CliCommand(name, desc), settings(sett) {}

    virtual std::vector<std::string> doAutoComplete(std::vector<std::string> argv, std::vector<std::string> complete) override
    {
        // Your implementation here
        complete.clear();
        if (argv.size() > 0)
        {
            // std::vector<Setting*> matchingSettings;
            // std::vector<std::string> stringsVector;
            for (Setting *setting : settings->getSettings())
            {
                if (strncmp(setting->settingName.c_str(), argv.back().c_str(), argv.back().size()) == 0)
                {
                    // matchingSettings.push_back(setting);
                    complete.push_back(setting->settingName.c_str());
                }
            }
        }
        return complete;
    }

protected:
    Settings *settings;
};

class SettingsShow : public SettingsCommand
{
public:
    SettingsShow(Settings *sett)
        : SettingsCommand("settings-show", "show all configuration settings", sett) {}

    Error doCommand(std::vector<std::string> argv) override
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
            argv.erase(argv.begin());
            for (std::string arg : argv)
            {
                auto param = settings->getSetting(arg.c_str());
                if (param == 0)
                {
                    if (arg.compare("") != 0)
                    {
                        printError(SETTING_NOT_FOUND);
                        Serial.printf(" (%s)\n", arg.c_str());
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

    Error doCommand(std::vector<std::string> argv) override
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

    Error doCommand(std::vector<std::string> argv) override
    {
        if (argv.size() < 2)
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
    void doCommandLine();
    static const uint32_t MAX_COMMAND_LENGTH = 128; // length of serial buffer for incoming commands

private:
    char cmdLine[MAX_COMMAND_LENGTH + 1];       // Read commands into this buffer from Serial.  +1 in length for a termination char
    char shadowCmdLine[MAX_COMMAND_LENGTH + 1]; // because strtok modifies the string...
    CommandHelp commandHelp;
    Command1 command1;
    Command2 command2;
    SettingsShow showSettings;
    SettingsResetDefaultValues settingsResetDefaultValues;
    SettingsSet settingsSet;

    std::vector<CliCommand *> cliCommands;
    const char *delimiters = ", \n";
    bool getCommandLineFromSerialPort(char *commandLine);
    std::string prompt = "cli$ ";

    std::vector<CliCommand *> commandCandidates;
    std::vector<std::string> stringsVector;
};

#endif /* COMMANDLINE_H */