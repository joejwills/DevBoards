#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "si7021.h"

uint8_t const SI7021_ADDRESS = 0x40;
uint8_t const SI7021_TEMP_CMD = 0xF3;
uint8_t const SI7021_RELHUM_CMD = 0xF5;
uint8_t const SI7021_RESET_CMD = 0xFE;
uint8_t const SI7021_ID1_CMD[] = {0xFA, 0x0F};
uint8_t const SI7021_ID2_CMD[] = {0xFC, 0xC9};
uint8_t const SI7021_FIRM_REV_CMD[] = {0x84, 0xB8};
uint32_t const SI7021_I2C_WAIT_TIME = 25;

void si7021_uint8_to_uint16(uint8_t *data, uint16_t *sixteen, int count)
{
    for (int i = 0; i < count; i += 2)
    {
        sixteen[i / 2] = data[i + 1] | (uint16_t)data[i] << 8;
    }
}

void si7021_bytes_to_serial(const uint8_t bytes[8], uint64_t *id)
{
    *id = bytes[0];
    *id = *id << 8;
    *id = *id | bytes[1];
    *id = *id << 8;
    *id = *id | bytes[2];
    *id = *id << 8;
    *id = *id | bytes[3];
    *id = *id << 8;
    *id = *id | bytes[4];
    *id = *id << 8;
    *id = *id | bytes[5];
    *id = *id << 8;
    *id = *id | bytes[6];
    *id = *id << 8;
    *id = *id | bytes[7];
}

void si7021_init_I2C(i2c_inst_t *I2C_PORT, int SDA_PIN, int SCL_PIN)
{
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void si7021_get_sensor_type(i2c_inst_t *I2C_PORT, char *sensortype)
{
    uint8_t id1[8];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, SI7021_ID1_CMD, 2, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, id1, 8, false);

    uint8_t id2[6];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, SI7021_ID2_CMD, 2, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, id2, 6, false);

    switch (id2[0])
    {
    case 0x00:
    case 0xFF:
        strcpy(sensortype, "engineering sample");
        break;
    case 0x0D:
        strcpy(sensortype, "Si7013");
        break;
    case 0x14:
        strcpy(sensortype, "Si7020");
        break;
    case 0x15:
        strcpy(sensortype, "Si7021");
        break;
    default:
        strcpy(sensortype, "unknown");
        break;
    };
}

void si7021_get_indentifier(i2c_inst_t *I2C_PORT, uint64_t *id)
{
    uint8_t id1[8];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, SI7021_ID1_CMD, 2, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, id1, 8, false);

    uint8_t id2[6];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, SI7021_ID2_CMD, 2, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, id2, 6, false);

    uint8_t serial_bytes[8] = {id1[0],
                               id1[2],
                               id1[4],
                               id1[6],
                               id2[0],
                               id2[1],
                               id2[3],
                               id2[4]};
    si7021_bytes_to_serial(serial_bytes, id);
}

void si7021_get_firmware_rev(i2c_inst_t *I2C_PORT, int *revision)
{
    uint8_t rev[1];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, SI7021_FIRM_REV_CMD, 2, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, rev, 1, false);
    switch (rev[0])
    {
    case 0xFF:
        *revision = 1;
        break;
    case 0x20:
        *revision = 2;
        break;
    default:
        *revision = 0;
        break;
    }
}

void si7021_get_temperature(i2c_inst_t *I2C_PORT, float *temp)
{
    uint8_t data[4];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, &SI7021_TEMP_CMD, 1, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, SI7021_ADDRESS, data, 4, false);
    uint16_t sixteen[1];
    si7021_uint8_to_uint16(data, sixteen, 4);
    float temperature = sixteen[0];
    temperature *= 175.72;
    temperature /= 65536;
    temperature -= 46.85;
    *temp = temperature;
}

void si7021_get_humidity(i2c_inst_t *I2C_PORT, float *hum)
{
    uint8_t data[4];
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, &SI7021_TEMP_CMD, 1, false);
    sleep_ms(SI7021_I2C_WAIT_TIME);
    i2c_read_blocking(I2C_PORT, 0x40, data, 4, false);
    uint16_t sixteen[1];
    si7021_uint8_to_uint16(data, sixteen, 4);
    float humidity = sixteen[0];
    humidity *= 125;
    humidity /= 65536;
    humidity -= 6;
    *hum = humidity;
}

void si7021_reset(i2c_inst_t *I2C_PORT)
{
    i2c_write_blocking(I2C_PORT, SI7021_ADDRESS, &SI7021_RESET_CMD, 1, false);
    sleep_ms(25);
}