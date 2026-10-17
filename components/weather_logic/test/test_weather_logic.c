#include <stdint.h>
#include "unity.h"
#include "weather_logic.h"

// --- PRUEBA 1: DECODIFICACIÓN CORRECTA ---
TEST_CASE("Decodifica mediciones correctas", "[weather_logic]")
{
    // Paquete artificial de 14 bytes
    const uint8_t paquete[14] = {
        0xA5,
        0x34, 0x09,  // Temperatura: 23.56 C
        0x18, 0x17,  // Humedad: 59.12 %
        0x65, 0x1D,  // Presion: 752.5 hPa
        0, 0, 0, 0, 0, 0, 0
    };

    weather_data_t lectura = {0};

    // Ejecutar el decodificador
    bool resultado = weather_decode_packet(
        paquete,
        sizeof(paquete),
        &lectura
    );

    // Verificar el resultado
    TEST_ASSERT_TRUE(resultado);

    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 23.56f, lectura.temperatura
    );

    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 59.12f, lectura.humedad
    );

    TEST_ASSERT_FLOAT_WITHIN(
        0.01f, 752.5f, lectura.presion
    );
}

// --- PRUEBA 2: RECHAZO DE CABECERA INCORRECTA ---
TEST_CASE("Rechaza cabecera incorrecta", "[weather_logic]")
{
    const uint8_t paquete[14] = {
        0x00,           // Cabecera incorrecta
        0x34, 0x09,     // Temperatura
        0x18, 0x17,     // Humedad
        0x65, 0x1D,     // Presion
        0, 0, 0, 0, 0, 0, 0
    };

    weather_data_t lectura = {0};

    bool resultado = weather_decode_packet(
        paquete,
        sizeof(paquete),
        &lectura
    );

    TEST_ASSERT_FALSE(resultado);
}