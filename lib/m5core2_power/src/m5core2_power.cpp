#include <m5core2_power.hpp>

static uint8_t calc_voltage_data(uint16_t value, uint16_t maxv, uint16_t minv,
                        uint16_t step) {
  uint8_t data = 0;
  if (value > maxv) value = maxv;
  if (value > minv) data = (value - minv) / step;
  return data;
}
m5core2_power::m5core2_power() {}

void m5core2_power::initialize() {
  Wire1.begin(21,22);
  Wire1.setClock(400000);

  //m5core2_power 30H
  Write1Byte(0x30, (Read8bit(0x30) & 0x04) | 0X02);
  
  //m5core2_power GPIO1:OD OUTPUT
  Write1Byte(0x92, Read8bit(0x92) & 0xf8);
  
  //m5core2_power GPIO2:OD OUTPUT
  Write1Byte(0x93, Read8bit(0x93) & 0xf8);
  
  //m5core2_power RTC CHG
  Write1Byte(0x35, (Read8bit(0x35) & 0x1c) | 0xa2);
  
  mcu_voltage(3.350);
  
  lcd_voltage(2.800);
  
  ldo_voltage(2, 3.300);  //Periph power voltage preset (LCD_logic, SD card)
  
  ldo_voltage(3, 2.000);  //Vibrator power voltage preset
  
  ldo_enable(2, true);
  dcd3_enable(true);  // LCD backlight
  power_led_enable(true);

  charge_current(m5core2_charge_current::current_100mA);
  //SetAxpPriphPower(1);
  //Serial.printf("axp: lcd_logic and sdcard power enabled\n\n");

  //pinMode(39, INPUT_PULLUP);

  //m5core2_power GPIO4
  Write1Byte(0X95, (Read8bit(0x95) & 0x72) | 0X84);

  Write1Byte(0X36, 0X4C);

  Write1Byte(0x82, 0xff);

  lcd_reset_enable(0);
  delay(100);
  lcd_reset_enable(1);
  delay(100);
  // I2C_WriteByteDataAt(0X15,0XFE,0XFF);
  // set peripherals power on
  set_peripheral_power(true);
  
  // axp: check v-bus status
  if (Read8bit(0x00) & 0x08) {
    Write1Byte(0x30, Read8bit(0x30) | 0x80);
    // if v-bus can use, disable M-Bus 5V output to input
    bus_external_power_enable(1);
  } else {
    // if not, enable M-Bus 5V output
    bus_external_power_enable(0);
  }
  //speaker_enable(true);
}
void m5core2_power::set_peripheral_power(bool value) {
  if (!value)
    Write1Byte(0x10, Read8bit(0x10) & 0XFB);
  else if (value)
    Write1Byte(0x10, Read8bit(0x10) | 0X04);
  // uint8_t data;
  // Set EXTEN to enable 5v boost
}
void m5core2_power::Write1Byte(uint8_t Addr, uint8_t Data) {
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.write(Data);
  Wire1.endTransmission();
}

uint8_t m5core2_power::Read8bit(uint8_t Addr) {
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  return Wire1.read();
}

uint16_t m5core2_power::Read12Bit(uint8_t Addr) {
  uint16_t Data = 0;
  uint8_t buf[2];
  ReadBuff(Addr, 2, buf);
  Data = ((buf[0] << 4) + buf[1]);  //
  return Data;
}

uint16_t m5core2_power::Read13Bit(uint8_t Addr) {
  uint16_t Data = 0;
  uint8_t buf[2];
  ReadBuff(Addr, 2, buf);
  Data = ((buf[0] << 5) + buf[1]);  //
  return Data;
}

uint16_t m5core2_power::Read16bit(uint8_t Addr) {
  uint16_t ReData = 0;
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 2);
  for (int i = 0; i < 2; i++) {
    ReData <<= 8;
    ReData |= Wire1.read();
  }
  return ReData;
}

uint32_t m5core2_power::Read24bit(uint8_t Addr) {
  uint32_t ReData = 0;
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 3);
  for (int i = 0; i < 3; i++) {
    ReData <<= 8;
    ReData |= Wire1.read();
  }
  return ReData;
}

