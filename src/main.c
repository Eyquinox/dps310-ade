// Fixed-sized int and uint
#include <stdint.h>

#include <stdio.h>

#include "dps310.h"

// This table represents a register dump of a DPS310
// See description https://www.infineon.com/cms/en/product/sensor/pressure-sensors/pressure-sensors-for-iot/dps310/
// See datasheet https://www.infineon.com/cms/en/product/sensor/pressure-sensors/pressure-sensors-for-iot/dps310/#!?fileId=5546d462576f34750157750826c42242
// Especially §7 and §8
//
// Dumped from a real dps310 dump, after it has been initialized/started
// so temperature is ready to be read, embedded here as a C array.
// FIFO not used/not activated.
//
// Hint: Temperature to read is between 25.0 and 30.0 °C.

// Declare external DPS310 register dump in separate file fakedps310.c
extern const uint8_t dps310_regmap[];

int main(void) {
    float temperature = dps310_get_temperature_real(dps310_regmap);
    printf("Measured temperature is %.2f °C\n", temperature);
    return 0;
}
