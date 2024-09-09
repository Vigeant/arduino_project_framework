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

  //spirulina control settings
  SettingImpl<uint32_t> grow_2_rest_time_setpoint;
  SettingImpl<uint32_t> rest_2_grow_time_setpoint;
  SettingImpl<uint32_t> grow_temp_setpoint;
  SettingImpl<uint32_t> rest_temp_setpoint;
  SettingImpl<uint32_t> temp_hysteresis;
  SettingImpl<uint32_t> grow_light_intensity_setpoint;
  SettingImpl<uint32_t> rest_light_intensity_setpoint;
  SettingImpl<uint32_t> light_hysteresis;
  SettingImpl<uint32_t> silence_mode;
  SettingImpl<uint32_t> manual_mode;
  SettingImpl<uint32_t> tube0_auto_harvest;
  SettingImpl<uint32_t> tube1_auto_harvest;
  SettingImpl<uint32_t> tube2_auto_harvest;

  //spirulina output settings
  SettingImpl<uint32_t> led_dimmer_pwm;
  SettingImpl<uint32_t> heater;
  SettingImpl<uint32_t> return_pump;
  SettingImpl<uint32_t> air_pump;
  SettingImpl<uint32_t> tube0_harvest_valve;
  SettingImpl<uint32_t> tube1_harvest_valve;
  SettingImpl<uint32_t> tube2_harvest_valve;
  SettingImpl<uint32_t> tube0_return_valve;
  SettingImpl<uint32_t> tube1_return_valve;
  SettingImpl<uint32_t> tube2_return_valve;
  SettingImpl<uint32_t> tube0_harvest_ready;
  SettingImpl<uint32_t> tube1_harvest_ready;
  SettingImpl<uint32_t> tube2_harvest_ready;

  //spirulina calibration settings
  SettingImpl<uint32_t> tube0_ph_calibration_factor_m;
  SettingImpl<uint32_t> tube0_ph_calibration_factor_b;
  SettingImpl<uint32_t> tube1_ph_calibration_factor_m;
  SettingImpl<uint32_t> tube1_ph_calibration_factor_b;
  SettingImpl<uint32_t> tube2_ph_calibration_factor_m;
  SettingImpl<uint32_t> tube2_ph_calibration_factor_b;
  SettingImpl<uint32_t> tube0_temp_calibration_factor_m;
  SettingImpl<uint32_t> tube0_temp_calibration_factor_b;
  SettingImpl<uint32_t> tube1_temp_calibration_factor_m;
  SettingImpl<uint32_t> tube1_temp_calibration_factor_b;
  SettingImpl<uint32_t> tube2_temp_calibration_factor_m;
  SettingImpl<uint32_t> tube2_temp_calibration_factor_b;
  SettingImpl<uint32_t> tube0_turbitity_calibration_factor_m;
  SettingImpl<uint32_t> tube0_turbitity_calibration_factor_b;
  SettingImpl<uint32_t> tube1_turbitity_calibration_factor_m;
  SettingImpl<uint32_t> tube1_turbitity_calibration_factor_b;
  SettingImpl<uint32_t> tube2_turbitity_calibration_factor_m;
  SettingImpl<uint32_t> tube2_turbitity_calibration_factor_b;
  SettingImpl<uint32_t> tube0_recovery_bin_load_calibration_factor_m;
  SettingImpl<uint32_t> tube0_recovery_bin_load_calibration_factor_b;

  //spirulina sensor readings
  SettingImpl<float> tube0_ph;
  SettingImpl<float> tube1_ph;
  SettingImpl<float> tube2_ph;
  SettingImpl<float> tube0_temp;
  SettingImpl<float> tube1_temp;
  SettingImpl<float> tube2_temp;
  SettingImpl<uint32_t> tube0_level_full;
  SettingImpl<uint32_t> tube1_level_full;
  SettingImpl<uint32_t> tube2_level_full;
  SettingImpl<uint32_t> light_intensity;
  SettingImpl<uint32_t> tube0_turbitity;
  SettingImpl<uint32_t> tube1_turbitity;
  SettingImpl<uint32_t> tube2_turbitity;
  SettingImpl<uint32_t> recovery_bin_load;
  SettingImpl<uint32_t> recovery_drawer_interlock;



private:
  StaticVector<Setting*,MAX_SETTINGS> settings;
};

extern Settings settings;

#endif /* SETTINGS_H */