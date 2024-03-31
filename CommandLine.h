#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "Arduino.h"
#include <vector>
#include "Settings.h"

//forward declare helper function
std::string longestCommonPrefix(const std::vector<std::string> &strs);

// Define CliCommand interface
class CliCommand
{
public:
    CliCommand(const std::string &name, const std::string &description)
        : name(name), description(description) {}

    virtual int doCommand(std::vector<std::string> argv) = 0;
    virtual std::vector<std::string> doAutoComplete(std::vector<std::string> argv, std::vector<std::string> complete){
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
    int doCommand(std::vector<std::string> argv) override
    {
        // Your implementation here
        Serial.printf("Available commands are:\n");
        for (const auto cmd : *cliCommands)
        {
            Serial.printf("%15s : %s\n", cmd->getName().c_str(), cmd->getDescription().c_str());
        }
        return 0;
    }
/*
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv) override
    {
        // Your implementation here
        return argv[0];
    }
*/
private:
    std::vector<CliCommand *> *cliCommands;
};

class Command1 : public CliCommand
{
public:
    Command1()
        : CliCommand("command1", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    int doCommand(std::vector<std::string> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return 0;
    }
/*
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv) override
    {
        // Your implementation here
        return argv[0];
    }*/
};

class Command2 : public CliCommand
{
public:
    Command2()
        : CliCommand("command2", "prints its name") {}

    // Implement doCommand and doAutoComplete methods
    int doCommand(std::vector<std::string> argv) override
    {
        Serial.printf("command name : %s\n", name.c_str());
        return 0;
    }
/*
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv) override
    {
        // Your implementation here
        return argv[0];
    }*/
};

class SettingsShow : public CliCommand
{
public:
    SettingsShow(Settings *sett)
        : CliCommand("settings-show", "show all configuration settings"), settings(sett) {}

    int doCommand(std::vector<std::string> argv) override
    {
        Serial.print("GENERAL SYSTEM CONFIGURATION\n\n");
        settings->printSettings();
        return 0;
    }
    /*
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv) override
    {
        // Your implementation here
        return argv[0];
    }*/

private:
    Settings *settings;
};

class SettingsResetDefaultValues : public CliCommand
{
public:
    SettingsResetDefaultValues(Settings *sett)
        : CliCommand("settings-reset-default-values", "Reset all default configuration values"), settings(sett) {}

    int doCommand(std::vector<std::string> argv) override
    {
        settings->reloadDefaultSettings();
        return 0;
    }
    /*
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv) override
    {
        // Your implementation here
        return argv[0];
    }*/

private:
    Settings *settings;
};

class SettingsSet : public CliCommand
{
public:
    SettingsSet(Settings *sett)
        : CliCommand("settings-set", "set a setting. eg.: settings-set aaaa 123"), settings(sett) {}

    int doCommand(std::vector<std::string> argv) override
    {
        //settings->reloadDefaultSettings();
        return 0;
    }
    
    std::vector<std::string> doAutoComplete(std::vector<std::string> argv, std::vector<std::string> complete) override
    {
        // Your implementation here
        complete.clear();
        if (argv.size() == 1){
            //std::vector<Setting*> matchingSettings;
            //std::vector<std::string> stringsVector;
            for (Setting *setting : settings->getSettings()){
                if (strncmp(setting->settingName.c_str(), argv[0].c_str(),argv[0].size()) == 0){
                    //matchingSettings.push_back(setting);
                    complete.push_back(setting->settingName.c_str());
                }
            }
        }
        return complete;
    }

private:
    Settings *settings;
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