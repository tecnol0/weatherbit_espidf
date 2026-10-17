
#ifndef WEATHER_LOGIC_H
#define WEATHER_LOGIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    float temperatura;
    float humedad;
    float presion;
} weather_data_t;

bool weather_decode_packet(
    const uint8_t *packet,
    size_t length,
    weather_data_t *result
);

#endif
