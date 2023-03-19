#ifndef CONFIG_H
#define CONFIG_H
#ifdef TTGO_T1
#define PIN_NUM_BUTTON_A 35
#define PIN_NUM_BUTTON_B 0
#define LCD_ROTATION 1
#endif // TTGO_T1

#ifdef ESP_DISPLAY_S3
#define I2C_SDA 38
#define I2C_SCL 39
#define LCD_TOUCH ft6236<LCD_HRES,LCD_VRES>
#define LCD_ROTATION 1
#endif // ESP_DISPLAY_S3

#ifdef M5STACK_CORE2
#define LCD_TOUCH ft6336<LCD_HRES,LCD_VRES,-1>
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
#define EXTRA_INIT pinMode(PIN_NUM_POWER,OUTPUT); digitalWrite(PIN_NUM_POWER,HIGH);
#endif 

#endif // CONFIG_H