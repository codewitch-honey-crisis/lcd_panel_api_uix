#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#ifdef ESP_WROVER_KIT
#include "esp_lcd_panel_ili9341.h"
#endif // ESP_WROVER_KIT
#ifdef ESP_DISPLAY_S3
#include "esp_lcd_panel_ili9488.h"
#include <ft6236.hpp>
using namespace arduino;
#endif // ESP_DISPLAY_S3
#ifdef M5STACK_CORE2
#include "esp_lcd_panel_ili9342.h"
#include "m5core2_power.hpp"
#include <ft6336.hpp>
using namespace arduino;
#endif // M5STACK_CORE2
#ifdef M5STACK_FIRE
#include "esp_lcd_panel_ili9342.h"
#endif // M5STACK_FIRE
#if defined(PIN_NUM_BUTTON_A) || defined(PIN_NUM_BUTTON_B)
#include <button.hpp>
using namespace arduino;
#endif // defined(PIN_NUM_BUTTON_A) || defined(PIN_NUM_BUTTON_B)
#include <gfx.hpp>
using namespace gfx;
#include <uix.hpp>
using namespace uix;
// SVG converted to header using
// https://honeythecodewitch.com/gfx/converter
#include <bee_icon.hpp>
// downloaded from fontsquirrel.com and header generated with
// https://honeythecodewitch.com/gfx/generator
//#include <fonts/Rubik_Black.hpp>
//#include <fonts/Telegrama.hpp>
#include <fonts/OpenSans_Regular.hpp>
static const open_font& text_font = OpenSans_Regular;
#ifdef LCD_TOUCH
void svg_touch();
void svg_release();
#endif // LCD_TOUCH
// declare a custom control
template <typename PixelType, typename PaletteType = gfx::palette<PixelType, PixelType>>
class svg_box : public control<PixelType, PaletteType> {
   // public and private type aliases
   // pixel_type and palette_type are 
   // required on any control
   public:
    using type = svg_box;
    using pixel_type = PixelType;
    using palette_type = PaletteType;
   private:
    using base_type = control<PixelType, PaletteType>;
    using control_surface_type = typename base_type::control_surface_type;
    // member data
    gfx::svg_doc* m_svg;
    // no reason for copy semantics
    svg_box(const svg_box& rhs) = delete;
    svg_box& operator=(const svg_box& rhs) = delete;
    // implements move semantics
    void do_move(svg_box& rhs) {
        do_move_control(rhs);
        m_svg = rhs.m_svg;
    }

   public:
    svg_box(svg_box&& rhs) {
        do_move(rhs);
    }
    svg_box& operator=(svg_box&& rhs) {
        do_move(rhs);
        return *this;
    }
    svg_doc* doc() const {
        return m_svg;
    }
    void doc(svg_doc* value) {
        if (value != m_svg) {
            m_svg = value;
            this->invalidate();
        }
    }
    svg_box(invalidation_tracker& parent, const palette_type* palette = nullptr) 
            : base_type(parent, palette), m_svg(nullptr) {

    }
    
    virtual void on_paint(control_surface_type& destination, const srect16& clip) override {
        // get the rect for the drawing area
        srect16 b = (srect16)this->dimensions().bounds();
        // if there's an SVG set, render it
        // scaled to the control
        if (m_svg != nullptr) {
            draw::svg(destination, 
                        this->bounds().dimensions().bounds(), 
                        *m_svg, 
                        m_svg->scale(b.dimensions()));
        }
        // call the base on paint method
        base_type::on_paint(destination, clip);
    }
#ifdef LCD_TOUCH
    virtual bool on_touch(size_t locations_size,const spoint16* locations) {
        svg_touch();     
        return true;
    }
    virtual void on_release() {
        svg_release();
    }
#endif // LCD_TOUCH
};

// declare the format of the screen (x,y are swapped compared 
// to the display settings)
using screen_t = screen<LCD_VRES, LCD_HRES, rgb_pixel<16>>;
// declare the control types to match the screen
#ifdef LCD_TOUCH
// since this supports touch, we use an interactive button
// instead of a static label
using label_t = push_button<typename screen_t::pixel_type>;
#else
using label_t = label<typename screen_t::pixel_type>;
#endif // LCD_TOUCH
using svg_box_t = svg_box<typename screen_t::pixel_type>;
// for access to RGB565 colors which LCDs and the main screen use
using color16_t = color<rgb_pixel<16>>;
// for access to RGBA8888 colors which controls use
using color32_t = color<rgba_pixel<32>>;
#ifdef PIN_NUM_BUTTON_A
// declare the buttons if defined
using button_a_t = int_button<PIN_NUM_BUTTON_A, 10, true>;
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
using button_b_t = int_button<PIN_NUM_BUTTON_B, 10, true>;
#endif
#if !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
using cycle_timer_t = uix::timer;
#endif // !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
#ifdef LCD_TOUCH
using touch_t = LCD_TOUCH;
#endif // LCD_TOUCH
// UIX allows you to use two buffers for maximum DMA efficiency
// you don't have to, but performance is significantly better
// declare 64KB across two buffers for transfer
constexpr static const int lcd_buffer_size = 32 * 1024;
uint8_t lcd_buffer1[lcd_buffer_size];
uint8_t lcd_buffer2[lcd_buffer_size];
// this is the handle from the esp lcd panel api
esp_lcd_panel_handle_t lcd_handle;

