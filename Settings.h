#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include "StaticVector.h"
#include "StaticString.h"
#include "Errors.h"
#include <string>

#define EEPROM_VERSION 123

#define MAX_SETTINGS 50
#define MAX_SETTING_ARG_LENGTH 50
#define MAX_SETTING_NAME_LENGTH 50
#define MAX_SETTING_DESCRIPTION_LENGTH 100

class Setting {
public:
  Setting(const StaticString<MAX_SETTING_NAME_LENGTH> settingName, bool editable, const StaticString<MAX_SETTING_DESCRIPTION_LENGTH> description)
    : settingName(settingName),
      editable(editable),
      description(description) {
    ;
  }
  const StaticString<MAX_SETTING_NAME_LENGTH> settingName;
  virtual void prettyPrint() = 0;
  virtual Error setVal(StaticString<MAX_SETTING_ARG_LENGTH> valStr) = 0;
  virtual uint16_t getSize() = 0;
  virtual void resetDefault() = 0;
  virtual uint32_t saveToEEPROM(uint32_t address) = 0;
  virtual uint32_t loadFromEEPROM(uint32_t address) = 0;
protected:
  bool editable = true;
  const StaticString<MAX_SETTING_DESCRIPTION_LENGTH> description;
};

template<class C> class SettingImpl : public Setting {
public:
  SettingImpl(const char* paramName, bool editable, C value, C valueDefault, C valueMin, C valueMax, const char* description)
    : Setting(paramName, editable, description),
      value(value),
      valueDefault(valueDefault),
      valueMin(valueMin),
      valueMax(valueMax) {
    ;
  }

  void prettyPrint();
  Error setVal(StaticString<MAX_SETTING_ARG_LENGTH> valStr);

  C getVal() {
    return value;
  }

  void resetDefault() {
    value = valueDefault;
  }

  uint16_t getSize();
  bool valueMatchDefault() {
    return (value == valueDefault);
  }

  uint32_t saveToEEPROM(uint32_t address) {
    EEPROM.put(address, value);
    return sizeof(C);
  }

  uint32_t loadFromEEPROM(uint32_t address) {
    EEPROM.get(address, value);
    return sizeof(C);
  }

private:
  C value;
  C valueDefault;
  C valueMin;
  C valueMax;
};

class Settings {
public:
  void printSettings();
  Settings();
  Setting* getSetting(StaticString<MAX_SETTING_NAME_LENGTH> name);
  StaticVector<Setting*,MAX_SETTINGS> getSettings();
  uint16_t size();
  void reloadDefaultSettings();
  void saveAllSettingsToEEPROM(uint32_t address);
  void loadAllSettingsFromEEPROM(uint32_t address);

  SettingImpl<uint32_t> magic_bytes;
  SettingImpl<uint32_t> eeprom_version;
  SettingImpl<uint32_t> canbus_speed;
  SettingImpl<uint32_t> canbus_to_EVCC;
  SettingImpl<float> over_v_setpoint;
  SettingImpl<float> under_v_setpoint;
  SettingImpl<float> max_charge_v_setpoint;
  SettingImpl<float> charger_cycle_v_setpoint;
  SettingImpl<float> top_balance_v_setpoint;
  SettingImpl<float> warn_cell_v_offset;
  SettingImpl<float> floor_duty_coolant_pump;
  SettingImpl<float> cooling_lowt_setpoint;
  SettingImpl<float> cooling_hight_setpoint;
  SettingImpl<float> over_t_setpoint;
  SettingImpl<float> under_t_setpoint;
  SettingImpl<float> warn_t_offset;
  SettingImpl<float> precision_balance_v_setpoint;
  SettingImpl<float> precision_balance_cell_v_offset;
  SettingImpl<float> rough_balance_v_setpoint;
  SettingImpl<float> rough_balance_cell_v_offset;
  SettingImpl<float> dc2dc_cycle_v_setpoint;
  SettingImpl<uint32_t> dc2dc_cycle_time_s;
  SettingImpl<float> bat12v_over_v_setpoint;
  SettingImpl<float> bat12v_under_v_setpoint;
  SettingImpl<float> bat12v_scaling_divisor;
  SettingImpl<uint32_t> fault_debounce_count;
  SettingImpl<uint32_t> module_count;
  SettingImpl<uint32_t> oled_cycle_time;
  SettingImpl<uint32_t> time_before_first_sleep;

private:
  StaticVector<Setting*,MAX_SETTINGS> settings;
};

extern Settings settings;

#endif /* SETTINGS_H */