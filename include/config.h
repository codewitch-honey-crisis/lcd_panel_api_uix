#ifndef CONFIG_H
#define CONFIG_H
#ifdef TTGO_T1
#define PIN_NUM_BUTTON_A 35
#define PIN_NUM_BUTTON_B 0
#define LCD_ROTATION 1
#endif  // TTGO_T1

#ifdef ESP_DISPLAY_S3
#define I2C_SDA 38
#define I2C_SCL 39
#define LCD_TOUCH ft6236<LCD_HRES, LCD_VRES>
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
#endif  // ESP_DISPLAY_S3
#ifdef ESP_DISPLAY_4INCH
#define I2C_SDA 17
#define I2C_SCL 18

#define PIN_NUM_TOUCH_RST 38

#define LCD_TOUCH gt911<PIN_NUM_TOUCH_RST>
#define LCD_TOUCH_IMPL \
touch.update(); \
size_t touches = touch.locations_size(); \
if (touches) { \
    if(touches>*in_out_locations_size) { \
        touches = *in_out_locations_size; \
    } \
    decltype(touch)::point pt[5]; \
    touch.locations(pt,&touches); \
    for (uint8_t  i = 0; i < touches; i++) { \
        out_locations[i].x = pt[i].x; \
        out_locations[i].y = pt[i].y; \
    } \
} \
*in_out_locations_size = touches;
#define EXTRA_INIT touch.initialize();
#endif

#ifdef M5STACK_CORE2
#define LCD_TOUCH ft6336<LCD_HRES, LCD_VRES, -1>
#define LCD_TOUCH_WIRE Wire1
#define EXTRA_DECLS m5core2_power power;
#define EXTRA_INIT power.initialize();
#endif

#ifdef M5STACK_FIRE
#define LCD_ROTATION 1
#define PIN_NUM_BUTTON_A 39
#define PIN_NUM_BUTTON_B 38
#define PIN_NUM_BUTTON_C 37
#endif

#ifdef T_DISPLAY_S3
#define LCD_ROTATION 1
#define PIN_NUM_BUTTON_A 0
#define PIN_NUM_BUTTON_B 14
#define PIN_NUM_POWER 15
#define EXTRA_INIT                  \
    pinMode(PIN_NUM_POWER, OUTPUT); \
    digitalWrite(PIN_NUM_POWER, HIGH);
#endif

#endif  // CONFIG_H