// our svg doc for svg_box
svg_doc doc;

#ifdef EXTRA_DECLS
EXTRA_DECLS
#endif // EXTRA_DECLS

// the main screen
screen_t main_screen(sizeof(lcd_buffer1), lcd_buffer1, lcd_buffer2);
// the controls
label_t test_label(main_screen);
svg_box_t test_svg(main_screen);

#ifdef PIN_NUM_BUTTON_A
// the buttons
button_a_t button_a;
void button_a_on_click(bool pressed, void* state) {
    if (pressed) {
        test_label.text_color(color32_t::red);
    } else {
        test_label.text_color(color32_t::blue);
    }
}
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
button_b_t button_b;
void button_b_on_click(bool pressed, void* state) {
    if (pressed) {
        main_screen.background_color(color16_t::light_green);
    } else {
        main_screen.background_color(color16_t::white);
    }
}
#endif // PIN_NUM_BUTTON_B
#if !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
int cycle_state = 0;
cycle_timer_t cycle_timer(1000, [](void* state) {
    switch (cycle_state) {
        case 0:
            main_screen.background_color(color16_t::light_green);
            break;
        case 1:
            main_screen.background_color(color16_t::white);
            break;
        case 2:
            test_label.text_color(color32_t::red);
            break;
        case 3:
            test_label.text_color(color32_t::blue);
            break;
    }
    ++cycle_state;
    if (cycle_state > 3) {
        cycle_state = 0;
    }
});
#endif // !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
#ifdef LCD_TOUCH
#ifdef LCD_TOUCH_WIRE
touch_t touch(LCD_TOUCH_WIRE);
#else
touch_t touch;
#endif // LCD_TOUCH_WIRE
static void uix_touch(point16* out_locations, size_t* in_out_locations_size, void* state) {
    if(in_out_locations_size<=0) {
        *in_out_locations_size=0;
        return;
    }
    if(touch.update()) {
        if(touch.xy(&out_locations[0].x,&out_locations[0].y)) {
            if(*in_out_locations_size>1) {
                *in_out_locations_size = 1;
                if(touch.xy2(&out_locations[1].x,&out_locations[1].y)) {
                    *in_out_locations_size = 2;
                }
            } else {
                *in_out_locations_size = 1;
            }
        } else {
            *in_out_locations_size = 0;
        }
    }
}
void svg_touch() {
    main_screen.background_color(color16_t::light_green);
}
void svg_release() {
    main_screen.background_color(color16_t::white);
}
#endif // LCD_TOUCH
// tell UIX the DMA transfer is complete
static bool lcd_flush_ready(esp_lcd_panel_io_handle_t panel_io, 
                            esp_lcd_panel_io_event_data_t* edata, 
                            void* user_ctx) {
    main_screen.set_flush_complete();
    return true;
}
// tell the lcd panel api to transfer data via DMA
static void uix_flush(point16 location, 
                    typename screen_t::bitmap_type& bmp, 
                    void* state) {
    // note that x2, and y2 extend past their destination by 1 pixel each.
    // this is a quirk of the ESP LCD Panel API, and must be done.
    int x1 = location.x, 
        y1 = location.y, 
        x2 = location.x + bmp.dimensions().width, 
        y2 = location.y + bmp.dimensions().height;
    esp_lcd_panel_draw_bitmap(lcd_handle, x1, y1, x2, y2, bmp.begin());
}
// initialize the screen using the esp lcd panel API
void lcd_panel_init() {
#ifdef PIN_NUM_BCKL
    pinMode(PIN_NUM_BCKL, OUTPUT);
#endif // PIN_NUM_BCKL
#ifdef LCD_SPI_HOST // 1-bit SPI
    spi_bus_config_t bus_config;
    memset(&bus_config, 0, sizeof(bus_config));
    bus_config.sclk_io_num = PIN_NUM_CLK;
    bus_config.mosi_io_num = PIN_NUM_MOSI;
#ifdef PIN_NUM_MISO
    bus_config.miso_io_num = PIN_NUM_MISO;
#else
    bus_config.miso_io_num = -1;
#endif // PIN_NUM_MISO
#ifdef PIN_NUM_QUADWP
    bus_config.quadwp_io_num = PIN_NUM_QUADWP;
#else
    bus_config.quadwp_io_num = -1;
#endif
#ifdef PIN_NUM_QUADHD
    bus_config.quadhd_io_num = PIN_NUM_QUADHD;
#else
    bus_config.quadhd_io_num = -1;
#endif
    bus_config.max_transfer_sz = sizeof(lcd_buffer1) + 8;

    // Initialize the SPI bus on LCD_SPI_HOST
    spi_bus_initialize(LCD_SPI_HOST, &bus_config, SPI_DMA_CH_AUTO);

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config;
    memset(&io_config, 0, sizeof(io_config));
    io_config.dc_gpio_num = PIN_NUM_DC,
    io_config.cs_gpio_num = PIN_NUM_CS,
    io_config.pclk_hz = LCD_PIXEL_CLOCK_HZ,
    io_config.lcd_cmd_bits = 8,
    io_config.lcd_param_bits = 8,
    io_config.spi_mode = 0,
    io_config.trans_queue_depth = 10,
    io_config.on_color_trans_done = lcd_flush_ready;
    // Attach the LCD to the SPI bus
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, 
                            &io_config, &
                            io_handle);
