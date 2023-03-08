#ifndef CONFIG_H
#define CONFIG_H
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)

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
#define PIN_BUTTON_A 35
#define PIN_BUTTON_B 0
#define I2C_SDA -1
#define I2C_SCL -1
#endif // TTGO_T1

#ifdef WROVER_KIT
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
#define I2C_SDA -1
#define I2C_SCL -1
#endif // WROVER_KIT

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
#define PIN_NUM_RST -1
#define PIN_NUM_BCKL 45
#define LCD_PANEL esp_lcd_new_panel_ili9488
#define LCD_HRES 320
#define LCD_VRES 480
#define I2C_SDA 38
#define I2C_SCL 39

#endif // ESP_DISPLAY_S3

#endif