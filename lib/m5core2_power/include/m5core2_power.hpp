// copyright (c) 2022 by honey the codewitch
// derived from M5Stack code:
/*
MIT License

Copyright (c) 2020 M5Stack

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <Wire.h>
#include <Arduino.h>

#define SLEEP_MSEC(us) (((uint64_t)us) * 1000L)
#define SLEEP_SEC(us) (((uint64_t)us) * 1000000L)
#define SLEEP_MIN(us) (((uint64_t)us) * 60L * 1000000L)
#define SLEEP_HR(us) (((uint64_t)us) * 60L * 60L * 1000000L)

#define AXP_ADDR 0X34

#define PowerOff(x) SetSleep(x)

class m5core2_power {
 public:
  enum struct m5core2_charge_current {
      current_100mA = 0,
      current_190mA,
      current_280mA,
      current_360mA,
      current_450mA,
      current_550mA,
      current_630mA,
      current_700mA,
      current_780mA,
      current_880mA,
      current_960mA,
      current_1000mA,
      current_1080mA,
      current_1160mA,
      current_1240mA,
      current_1320mA,
  };

  m5core2_power();
  void initialize();
  void lcd_dim(float brightness);
  bool battery_state();

  void coulomb_counter_enable(bool value);
  void coulomb_counter_stop(void);
  void coulomb_counter_clear(void);
  uint32_t coulomb_charge(void);
  uint32_t coulomb_discharge(void);
  float coulomb(void);
  float battery_level(void);
  void power_off(void);
  void adc_enable(bool state);
  // -- sleep
  void prepare_sleep(void);
  void restore_from_light_sleep(void);
  void deep_sleep(uint64_t time_in_us = 0);
  void light_sleep(uint64_t time_in_us = 0);
  uint8_t warning_level_raw(void);

  float battery_voltage();
  float battery_current();
  float vin_voltage();
  float vin_current();
  float vbus_voltage();
  float vbus_current();
  float temperature();
  float battery_power();
  float battery_charge_current();
  float aps_voltage();
  float battery_coulomb_input();
  float battery_coulomb_output();
  uint8_t warning_level(void);
  void set_coulomb_clear();
  void ldo2_enable(bool State);
  void dcd3_enable(bool State);

  uint8_t state();
  bool ac_in();
  bool charging();
  bool vbus();

  void ldo_voltage(uint8_t number, float voltage);
  void dc_voltage(uint8_t number, float voltage);
  void mcu_voltage(float voltage);
  void lcd_voltage(float voltage);
  void ldo_enable(uint8_t number, bool value);
  void lcd_reset_enable(bool value);
  void bus_external_power_enable(bool value);
  void power_led_enable(bool value);
  void speaker_enable(bool value);
  void charge_current(m5core2_charge_current value);
  void set_peripheral_power(bool value);

 private:
  void Write1Byte(uint8_t Addr, uint8_t Data);
  uint8_t Read8bit(uint8_t Addr);
  uint16_t Read12Bit(uint8_t Addr);
  uint16_t Read13Bit(uint8_t Addr);
  uint16_t Read16bit(uint8_t Addr);
  uint32_t Read24bit(uint8_t Addr);
  uint32_t Read32bit(uint8_t Addr);
  void ReadBuff(uint8_t Addr, uint8_t Size, uint8_t *Buff);
};
