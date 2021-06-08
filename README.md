
# Pi Pico C/C++ Adafruit Si7021 Library.

## Description

A simple library to use the Adafruit Si7021 Temperature & Humidity Sensor Breakout Board with the Raspberry Pi Pico RP2040 board with the C/C++ sdk.

## Getting Started
An example using the library is below.
```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "si7021.h"

int main()
{
    stdio_init_all();
    si7021_init_I2C(i2c1, 2, 3);
    while (true)
    {
        char sensorType[64] = {0};
        uint64_t id;
        int firmwarerev;
        float temperature;
        float humidity;
        si7021_get_sensor_type(i2c1, sensorType);
        si7021_get_indentifier(i2c1, &id);
        si7021_get_firmware_rev(i2c1, &firmwarerev);
        si7021_get_temperature(i2c1, &temperature);
        si7021_get_humidity(i2c1, &humidity);
        printf("Sensor type: %s \n", sensorType);
        printf("Serial: %" PRIu64 "\n", id);
        printf("Firmware: %d\n", firmwarerev);
        printf("Temperature: %fC\n", temperature);
        printf("Humidity: %f%%\n", humidity);
        si7021_reset(i2c1);
        sleep_ms(5000);
    }
    return 0;
}
```


## Authors

Joe Willson

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the MIT License License - see the LICENSE file for details

## Acknowldements and references

### Sensor datasheet
* [Si7021 datasheet](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf)  

### Adafruit Si7021 Arduino Library  
  
This is a library for the Adafruit Si7021 breakout board.
 
 Designed specifically to work with the Adafruit Si7021 breakout board.

Pick one up today in the adafruit shop!
https://www.adafruit.com/product/3251

These sensors use I2C to communicate, 2 pins are required to interface.

Adafruit invests time and resources providing this open source code,
please support Adafruit andopen-source hardware by purchasing products
from Adafruit!

Limor Fried (Adafruit Industries)

BSD license, all text above must be included in any redistribution

* [Adafruit Si7021 Arduino Library](https://github.com/adafruit/Adafruit_Si7021)

### Raspberry Pi Pico SDK

* [raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)

Also see Third Part Notices.txt