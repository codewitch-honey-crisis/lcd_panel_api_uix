#ifndef CONFIG_H
#define CONFIG_H
#define LCD_SWAP_XY true
#ifdef TTGO_T1
#define LCD_SPI_HOST    SPI3_HOST
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_MOSI 19
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define PIN_NUM_DC 16
#define PIN_NUM_RST 23
#define PIN_NUM_BCKL 4
#define LCD_PANEL esp_lcd_new_panel_st7789
#define LCD_HRES 135
#define LCD_VRES 240
#define PIN_NUM_BUTTON_A 35
#define PIN_NUM_BUTTON_B 0
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_RGB
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 40
#define LCD_GAP_Y 52
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y true
#define LCD_INVERT_COLOR true
#define LCD_ROTATION 1
#endif // TTGO_T1

#ifdef ESP_WROVER_KIT
#define LCD_BK_LIGHT_ON_LEVEL 0
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define LCD_SPI_HOST    HSPI_HOST
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22
#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5
#define LCD_PANEL esp_lcd_new_panel_ili9341
#define LCD_HRES 240
#define LCD_VRES 320
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y false
#define LCD_INVERT_COLOR false
#define LCD_ROTATION 1
#endif // ESP_WROVER_KIT

#ifdef ESP_DISPLAY_S3
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_CS 37
#define PIN_NUM_WR 35
#define PIN_NUM_RD 48
#define PIN_NUM_RS 36
#define PIN_NUM_D00 47
#define PIN_NUM_D01 21
#define PIN_NUM_D02 14
#define PIN_NUM_D03 13
#define PIN_NUM_D04 12
#define PIN_NUM_D05 11
#define PIN_NUM_D06 10
#define PIN_NUM_D07 9
#define PIN_NUM_D08 3
#define PIN_NUM_D09 8
#define PIN_NUM_D10 16
#define PIN_NUM_D11 15
#define PIN_NUM_D12 7
#define PIN_NUM_D13 6
#define PIN_NUM_D14 5
#define PIN_NUM_D15 4
#define PIN_NUM_BCKL 45
#define LCD_PANEL esp_lcd_new_panel_ili9488
#define LCD_HRES 320
#define LCD_VRES 480
#define I2C_SDA 38
#define I2C_SCL 39
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_SWAP_COLOR_BYTES true
#define LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y false
#define LCD_INVERT_COLOR false
#define LCD_TOUCH ft6236<LCD_HRES,LCD_VRES>
#define LCD_ROTATION 1
#endif // ESP_DISPLAY_S3

#ifdef M5STACK_CORE2
#define LCD_SPI_HOST    SPI3_HOST
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define PIN_NUM_DC 15
#define LCD_PANEL esp_lcd_new_panel_ili9342
#define LCD_HRES 240
#define LCD_VRES 320
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y false
#define LCD_INVERT_COLOR true
#define LCD_ROTATION 1
#define LCD_TOUCH ft6336<LCD_HRES,LCD_VRES,-1>
#define LCD_TOUCH_WIRE Wire1
#define EXTRA_DECLS m5core2_power power;
#define EXTRA_INIT power.initialize();
#endif

#ifdef M5STACK_FIRE
#define LCD_SPI_HOST    SPI3_HOST
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 14
#define PIN_NUM_DC 27
#define PIN_NUM_RST 33
#define PIN_NUM_BCKL 32
#define PIN_NUM_BUTTON_A 39
#define PIN_NUM_BUTTON_B 38
#define PIN_NUM_BUTTON_C 37

#define LCD_PANEL esp_lcd_new_panel_ili9342
#define LCD_HRES 240
#define LCD_VRES 320
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y false
#define LCD_INVERT_COLOR true
#define LCD_ROTATION 1
#endif

#ifdef T_DISPLAY_S3
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define PIN_NUM_CS 6
#define PIN_NUM_RST 5
#define PIN_NUM_WR 8
#define PIN_NUM_RD 9
#define PIN_NUM_RS 7
#define PIN_NUM_D00 39
#define PIN_NUM_D01 40
#define PIN_NUM_D02 41
#define PIN_NUM_D03 42
#define PIN_NUM_D04 45
#define PIN_NUM_D05 46
#define PIN_NUM_D06 47
#define PIN_NUM_D07 48
#define PIN_NUM_BCKL 38
#define LCD_PANEL esp_lcd_new_panel_st7789
#define LCD_HRES 170
#define LCD_VRES 320
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_RGB
#define LCD_PIXEL_CLOCK_HZ (6528 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 35
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y true
#define LCD_INVERT_COLOR true
#define LCD_ROTATION 1
#define PIN_NUM_BUTTON_A 0
#define PIN_NUM_BUTTON_B 14
#define PIN_NUM_POWER 15
#define EXTRA_INIT pinMode(PIN_NUM_POWER,OUTPUT); digitalWrite(PIN_NUM_POWER,HIGH);
#endif 

#endif // CONFIG_H