#elif defined(PIN_NUM_D07) // 8 or 16-bit i8080
    pinMode(PIN_NUM_RD,OUTPUT);
    digitalWrite(PIN_NUM_RD,HIGH);
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config;
    memset(&bus_config,0,sizeof(bus_config));
    bus_config.clk_src = LCD_CLK_SRC_PLL160M;
    bus_config.dc_gpio_num = PIN_NUM_RS;
    bus_config.wr_gpio_num = PIN_NUM_WR;
    bus_config.data_gpio_nums[0] = PIN_NUM_D00;
    bus_config.data_gpio_nums[1] = PIN_NUM_D01;
    bus_config.data_gpio_nums[2] = PIN_NUM_D02;
    bus_config.data_gpio_nums[3] = PIN_NUM_D03;
    bus_config.data_gpio_nums[4] = PIN_NUM_D04;
    bus_config.data_gpio_nums[5] = PIN_NUM_D05;
    bus_config.data_gpio_nums[6] = PIN_NUM_D06;
    bus_config.data_gpio_nums[7] = PIN_NUM_D07;
#ifdef PIN_NUM_D15
    bus_config.data_gpio_nums[8] = PIN_NUM_D08;
    bus_config.data_gpio_nums[9] = PIN_NUM_D09;
    bus_config.data_gpio_nums[10] = PIN_NUM_D10;
    bus_config.data_gpio_nums[11] = PIN_NUM_D11;
    bus_config.data_gpio_nums[12] = PIN_NUM_D12;
    bus_config.data_gpio_nums[13] = PIN_NUM_D13;
    bus_config.data_gpio_nums[14] = PIN_NUM_D14;
    bus_config.data_gpio_nums[15] = PIN_NUM_D15;
    bus_config.bus_width = 16;
#else
    bus_config.bus_width = 8;
#endif // PIN_NUM_D15
    bus_config.max_transfer_bytes = sizeof(lcd_buffer1);

    esp_lcd_new_i80_bus(&bus_config, &i80_bus);

    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_i80_config_t io_config;
    memset(&io_config,0,sizeof(io_config));
    io_config.cs_gpio_num = PIN_NUM_CS;
    io_config.pclk_hz = LCD_PIXEL_CLOCK_HZ;
    io_config.trans_queue_depth = 20;
    io_config.dc_levels.dc_idle_level=0;
    io_config.dc_levels.dc_idle_level = 0;
    io_config.dc_levels.dc_cmd_level = 0;
    io_config.dc_levels.dc_dummy_level = 0;
    io_config.dc_levels.dc_data_level = 1;    
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;
    io_config.on_color_trans_done = lcd_flush_ready;
    io_config.user_ctx = nullptr;
#ifdef LCD_SWAP_COLOR_BYTES
    io_config.flags.swap_color_bytes = LCD_SWAP_COLOR_BYTES;
#else 
    io_config.flags.swap_color_bytes = false;
#endif // LCD_SWAP_COLOR_BYTES
    io_config.flags.cs_active_high = false;
    io_config.flags.reverse_color_bits = false;
    esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle);
