#ifndef CONFIG_H
#define CONFIG_H

#ifdef TTGO_T1
#if __has_include(<button.hpp>)
#define PIN_NUM_BUTTON_A 35
#define PIN_NUM_BUTTON_B 0
#include <button.hpp>

#endif
#endif  // TTGO_T1

#ifdef ESP_WROVER_KIT
#include <esp_lcd_panel_ili9341.h>
#endif  // ESP_WROVER_KIT

#ifdef ESP_DISPLAY_S3
#include <esp_lcd_panel_ili9488.h>
#define I2C_PIN_NUM_SDA 38
#define I2C_PIN_NUM_SCL 39
#if __has_include(<ft6236.hpp>)
#define LCD_TOUCH
#define EXTRA_DECLS ft6236<LCD_HRES, LCD_VRES> touch;
#define LCD_ROTATION 1
#define EXTRA_INIT      \
    touch.initialize(); \
    touch.rotation(LCD_ROTATION);
#define LCD_TOUCH_IMPL                                                     \
    if (touch.update()) {                                                  \
        if (touch.xy(&out_locations[0].x, &out_locations[0].y)) {          \
            if (*in_out_locations_size > 1) {                              \
                *in_out_locations_size = 1;                                \
                if (touch.xy2(&out_locations[1].x, &out_locations[1].y)) { \
                    *in_out_locations_size = 2;                            \
                }                                                          \
            } else {                                                       \
                *in_out_locations_size = 1;                                \
            }                                                              \
        } else {                                                           \
            *in_out_locations_size = 0;                                    \
        }                                                                  \
    }
#include <ft6236.hpp>
#endif
#endif  // ESP_DISPLAY_S3

#ifdef ESP_DISPLAY_4INCH
#if __has_include(<gt911.hpp>)
#define I2C_PIN_NUM_SDA 17
#define I2C_PIN_NUM_SCL 18
#define LCD_TOUCH_PIN_NUM_RST 38
#define LCD_TOUCH
#define EXTRA_DECLS gt911<LCD_TOUCH_PIN_NUM_RST> touch;
#define LCD_TOUCH_IMPL                          \
    touch.update();                             \
    size_t touches = touch.locations_size();    \
    if (touches) {                              \
        if (touches > *in_out_locations_size) { \
            touches = *in_out_locations_size;   \
        }                                       \
        decltype(touch)::point pt[5];           \
        touch.locations(pt, &touches);          \
        for (uint8_t i = 0; i < touches; i++) { \
            out_locations[i].x = pt[i].x;       \
            out_locations[i].y = pt[i].y;       \
        }                                       \
    }                                           \
    *in_out_locations_size = touches;
#define EXTRA_INIT touch.initialize();
#include <gt911.hpp>

#endif // __has_include
#endif  // ESP_DISPLAY_4INCH

#ifdef M5STACK_CORE2
#if __has_include(<ft6336.hpp>)
#define LCD_TOUCH
#define LCD_TOUCH_IMPL \
    vTaskDelay(pdMS_TO_TICKS(1));                                          \
    if (touch.update()) {                                                  \
        if (touch.xy(&out_locations[0].x, &out_locations[0].y)) {          \
            if (*in_out_locations_size > 1) {                              \
                *in_out_locations_size = 1;                                \
                if (touch.xy2(&out_locations[1].x, &out_locations[1].y)) { \
                    *in_out_locations_size = 2;                            \
                }                                                          \
            } else {                                                       \
                *in_out_locations_size = 1;                                \
            }                                                              \
        } else {                                                           \
            *in_out_locations_size = 0;                                    \
        }                                                                  \
    }
#ifdef ESP_IDF
#define LCD_TOUCH_I2C I2C_NUM_1
#else 
#define LCD_TOUCH_I2C Wire1
#endif // ESP_IDF
#define EXTRA_DECLS m5core2_power power; \
                    ft6336<LCD_HRES, LCD_VRES> touch(LCD_TOUCH_I2C);
#else
#define EXTRA_DECLS m5core2_power power; 
#endif // __has_include
#ifdef ESP_IDF
#if __has_include(<ft6336.hpp>)
#define EXTRA_INIT                                         \
    power.initialize(); \
    touch.initialize();
#else
#define EXTRA_INIT                                         \
    power.initialize();
#endif // __has_include
#else
#if __has_include(<ft6336.hpp>)
#define EXTRA_INIT \
    power.initialize(); \
    touch.initialize();
#else
    power.initialize();
#endif // __has_include
#endif // ESP_IDF
#include <esp_lcd_panel_ili9342.h>
#include <driver/i2c.h>
#if __has_include(<m5core2_power.hpp>)
#include <m5core2_power.hpp>
#endif // __has_include
#if __has_include(<ft6336.hpp>)
#include <ft6336.hpp>

#endif // __has_include
#endif  // M5STACK_CORE2

#ifdef M5STACK_FIRE
#include <esp_lcd_panel_ili9342.h>
#define PIN_NUM_BUTTON_A 39
#define PIN_NUM_BUTTON_B 38
#define PIN_NUM_BUTTON_C 37
#include <button.hpp>
#endif  // M5STACK_FIRE

#ifdef T_DISPLAY_S3
#define PIN_NUM_POWER 15
#ifdef ESP_IDF
#define EXTRA_INIT                  \
    gpio_set_direction((gpio_num_t)PIN_NUM_POWER, GPIO_MODE_OUTPUT); \
    gpio_set_level((gpio_num_t)PIN_NUM_POWER,1);
#else
#define EXTRA_INIT                  \
    pinMode(PIN_NUM_POWER, OUTPUT); \
    digitalWrite(PIN_NUM_POWER, HIGH);
#endif
#if __has_include(<button.hpp>)
#define PIN_NUM_BUTTON_A 0
#define PIN_NUM_BUTTON_B 14
#include <button.hpp>

#endif  // __has_include(<button.hpp>)
#endif  // T_DISPLAY_S3
#ifdef ESP_IDF
#ifdef I2C_PIN_NUM_SDA
#include <driver/i2c.h>
#define I2C_INIT                                           \
        i2c_config_t i2c_conf;                                 \
        i2c_conf.mode = I2C_MODE_MASTER;                       \
        i2c_conf.sda_io_num = (gpio_num_t)I2C_PIN_NUM_SDA;     \
        i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;           \
        i2c_conf.scl_io_num = (gpio_num_t)I2C_PIN_NUM_SCL;     \
    i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;               \
        i2c_conf.master.clk_speed = 100 * 1000;                \
        i2c_conf.clk_flags = 0;                                \
        i2c_param_config(I2C_NUM_0, &i2c_conf);                \
        i2c_driver_install(I2C_NUM_0, i2c_conf.mode, 0, 0, 0); 
#endif  // I2C_PIN_NUM_SDA
#else
#ifdef I2C_PIN_NUM_SDA
#include <Wire.h>
#define I2C_INIT \
    Wire.begin(I2C_PIN_NUM_SDA, I2C_PIN_NUM_SCL);
#endif  // I2C_PIN_NUM_SDA
#endif  // ESP_IDF
#endif  // CONFIG_H