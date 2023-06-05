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

#ifdef ESP_DISPLAY_4_3INCH
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

#ifdef S3_T_QT_PRO
#define PIN_NUM_POWER 4
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
#define PIN_NUM_BUTTON_A            0
#define PIN_NUM_BUTTON_B            47
#include <button.hpp>
#endif // __has_include
#endif // S3_T_QT_PRO

#ifdef T_RGB
#define I2C_PIN_NUM_SDA 8
#define I2C_PIN_NUM_SCL 48
#define I2C_FREQ (400*1000)
#define PIN_EX_NUM_POWER 2
#define LCD_PIN_EX_NUM_CS 3
#define LCD_PIN_EX_NUM_SDA 4 
#define LCD_PIN_EX_NUM_SCK 5
#define LCD_PIN_EX_NUM_RST 6

#include <xl9535.hpp>
#ifdef ESP_IDF
using namespace esp_idf;
#else
using namespace arduino;
#endif // ESP_IDF
    
#define EXTRA_INIT \
    io_exp.mode(PIN_EX_NUM_POWER,xl9535_pin_mode::output); \
    io_exp.pin(PIN_EX_NUM_POWER,true);

typedef struct {
  uint8_t cmd;
  uint8_t data[16];
  uint8_t databytes; // No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;
DRAM_ATTR static const lcd_init_cmd_t st_init_cmds[] = {
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}, 0x05},
    {0xC0, {0x3b, 0x00}, 0x02},
    {0xC1, {0x0b, 0x02}, 0x02},
    {0xC2, {0x07, 0x02}, 0x02},
    {0xCC, {0x10}, 0x01},
    {0xCD, {0x08}, 0x01}, // 用565时屏蔽    666打开
    {0xb0, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18}, 0x10},
    {0xb1, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2a, 0x31, 0x18}, 0x10},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x11}, 0x05},
    {0xb0, {0x6d}, 0x01},
    {0xb1, {0x37}, 0x01},
    {0xb2, {0x81}, 0x01},
    {0xb3, {0x80}, 0x01},
    {0xb5, {0x43}, 0x01},
    {0xb7, {0x85}, 0x01},
    {0xb8, {0x20}, 0x01},
    {0xc1, {0x78}, 0x01},
    {0xc2, {0x78}, 0x01},
    {0xc3, {0x8c}, 0x01},
    {0xd0, {0x88}, 0x01},
    {0xe0, {0x00, 0x00, 0x02}, 0x03},
    {0xe1, {0x03, 0xa0, 0x00, 0x00, 0x04, 0xa0, 0x00, 0x00, 0x00, 0x20, 0x20}, 0x0b},
    {0xe2, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x0d},
    {0xe3, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe4, {0x22, 0x00}, 0x02},
    {0xe5, {0x05, 0xec, 0xa0, 0xa0, 0x07, 0xee, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xe6, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe7, {0x22, 0x00}, 0x02},
    {0xe8, {0x06, 0xed, 0xa0, 0xa0, 0x08, 0xef, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xeb, {0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}, 0x07},
    {0xed, {0xff, 0xff, 0xff, 0xba, 0x0a, 0xbf, 0x45, 0xff, 0xff, 0x54, 0xfb, 0xa0, 0xab, 0xff, 0xff, 0xff}, 0x10},
    {0xef, {0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f}, 0x06},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x13}, 0x05},
    {0xef, {0x08}, 0x01},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x00}, 0x05},
    {0x36, {0x08}, 0x01},
    {0x3a, {0x66}, 0x01},
    {0x11, {0x00}, 0x80},
    // {0xFF, {0x77, 0x01, 0x00, 0x00, 0x12}, 0x05},
    // {0xd1, {0x81}, 0x01},
    // {0xd2, {0x06}, 0x01},
    {0x29, {0x00}, 0x80},
    {0, {0}, 0xff}};
xl9535 io_exp;
static void lcd_send_data(uint8_t data) {
  uint8_t n;
  for (n = 0; n < 8; n++) {
    if (data & 0x80)
      io_exp.pin(LCD_PIN_EX_NUM_SDA, true);
    else
        io_exp.pin(LCD_PIN_EX_NUM_SDA, false);

    data <<= 1;
    io_exp.pin(LCD_PIN_EX_NUM_SCK, false);
    io_exp.pin(LCD_PIN_EX_NUM_SCK, true);
  }
}

