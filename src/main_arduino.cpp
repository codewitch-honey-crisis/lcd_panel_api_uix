#ifndef ESP_IDF
namespace arduino {}
namespace esp_idf {}
using namespace arduino;
using namespace esp_idf;
#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "lcd_config.h"
#define LCD_IMPLEMENTATION
#include "lcd_init.h"
#include <gfx.hpp>
using namespace gfx;
#include <uix.hpp>
using namespace uix;
// SVG converted to header using
// https://honeythecodewitch.com/gfx/converter
#include "bee_icon.hpp"
static const_buffer_stream& svg_stream = bee_icon;
// downloaded from fontsquirrel.com and header generated with
// https://honeythecodewitch.com/gfx/generator
//#include "fonts/Rubik_Black.hpp"
//#include "fonts/Telegrama.hpp"
#include "fonts/OpenSans_Regular.hpp"
static const open_font& text_font = OpenSans_Regular;

#ifdef LCD_TOUCH
void svg_touch();
void svg_release();
#endif // LCD_TOUCH

// declare a custom control
template <typename PixelType, 
        typename PaletteType = gfx::palette<PixelType, PixelType>>
class svg_box_touch : public svg_box<PixelType, PaletteType> {
   // public and private type aliases
   // pixel_type and palette_type are 
   // required on any control
   public:
    using type = svg_box_touch;
    using pixel_type = PixelType;
    using palette_type = PaletteType;
   private:
    using base_type = svg_box<PixelType, PaletteType>;
    using control_type = control<PixelType, PaletteType>;
    using control_surface_type = typename control_type::control_surface_type;
     public:
    svg_box_touch(invalidation_tracker& parent, 
                const palette_type* palette = nullptr) 
                : base_type(parent, palette) {

    }
#ifdef LCD_TOUCH
    virtual bool on_touch(size_t locations_size,
                        const spoint16* locations) {
        svg_touch();     
        return true;
    }
    virtual void on_release() {
        svg_release();
    }
#endif // LCD_TOUCH
};

// declare the format of the screen
using screen_t = screen<LCD_WIDTH, LCD_HEIGHT, rgb_pixel<16>>;
// declare the control types to match the screen
#ifdef LCD_TOUCH
// since this supports touch, we use an interactive button
// instead of a static label
using label_t = push_button<typename screen_t::pixel_type>;
#else
using label_t = label<typename screen_t::pixel_type>;
#endif // LCD_TOUCH
using svg_box_t = svg_box_touch<typename screen_t::pixel_type>;
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
// if we have no inputs, declare
// a timer
#if !defined(PIN_NUM_BUTTON_A) && \
    !defined(PIN_NUM_BUTTON_B) && \
    !defined(LCD_TOUCH)
using cycle_timer_t = uix::timer;
#endif // !defined(PIN_NUM_BUTTON_A) ...

// UIX allows you to use two buffers for maximum DMA efficiency
// you don't have to, but performance is significantly better
// declare 64KB across two buffers for transfer
// RGB mode is the exception. We don't need two buffers
// because the display works differently.
#ifndef LCD_PIN_NUM_HSYNC
constexpr static const int lcd_buffer_size 
                            = 32 * 1024;
uint8_t lcd_buffer1[lcd_buffer_size];
uint8_t lcd_buffer2[lcd_buffer_size];
#else
constexpr static const int lcd_buffer_size 
                            = 64 * 1024;
uint8_t lcd_buffer1[lcd_buffer_size];
uint8_t* lcd_buffer2=nullptr;
#endif // !LCD_PIN_VSYNC
// our svg doc for svg_box
svg_doc doc;
// the main screen
screen_t main_screen(sizeof(lcd_buffer1), 
                    lcd_buffer1, 
                    lcd_buffer2);
// the controls
label_t test_label(main_screen);
svg_box_t test_svg(main_screen);
#ifdef PIN_NUM_BUTTON_A
button_a_t button_a;
#endif
#ifdef PIN_NUM_BUTTON_B
button_b_t button_b;
#endif
#ifdef EXTRA_DECLS
EXTRA_DECLS
#endif // EXTRA_DECLS