#endif // PIN_NUM_D15
    lcd_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config;
    memset(&panel_config, 0, sizeof(panel_config));
#ifdef PIN_NUM_RST
    panel_config.reset_gpio_num = PIN_NUM_RST;
#else
    panel_config.reset_gpio_num = -1;
#endif
    panel_config.color_space = LCD_COLOR_SPACE;
    panel_config.bits_per_pixel = 16;

    // Initialize the LCD configuration
    LCD_PANEL(io_handle, &panel_config, &lcd_handle);

#ifdef PIN_NUM_BCKL
        // Turn off backlight to avoid unpredictable display on 
    // the LCD screen while initializing
    // the LCD panel driver. (Different LCD screens may need different levels)
    digitalWrite(PIN_NUM_BCKL, LCD_BK_LIGHT_OFF_LEVEL);
#endif // PIN_NUM_BCKL
    // Reset the display
    esp_lcd_panel_reset(lcd_handle);

    // Initialize LCD panel
    esp_lcd_panel_init(lcd_handle);

    esp_lcd_panel_swap_xy(lcd_handle, LCD_SWAP_XY);
    esp_lcd_panel_set_gap(lcd_handle, LCD_GAP_X, LCD_GAP_Y);
    esp_lcd_panel_mirror(lcd_handle, LCD_MIRROR_X, LCD_MIRROR_Y);
    esp_lcd_panel_invert_color(lcd_handle, LCD_INVERT_COLOR);
    // Turn on the screen
    esp_lcd_panel_disp_off(lcd_handle, false);
#ifdef PIN_NUM_BCKL
    // Turn on backlight (Different LCD screens may need different levels)
    digitalWrite(PIN_NUM_BCKL, LCD_BK_LIGHT_ON_LEVEL);
#endif // PIN_NUM_BCKL
}
// initialize the screen and controls
void screen_init() {
    test_label.bounds(srect16(spoint16(0, 10), ssize16(200, 60))
                .center_horizontal(main_screen.bounds()));
    test_label.text_color(color32_t::blue);
    test_label.text_open_font(&text_font);
    test_label.text_line_height(45);
    test_label.text_justify(uix_justify::center);
    test_label.round_ratio(NAN);
    test_label.padding({8, 8});
    test_label.text("Hello!");
    // make the backcolor transparent
    auto bg = color32_t::black;
    bg.channel<channel_name::A>(0);
    test_label.background_color(bg);
    // and the border
    test_label.border_color(bg);
#ifdef LCD_TOUCH
    test_label.pressed_text_color(color32_t::red);
    test_label.pressed_background_color(bg);
    test_label.pressed_border_color(bg);
#endif // LCD_TOUCH
    
    test_svg.bounds(srect16(spoint16(0, 70), ssize16(60, 60))
                .center_horizontal(main_screen.bounds()));
    gfx_result res = svg_doc::read(&bee_icon, &doc);
    if (gfx_result::success != res) {
        Serial.printf("Error reading SVG: %d", (int)res);
    }
    test_svg.doc(&doc);
    main_screen.background_color(color16_t::white);
    main_screen.register_control(test_label);
    main_screen.register_control(test_svg);
    main_screen.on_flush_callback(uix_flush);
#ifdef LCD_TOUCH
    main_screen.on_touch_callback(uix_touch);
#endif // LCD_TOUCH
}
// set up the hardware
void setup() {
    Serial.begin(115200);
    Serial.write(bee_icon_data, sizeof(bee_icon_data));
    Serial.println();
#ifdef I2C_SDA
    Wire.begin(I2C_SDA,I2C_SCL);
#endif
#ifdef EXTRA_INIT
EXTRA_INIT
#endif //EXTRA_INIT
    lcd_panel_init();
    screen_init();
#ifdef PIN_NUM_BUTTON_A
    button_a.initialize();
    button_a.on_pressed_changed(button_a_on_click);
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
    button_b.initialize();
    button_b.on_pressed_changed(button_b_on_click);
#endif // PIN_NUM_BUTTON_B
#ifdef LCD_TOUCH
    touch.initialize();
    touch.rotation(LCD_ROTATION);
#endif // LCD_TOUCH
}
// keep our stuff up to date and responsive
void loop() {
#ifdef PIN_NUM_BUTTON_A
    button_a.update();
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
    button_b.update();
#endif // PIN_NUM_BUTTON_B
#if !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
    cycle_timer.update();
#endif // !defined(PIN_NUM_BUTTON_A) && !defined(PIN_NUM_BUTTON_B) && !defined(LCD_TOUCH)
    main_screen.update();
}