static void lcd_cmd(const uint8_t cmd) {
  io_exp.pin(LCD_PIN_EX_NUM_CS, false);
  io_exp.pin(LCD_PIN_EX_NUM_SDA, false);
  io_exp.pin(LCD_PIN_EX_NUM_SCK, false);
  io_exp.pin(LCD_PIN_EX_NUM_SCK, true);
  lcd_send_data(cmd);
  io_exp.pin(LCD_PIN_EX_NUM_CS, true);
}

static void lcd_data(const uint8_t *data, int len) {
  uint32_t i = 0;
  if (len == 0)
    return; // no need to send anything
  do {
    io_exp.pin(LCD_PIN_EX_NUM_CS, false);
    io_exp.pin(LCD_PIN_EX_NUM_SDA, true);
    io_exp.pin(LCD_PIN_EX_NUM_SCK, false);
    io_exp.pin(LCD_PIN_EX_NUM_SCK, true);
    lcd_send_data(*(data + i));
    io_exp.pin(LCD_PIN_EX_NUM_CS, true);
    i++;
  } while (len--);
}

esp_err_t esp_lcd_new_panel_st7701_t_rgb() {
  io_exp.mode(LCD_PIN_EX_NUM_CS,xl9535_pin_mode::output);
  io_exp.pin(LCD_PIN_EX_NUM_CS,true);
  io_exp.mode(LCD_PIN_EX_NUM_SDA,xl9535_pin_mode::output);
  io_exp.pin(LCD_PIN_EX_NUM_SDA,true);
  io_exp.mode(LCD_PIN_EX_NUM_SCK,xl9535_pin_mode::output);
  io_exp.pin(LCD_PIN_EX_NUM_SCK,true);

  // Reset the display
  io_exp.mode(LCD_PIN_EX_NUM_RST,xl9535_pin_mode::output);
  io_exp.pin(LCD_PIN_EX_NUM_RST, 1);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  io_exp.pin(LCD_PIN_EX_NUM_RST, false);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  io_exp.pin(LCD_PIN_EX_NUM_RST, true);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  int cmd = 0;
  while (st_init_cmds[cmd].databytes != 0xff) {
    lcd_cmd(st_init_cmds[cmd].cmd);
    lcd_data(st_init_cmds[cmd].data, st_init_cmds[cmd].databytes & 0x1F);
    if (st_init_cmds[cmd].databytes & 0x80) {
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    cmd++;
  }
  return ESP_OK;
}
#endif // T_RGB
#ifdef ESP_IDF
#ifdef I2C_PIN_NUM_SDA
#ifndef I2C_FREQ
#define I2C_FREQ (100*1000)
#endif
#include <driver/i2c.h>
#define I2C_INIT                                           \
        i2c_config_t i2c_conf;                                 \
        i2c_conf.mode = I2C_MODE_MASTER;                       \
        i2c_conf.sda_io_num = (gpio_num_t)I2C_PIN_NUM_SDA;     \
        i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;           \
        i2c_conf.scl_io_num = (gpio_num_t)I2C_PIN_NUM_SCL;     \
        i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;           \
        i2c_conf.master.clk_speed = I2C_FREQ;                \
        i2c_conf.clk_flags = 0;                                \
        i2c_param_config(I2C_NUM_0, &i2c_conf);                \
        i2c_driver_install(I2C_NUM_0, i2c_conf.mode, 0, 0, 0); 
#endif  // I2C_PIN_NUM_SDA
#else
#ifdef I2C_PIN_NUM_SDA
#ifndef I2C_FREQ
#define I2C_FREQ (100*1000)
#endif
#include <Wire.h>
#define I2C_INIT \
    Wire.begin(I2C_PIN_NUM_SDA, I2C_PIN_NUM_SCL,(uint32_t)I2C_FREQ); 
#endif  // I2C_PIN_NUM_SDA
#endif  // ESP_IDF
#endif  // CONFIG_H
