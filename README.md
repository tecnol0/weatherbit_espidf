# Estación Meteorológica ESP32 + micro:bit

## 1. Descripción
Este proyecto es un sistema meteorológico basado en ESP32 que recibe, valida y decodifica tramas de datos atmosféricos (Temperatura, Humedad, Presión) enviadas por una micro:bit a través del protocolo I2C.

## 2. Hardware
* Microcontrolador ESP32
* Placa BBC micro:bit
* Placa de expansión SparkFun weather:bit

## 3. Arquitectura
El proyecto utiliza una arquitectura modular. La comunicación se realiza mediante I2C en modo esclavo. Los paquetes de 14 bytes recibidos se envían a través de colas de FreeRTOS hacia el bucle principal para su decodificación matemática usando la librería `weather_logic`.

## 4. Requisitos
* ESP-IDF v6.1
* Target: `esp32`

## 5. Instalación
Para clonar y preparar el proyecto:
```bash
git clone <url-del-repositorio>
cd weatherbit_espidf
idf.py set-target esp32
idf.py build