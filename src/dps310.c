#include <stdint.h>
#include "dps310.h"

// Known registers addresses
enum RegOffset_e 
{
    REG_PRES        = 0x00,
    REG_TEMP        = 0x03,
    REG_PRS_CFG     = 0x06,
    REG_TMP_CFG     = 0x07,
    REG_MEAS_CFG    = 0x08,
    REG_CFG_REG     = 0x09,
    REG_RESET       = 0x0C,
    REG_COEF1       = 0x10,  // C0 MSB
    REG_COEF2       = 0x11,  // C0 LSB (high 4b), C1 MSB (low 4b)
    REG_COEF3       = 0x12,  // C1 LSB
    REG_COEF4       = 0x13,
    REG_COEF5       = 0x14,
    REG_COEF6       = 0x15,
    REG_COEF7       = 0x16,
    REG_COEF8       = 0x17,
    REG_COEF9       = 0x18,
    REG_COEF10      = 0x19,
    REG_COEF11      = 0x1A,
    REG_COEF12      = 0x1B,
    REG_COEF13      = 0x1C,
    REG_COEF14      = 0x1D,
    REG_COEF15      = 0x1E,
    REG_COEF16      = 0x1F,
    REG_COEF17      = 0x20,
    REG_COEF18      = 0x21,
    REG_TMP_COEF_SRCE = 0x28
};

// Over Sampling configuration options
enum OversamplingRate_e
{
    OSR_SINGLE                     = 0x00,
    OSR_TWICE                      = 0x01,
    OSR_FOUR_TIMES                 = 0x02,
    OSR_EIGHT_TIMES                = 0x03,
    OSR_SIXTEEN_TIMES              = 0x04,
    OSR_THIRTY_TWO_TIMES           = 0x05,
    OSR_SIXTY_FOUR_TIMES           = 0x06,
    OSR_HUNDRED_TWENTY_EIGHT_TIMES = 0x07
};

// kT/kP compensation scale factors (see datasheet §7.3 Table 9)
static const uint32_t _scaleFactors[] = {
    524288,
    1572864,
    3670016,
    7864320,
    253952,
    516096,
    1040384,
    2088960
};

// Generic 2's complement sign-extension from 'bits'-bit to 32-bit int
static int32_t _signed_to_signed32(int32_t v, int bits) {
    return (v << (32 - bits)) >> (32 - bits);
}

// Read a signed 24-bit value (big-endian) and return as signed 32-bit
int32_t _get_s24_at(const uint8_t *regmap, int offset) {
    uint32_t raw = ((uint32_t)regmap[offset]   << 16)
                 | ((uint32_t)regmap[offset+1] <<  8)
                 |  (uint32_t)regmap[offset+2];
    return _signed_to_signed32(raw, 24);
}

// Return raw TMP (which is a signed 24 bits spread onto 3 bytes) as a signed 32-bit
// caller provides dps310_regmap as an argument during the call for func to peek registers
int32_t dps310_get_temperature_raw(const uint8_t *regmap) {
    return _get_s24_at(regmap, REG_TEMP);
}

// Compute real temperature in °C based on datasheet §4.9.2
float dps310_get_temperature_real(const uint8_t *regmap) {
    // Extract coefficients C0 and C1 (12-bit signed each)
    int32_t c0_u = ((int32_t)regmap[REG_COEF1] << 4) | (regmap[REG_COEF2] >> 4);
    int32_t c0   = _signed_to_signed32(c0_u, 12);
    int32_t c1_u = ((regmap[REG_COEF2] & 0x0F) << 8) | regmap[REG_COEF3];
    int32_t c1   = _signed_to_signed32(c1_u, 12);

    // Read TMP_PRC to select scale factor kT
    uint8_t prc = regmap[REG_TMP_CFG] & 0x07;
    uint32_t kT  = _scaleFactors[prc];

    // Read raw temperature
    int32_t t_raw = dps310_get_temperature_raw(regmap);

    // T = c0*0.5 + c1*(t_raw/kT)
    return (float)(c0 * 0.5f + c1 * ((float)t_raw / (float)kT));
}
