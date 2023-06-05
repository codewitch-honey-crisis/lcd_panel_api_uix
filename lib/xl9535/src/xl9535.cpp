#include <xl9535.hpp>
#define XL9535_IIC_ADDRESS 0X20

#define XL9535_INPUT_PORT_0_REG 0X00
#define XL9535_INPUT_PORT_1_REG 0X01
#define XL9535_OUTPUT_PORT_0_REG 0X02
#define XL9535_OUTPUT_PORT_1_REG 0X03
#define XL9535_INVERSION_PORT_0_REG 0X04
#define XL9535_INVERSION_PORT_1_REG 0X05
#define XL9535_CONFIG_PORT_0_REG 0X06
#define XL9535_CONFIG_PORT_1_REG 0X07
#ifdef ARDUINO
#define I2C_TYPE TwoWire&
using namespace arduino;
#else
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define I2C_TYPE i2c_port_t
using namespace esp_idf;
#endif

static uint8_t xl9535_read_reg(I2C_TYPE i2c, uint8_t address, uint8_t r) {
#ifdef ARDUINO
    i2c.beginTransmission(address);
    i2c.write(r);
    i2c.endTransmission();
    if(1>i2c.requestFrom(address, (uint8_t)1)) {
        return 0;
    }
    uint8_t result = i2c.read();
    return result;
#else
    uint8_t result = 0;
    i2c_master_write_read_device(i2c, address, &r, 1, &result, 1, pdTICKS_TO_MS(1000));
    return result;
#endif
}
static void xl9535_write_reg(I2C_TYPE i2c, uint8_t address, uint8_t r, const uint8_t value) {
#ifdef ARDUINO
    i2c.beginTransmission(address);
    i2c.write(r);
    i2c.write(value);
    i2c.endTransmission();
#else
    uint8_t write_buf[2] = {r, value};
    i2c_master_write_to_device(i2c, address, write_buf, sizeof(write_buf), pdMS_TO_TICKS(1000));
#endif
}

void xl9535::do_move(xl9535& rhs) {
    m_i2c = rhs.m_i2c;
    m_address = rhs.m_address;
}
xl9535::xl9535(
#ifdef ARDUINO
    TwoWire& i2c
#else
    i2c_port_t i2c
#endif
    ,
    uint8_t address) : m_i2c(i2c), m_address(address) {
}
xl9535::xl9535(xl9535&& rhs) : m_i2c(rhs.m_i2c) {
    do_move(rhs);
}
xl9535& xl9535::operator=(xl9535&& rhs) {
    do_move(rhs);
    return *this;
}
void xl9535::mode(uint8_t pin, xl9535_pin_mode mode) {
    uint8_t port;
    if (pin > 7) {
        port = xl9535_read_reg(m_i2c, m_address, XL9535_CONFIG_PORT_1_REG);
        if (mode == xl9535_pin_mode::output) {
            port = port & (~(1 << (pin - 10)));
        } else {
            port = port | (1 << (pin - 10));
        }
        xl9535_write_reg(m_i2c, m_address, XL9535_CONFIG_PORT_1_REG, port);
    } else {
        port = xl9535_read_reg(m_i2c, m_address, XL9535_CONFIG_PORT_0_REG);
        if (mode == xl9535_pin_mode::output) {
            port = port & (~(1 << pin));
        } else {
            port = port | (1 << pin);
        }
        xl9535_write_reg(m_i2c, m_address, XL9535_CONFIG_PORT_0_REG, port);
    }
}
void xl9535::mode_mask_lo(uint8_t pin_mask, xl9535_pin_mode mode) {
    uint8_t mask = (mode != xl9535_pin_mode::output) ? pin_mask : ~pin_mask;
      xl9535_write_reg(m_i2c,m_address,XL9535_CONFIG_PORT_0_REG, mask);
}
void xl9535::mode_mask_hi(uint8_t pin_mask, xl9535_pin_mode mode) {
    uint8_t mask = (mode != xl9535_pin_mode::output) ? pin_mask : ~pin_mask;
      xl9535_write_reg(m_i2c,m_address,XL9535_CONFIG_PORT_1_REG, mask);
}
bool xl9535::pin(uint8_t pin) const {
    int state = 0;
    uint8_t port = 0;
    if (pin > 7) {
        port = xl9535_read_reg(m_i2c,m_address, XL9535_INPUT_PORT_1_REG);
        state = port & (pin - 10) ? 1 : 0;
    } else {
        port = xl9535_read_reg(m_i2c,m_address, XL9535_INPUT_PORT_0_REG);
        state = port & pin ? 1 : 0;
    }
    return state;
}
void xl9535::pin(uint8_t pin, bool value) {
    uint8_t port = 0;
    uint8_t reg_data = 0;
    if (pin > 7) {
        reg_data = xl9535_read_reg(m_i2c,m_address,XL9535_OUTPUT_PORT_1_REG);
        reg_data = reg_data & (~(1 << (pin - 10)));
        port = reg_data | value << (pin - 10);
        xl9535_write_reg(m_i2c,m_address,XL9535_OUTPUT_PORT_1_REG, port);
    } else {
        reg_data = xl9535_read_reg(m_i2c,m_address,XL9535_OUTPUT_PORT_0_REG);
        reg_data = reg_data & (~(1 << pin));
        port = reg_data | value << pin;
        xl9535_write_reg(m_i2c,m_address,XL9535_OUTPUT_PORT_0_REG, port);
    }
}