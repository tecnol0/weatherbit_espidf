#include "weather_logic.h"

// ---------------------------------------------------------
// Función auxiliar para leer 2 bytes (16 bits) en formato Little Endian
// ---------------------------------------------------------
static uint16_t leer_u16_le(
    const uint8_t *datos,
    size_t posicion)
{
    return (uint16_t)datos[posicion] |
           ((uint16_t)datos[posicion + 1] << 8);
}

// ---------------------------------------------------------
// Función principal para decodificar y validar el paquete I2C
// ---------------------------------------------------------
bool weather_decode_packet(
    const uint8_t *packet,
    size_t length,
    weather_data_t *result
) {
    // 1. Verificar que los punteros no sean nulos
    if (packet == NULL || result == NULL) {
        return false;
    }

    // 2. Verificar la longitud esperada (14 bytes) y el byte identificador (0xA5)
    if (length != 14 || packet[0] != 0xA5) {
        return false;
    }

    // 3. Reconstruir los valores crudos usando la función auxiliar
    // Nota: Se hace un cast a int16_t para la temperatura porque puede ser negativa.
    int16_t temp_raw = (int16_t)leer_u16_le(packet, 1);
    uint16_t humedad_raw = leer_u16_le(packet, 3);
    uint16_t presion_raw = leer_u16_le(packet, 5);

    // 4. Convertir los valores crudos a unidades físicas reales (flotantes)
    float temperatura = temp_raw / 100.0f;
    float humedad = humedad_raw / 100.0f;
    float presion = presion_raw / 10.0f;

    // 5. Validar que las mediciones estén dentro de rangos lógicos
    if (temperatura < -40.0f || temperatura > 85.0f) {
        return false;
    }
    if (humedad > 100.0f) {
        return false;
    }
    if (presion < 300.0f || presion > 1100.0f) {
        return false;
    }

    // 6. Guardar las mediciones validadas en la estructura de salida
    result->temperatura = temperatura;
    result->humedad = humedad;
    result->presion = presion;

    return true; // El paquete fue procesado con éxito
}