uint32_t m5core2_power::Read32bit(uint8_t Addr) {
  uint32_t ReData = 0;
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 4);
  for (int i = 0; i < 4; i++) {
    ReData <<= 8;
    ReData |= Wire1.read();
  }
  return ReData;
}

void m5core2_power::ReadBuff(uint8_t Addr, uint8_t Size, uint8_t *Buff) {
  Wire1.beginTransmission(0x34);
  Wire1.write(Addr);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, (int)Size);
  for (int i = 0; i < Size; i++) {
    *(Buff + i) = Wire1.read();
  }
}

void m5core2_power::lcd_dim(float value) {
  uint8_t brightness=value*12;
  if (brightness > 12) {
    brightness = 12;
  }
  uint8_t buf = Read8bit(0x28);
  Write1Byte(0x28, ((buf & 0x0f) | (brightness << 4)));
}

bool m5core2_power::battery_state() {
  if (Read8bit(0x01) | 0x20)
    return true;
  else
    return false;
}
//---------coulombcounter_from_here---------
//enable: void EnableCoulombcounter(void);
//disable: void DisableCOulombcounter(void);
//stop: void StopCoulombcounter(void);
//clear: void ClearCoulombcounter(void);
//get charge data: uint32_t GetCoulombchargeData(void);
//get discharge data: uint32_t GetCoulombdischargeData(void);
//get coulomb val affter calculation: float GetCoulombData(void);
//------------------------------------------
void m5core2_power::coulomb_counter_enable(bool value) { 
  Write1Byte(0xB8, value?0x80:0x00); 
}

void m5core2_power::coulomb_counter_stop(void) { Write1Byte(0xB8, 0xC0); }

void m5core2_power::coulomb_counter_clear(void) { Write1Byte(0xB8, 0xA0); }

uint32_t m5core2_power::coulomb_charge(void) { return Read32bit(0xB0); }

uint32_t m5core2_power::coulomb_discharge(void) { return Read32bit(0xB4); }

float m5core2_power::coulomb(void) {
  uint32_t coin = 0;
  uint32_t coout = 0;

  coin = coulomb_charge();
  coout = coulomb_discharge();

  //c = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
  //Adc rate can be read from 84H ,change this variable if you change the ADC reate
  float ccc = 65536 * 0.5 * (int32_t)(coin - coout) / 3600.0 / 25.0;
  return ccc;
}

// Cut all power, except for LDO1 (RTC)
void m5core2_power::power_off(void) { Write1Byte(0x32, Read8bit(0x32) | 0b10000000); }

void m5core2_power::adc_enable(bool value) {
  // Enable / Disable all ADCs
  Write1Byte(0x82, value ? 0xff : 0x00);
}

void m5core2_power::prepare_sleep(void) {
  // Disable ADCs
  adc_enable(false);

  // Turn LED off
  power_led_enable(false);

  // Turn LCD backlight off
  dcd3_enable(false);
}

// Get current battery level
float m5core2_power::battery_level(void) {
  const float batVoltage = battery_voltage();
  const float batPercentage =
      (batVoltage < 3.248088) ? (0) : (batVoltage - 3.120712) * 100;
  return (batPercentage <= 100) ? batPercentage : 100;
}

void m5core2_power::restore_from_light_sleep(void) {
  // Turn LCD backlight on
  dcd3_enable(true);

  // Turn LED on
  power_led_enable(true);

  // Enable ADCs
  adc_enable(true);
}

uint8_t m5core2_power::warning_level_raw(void) {
  Wire1.beginTransmission(0x34);
  Wire1.write(0x47);
  Wire1.endTransmission();
  Wire1.requestFrom(0x34, 1);
  uint8_t buf = Wire1.read();
  return (buf & 0x01);
}