// button callbacks
#ifdef PIN_NUM_BUTTON_A
void button_a_on_click(bool pressed, void* state) {
    if (pressed) {
        test_label.text_color(color32_t::red);
    } else {
        test_label.text_color(color32_t::blue);
    }
}
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
void button_b_on_click(bool pressed, void* state) {
    if (pressed) {
        main_screen.background_color(color16_t::light_green);
    } else {
        main_screen.background_color(color16_t::white);
    }
}
#endif // PIN_NUM_BUTTON_B

// no inputs
#if !defined(PIN_NUM_BUTTON_A) && \
    !defined(PIN_NUM_BUTTON_B) && \
    !defined(LCD_TOUCH)
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
#endif // !defined(PIN_NUM_BUTTON_A) ...

// touch inputs
#ifdef LCD_TOUCH
void svg_touch() {
    main_screen.background_color(color16_t::light_green);
}
void svg_release() {
    main_screen.background_color(color16_t::white);
}
#endif // LCD_TOUCH

#ifdef LCD_TOUCH
static void uix_touch(point16* out_locations, 
                    size_t* in_out_locations_size, 
                    void* state) {
    if(in_out_locations_size<=0) {
        *in_out_locations_size=0;
        return;
    }
#ifdef LCD_TOUCH_IMPL
LCD_TOUCH_IMPL
#else
    in_out_locations_size = 0;
    return;
#endif // LCD_TOUCH_IMPL
}
#endif // LCD_TOUCH

#ifndef LCD_PIN_NUM_HSYNC
// not used in RGB mode
// tell UIX the DMA transfer is complete
static bool lcd_flush_ready(esp_lcd_panel_io_handle_t panel_io, 
                            esp_lcd_panel_io_event_data_t* edata, 
                            void* user_ctx) {
    main_screen.set_flush_complete();
    return true;
}
#endif // LCD_PIN_NUM_HSYNC

// tell the lcd panel api to transfer the display data
static void uix_flush(point16 location, 
                    typename screen_t::bitmap_type& bmp, 
                    void* state) {
    int x1 = location.x, 
        y1 = location.y, 
        x2 = location.x + bmp.dimensions().width-1, 
        y2 = location.y + bmp.dimensions().height-1;
    lcd_panel_draw_bitmap( x1, y1, x2, y2, bmp.begin());    

    // if we're in RGB mode:
#ifdef LCD_PIN_NUM_HSYNC
    // flushes are immediate in rgb mode
    main_screen.set_flush_complete();
#endif // LCD_PIN_NUM_HSYNC
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
    test_svg.bounds(srect16(spoint16(0, test_label.bounds().y2+1), 
                    ssize16(60,60)).center_horizontal(main_screen.bounds()));
    gfx_result res = svg_doc::read(&svg_stream, &doc);
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
#ifdef I2C_INIT
I2C_INIT
#endif
#ifdef EXTRA_INIT
EXTRA_INIT
#endif //EXTRA_INIT

    // RGB mode uses a slightly different call:
#ifdef LCD_PIN_NUM_HSYNC
    lcd_panel_init();
#else
    lcd_panel_init(sizeof(lcd_buffer1),lcd_flush_ready);
#endif // LCD_PIN_NUM_HSYNC
    screen_init();
#ifdef PIN_NUM_BUTTON_A
    button_a.initialize();
    button_a.on_pressed_changed(button_a_on_click);
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
    button_b.initialize();
    button_b.on_pressed_changed(button_b_on_click);
#endif // PIN_NUM_BUTTON_B
}
// keep our stuff up to date and responsive
void loop() {
#ifdef PIN_NUM_BUTTON_A
    button_a.update();
#endif // PIN_NUM_BUTTON_A
#ifdef PIN_NUM_BUTTON_B
    button_b.update();
#endif // PIN_NUM_BUTTON_B
#if !defined(PIN_NUM_BUTTON_A) && \
    !defined(PIN_NUM_BUTTON_B) && \
    !defined(LCD_TOUCH)
    cycle_timer.update();
#endif // !defined(PIN_NUM_BUTTON_A) ...
    main_screen.update();
    vTaskDelay(pdMS_TO_TICKS(1));
}
#endif