#ifndef DPS310_H
#define DPS310_H

#include <stdint.h>

// Read raw temperature (signed 24 bits) from DPS310 register map
int32_t dps310_get_temperature_raw(const uint8_t *regmap);

// Compute compensated temperature in °C using DPS310 register map
float dps310_get_temperature_real(const uint8_t *regmap);

#endif // DPS310_H