// -- sleep
void m5core2_power::deep_sleep(uint64_t time_in_us) {
  prepare_sleep();

  if (time_in_us > 0) {
    esp_sleep_enable_timer_wakeup(time_in_us);
  } else {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
  (time_in_us == 0) ? esp_deep_sleep_start() : esp_deep_sleep(time_in_us);

  // Never reached - after deep sleep ESP32 restarts
}

void m5core2_power::light_sleep(uint64_t time_in_us) {
  prepare_sleep();

  if (time_in_us > 0) {
    esp_sleep_enable_timer_wakeup(time_in_us);
  } else {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
  esp_light_sleep_start();

  restore_from_light_sleep();
}

uint8_t m5core2_power::warning_level(void) { return Read8bit(0x47) & 0x01; }

float m5core2_power::battery_voltage() {
  float ADCLSB = 1.1 / 1000.0;
  uint16_t ReData = Read12Bit(0x78);
  return ReData * ADCLSB;
}

float m5core2_power::battery_current() {
  float ADCLSB = 0.5;
  uint16_t CurrentIn = Read13Bit(0x7A);
  uint16_t CurrentOut = Read13Bit(0x7C);
  return (CurrentIn - CurrentOut) * ADCLSB;
}

float m5core2_power::vin_voltage() {
  float ADCLSB = 1.7 / 1000.0;
  uint16_t ReData = Read12Bit(0x56);
  return ReData * ADCLSB;
}

float m5core2_power::vin_current() {
  float ADCLSB = 0.625;
  uint16_t ReData = Read12Bit(0x58);
  return ReData * ADCLSB;
}

float m5core2_power::vbus_voltage() {
  float ADCLSB = 1.7 / 1000.0;
  uint16_t ReData = Read12Bit(0x5A);
  return ReData * ADCLSB;
}

float m5core2_power::vbus_current() {
  float ADCLSB = 0.375;
  uint16_t ReData = Read12Bit(0x5C);
  return ReData * ADCLSB;
}

float m5core2_power::temperature() {
  float ADCLSB = 0.1;
  const float OFFSET_DEG_C = -144.7;
  uint16_t ReData = Read12Bit(0x5E);
  return OFFSET_DEG_C + ReData * ADCLSB;
}

float m5core2_power::battery_power() {
  float VoltageLSB = 1.1;
  float CurrentLCS = 0.5;
  uint32_t ReData = Read24bit(0x70);
  return VoltageLSB * CurrentLCS * ReData / 1000.0;
}

float m5core2_power::battery_charge_current() {
  float ADCLSB = 0.5;
  uint16_t ReData = Read12Bit(0x7A);
  return ReData * ADCLSB;
}
float m5core2_power::aps_voltage() {
  float ADCLSB = 1.4 / 1000.0;
  uint16_t ReData = Read12Bit(0x7E);
  return ReData * ADCLSB;
}

float m5core2_power::battery_coulomb_input() {
  uint32_t ReData = Read32bit(0xB0);
  return ReData * 65536 * 0.5 / 3600 / 25.0;
}

float m5core2_power::battery_coulomb_output() {
  uint32_t ReData = Read32bit(0xB4);
  return ReData * 65536 * 0.5 / 3600 / 25.0;
}

void m5core2_power::set_coulomb_clear() { Write1Byte(0xB8, 0x20); }

void m5core2_power::ldo2_enable(bool value) {
  uint8_t buf = Read8bit(0x12);
  if (value)
    buf = (1 << 2) | buf;
  else
    buf = ~(1 << 2) & buf;
  Write1Byte(0x12, buf);
}

void m5core2_power::dcd3_enable(bool State) {
  uint8_t buf = Read8bit(0x12);
  if (State == true)
    buf = (1 << 1) | buf;
  else
    buf = ~(1 << 1) & buf;
  Write1Byte(0x12, buf);
}

uint8_t m5core2_power::state() { return Read8bit(0x00); }
bool m5core2_power::ac_in() { return (Read8bit(0x00) & 0x80) ? true : false; }
bool m5core2_power::charging() { return (Read8bit(0x00) & 0x04) ? true : false; }
bool m5core2_power::vbus() { return (Read8bit(0x00) & 0x20) ? true : false; }

void m5core2_power::ldo_voltage(uint8_t number, float voltage) {
  uint16_t value = voltage * 1000;
  //value = (value > 3300) ? 15 : (value / 100) - 18;
  value = calc_voltage_data(value,3300,1800,100)&0x0F;
  switch (number) {
    //uint8_t reg, data;
    case 2:
      Write1Byte(0x28, (Read8bit(0x28) & 0X0F) | (value << 4));
      break;
    case 3:
      Write1Byte(0x28, (Read8bit(0x28) & 0XF0) | value);
      break;
  }
}

void m5core2_power::dc_voltage(uint8_t number, float voltage) {
  uint8_t addr;
  uint16_t value = voltage * 1000;
  if (number > 2) return;
  //value = (value < 700) ? 0 : (value - 700) / 25;
  switch (number) {
    case 0:
      addr = 0x26;
      value = calc_voltage_data(value,3500,700,25) & 0x7f;
      break;
    case 1:
      addr = 0x25;
      value = calc_voltage_data(value,2275,700,25) & 0x3f;
      break;
    case 2:
      addr = 0x27;
      value = calc_voltage_data(value,3500,700,25) & 0x7f;
      break;
  }
  Write1Byte(addr, (Read8bit(addr) & 0X80) | (value & 0X7F));
}

void m5core2_power::mcu_voltage(float voltage) {
  if (voltage >= 3.000 && voltage <= 3.400) {
    dc_voltage(0, voltage);
  }
}
void m5core2_power::lcd_voltage(float voltage) {
  if (voltage >= 2.500 && voltage <= 3.300) {
    dc_voltage(2, voltage);
  }
}

void m5core2_power::ldo_enable(uint8_t number, bool state) {
  uint8_t mark = 0x01;
  if ((number < 2) || (number > 3)) return;

  mark <<= number;
  if (state) {
    Write1Byte(0x12, (Read8bit(0x12) | mark));
  } else {
    Write1Byte(0x12, (Read8bit(0x12) & (~mark)));
  }
}

void m5core2_power::lcd_reset_enable(bool state) {
  uint8_t reg_addr = 0x96;
  uint8_t gpio_bit = 0x02;
  uint8_t data;
  data = Read8bit(reg_addr);

  if (state) {
    data |= gpio_bit;
  } else {
    data &= ~gpio_bit;
  }

  Write1Byte(reg_addr, data);
}

// Select source for BUS_5V
// 0 : use internal boost
// 1 : powered externally
void m5core2_power::bus_external_power_enable(bool value) {
  uint8_t data;
  if (!value) {
    // Set GPIO to 3.3V (LDO OUTPUT mode)
    data = Read8bit(0x91);
    Write1Byte(0x91, (data & 0x0F) | 0xF0);
    // Set GPIO0 to LDO OUTPUT, pullup N_VBUSEN to disable VBUS supply from BUS_5V
    data = Read8bit(0x90);
    Write1Byte(0x90, (data & 0xF8) | 0x02);
    // Set EXTEN to enable 5v boost
    data = Read8bit(0x10);
    Write1Byte(0x10, data | 0x04);
  } else {
    // Set EXTEN to disable 5v boost
    data = Read8bit(0x10);
    Write1Byte(0x10, data & ~0x04);
    // Set GPIO0 to float, using enternal pulldown resistor to enable VBUS supply from BUS_5V
    data = Read8bit(0x90);
    Write1Byte(0x90, (data & 0xF8) | 0x07);
  }
}

void m5core2_power::power_led_enable(bool value) {
  uint8_t reg_addr = 0x94;
  uint8_t data;
  data = Read8bit(reg_addr);

  if (value) {
    data = data & 0XFD;
  } else {
    data |= 0X02;
  }

  Write1Byte(reg_addr, data);
}

//set led state(GPIO high active,set 1 to enable amplifier)
void m5core2_power::speaker_enable(bool value) {
  uint8_t reg_addr = 0x94;
  uint8_t gpio_bit = 0x04;
  uint8_t data;
  data = Read8bit(reg_addr);

  if (value) {
    data |= gpio_bit;
  } else {
    data &= ~gpio_bit;
  }

  Write1Byte(reg_addr, data);
}

void m5core2_power::charge_current(m5core2_charge_current value) {
  uint8_t data = Read8bit(0x33);
  data &= 0xf0;
  data = data | (((int)value) & 0x0f);
  Write1Byte(0x33, data);
}