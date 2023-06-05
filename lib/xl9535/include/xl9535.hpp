#pragma once
#if __has_include(<Arduino.h>)
#include <Arduino.h>
#include <Wire.h>
namespace arduino {
#else 
#ifndef ESP_PLATFORM
#error "This library requires Arduino or an ESP32"
#endif
#include <inttypes.h>
#include <driver/i2c.h>
#include <driver/gpio.h>
namespace esp_idf {
#endif
enum struct xl9535_pin_mode {
    output = 0,
    input = 1
};
class xl9535 {
    void do_move(xl9535& rhs);
    xl9535(const xl9535& rhs)=delete;
    xl9535& operator=(const xl9535& rhs)=delete;
    
#ifdef ARDUINO
    TwoWire& m_i2c;
#else
    i2c_port_t m_i2c;
#endif
    uint8_t m_address;
public:
    xl9535(
#ifdef ARDUINO
    TwoWire& i2c = Wire
#else
    i2c_port_t i2c = I2C_NUM_0
#endif  
    , uint8_t address = 0x20
    );
    xl9535(xl9535&& rhs);
    xl9535& operator=(xl9535&& rhs);
    void mode(uint8_t pin, xl9535_pin_mode mode);
    void mode_mask_lo(uint8_t lo_mask,xl9535_pin_mode mode);
    void mode_mask_hi(uint8_t lo_mask,xl9535_pin_mode mode);
    bool pin(uint8_t pin) const;
    void pin(uint8_t pin, bool value);
};
}

