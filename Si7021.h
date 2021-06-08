#ifndef PI_PICO_Si7021
#define PI_PICO_Si7021

void si7021_init_I2C(i2c_inst_t* I2C_PORT, int SDA_PIN, int SCL_PIN);

void si7021_get_sensor_type(i2c_inst_t* I2C_PORT, char *sensortype);

void si7021_get_indentifier(i2c_inst_t *I2C_PORT, uint64_t *id);

void si7021_get_firmware_rev(i2c_inst_t *I2C_PORT, int *rev);

void si7021_get_temperature(i2c_inst_t *I2C_PORT, float *temp);
/*some description*/
void si7021_get_humidity(i2c_inst_t *I2C_PORT, float *hum);

void si7021_reset (i2c_inst_t* I2C_PORT);

#endif