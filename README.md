# Estación Meteorológica ESP32 + micro:bit

## 1. Descripción

Este proyecto es un sistema meteorológico basado en ESP32 que recibe, valida y decodifica tramas de datos atmosféricos (temperatura, humedad y presión) enviadas por una micro:bit a través del protocolo I2C.

## 2. Hardware

- Microcontrolador ESP32.
- Placa BBC micro:bit.
- Placa de expansión SparkFun weather:bit.

## 3. Arquitectura

El proyecto utiliza una arquitectura modular. La comunicación se realiza mediante I2C, con el ESP32 configurado como esclavo. Los paquetes de 14 bytes recibidos se envían a través de colas de FreeRTOS hacia el bucle principal para su decodificación matemática usando la librería `weather_logic`.

## 4. Requisitos

- ESP-IDF v6.1.
- Target: `esp32`.

## 5. Instalación

Para clonar y preparar el proyecto:

```bash
git clone <url-del-repositorio-github>
cd weatherbit_espidf
idf.py set-target esp32
idf.py build
```

**Nota:** Reemplaza `<url-del-repositorio-github>` por la URL real del repositorio antes de ejecutar el comando `git clone`.

## 6. Ejecución y conexiones

**Conexión física:**

- micro:bit configurada como maestro I2C.
- ESP32 configurado como esclavo I2C en la dirección `0x42`.
- SDA: GPIO21.
- SCL: GPIO22.
- GND: tierra común.

**Nota:** Utilizar niveles lógicos de 3,3 V.

Después de compilar y conectar el hardware, graba el firmware y abre el monitor serial:

```bash
idf.py -p COM12 flash monitor
```

El puerto `COM12` corresponde al dispositivo utilizado durante las pruebas. Debe reemplazarse por el puerto asignado al ESP32 en cada equipo.

## 7. Pruebas Unity

El proyecto incluye pruebas unitarias para validar la lógica matemática de decodificación.

**Importante:** `test_app` es una aplicación separada para Unity. Su firmware reemplaza temporalmente el programa meteorológico del ESP32.

Para ejecutar las pruebas, navega a la carpeta de pruebas, compila y graba usando el puerto de tu equipo (por ejemplo, `COM12`):

```bash
cd test_app
idf.py set-target esp32
idf.py build
idf.py -p COM12 flash monitor
```

**Nota:** `idf.py set-target esp32` solo se necesita al preparar el proyecto de pruebas por primera vez, salvo que cambies su configuración de destino.

**Resultado obtenido:**

```text
Running Decodifica mediciones correctas...
test_weather_logic.c:6:Decodifica mediciones correctas:PASS

Running Rechaza cabecera incorrecta...
test_weather_logic.c:43:Rechaza cabecera incorrecta:PASS

-----------------------
2 Tests 0 Failures 0 Ignored

OK
```

**Retorno al firmware principal:** Una vez finalizadas las pruebas de Unity, para que la estación meteorológica vuelva a funcionar normalmente, debes regresar a la carpeta principal del proyecto y volver a grabar la aplicación:

```bash
cd ..
idf.py -p COM12 flash monitor
```

## 8. Estructura

- `main/`: contiene el bucle principal de la aplicación y la configuración I2C/FreeRTOS.
- `components/weather_logic/`: librería que encapsula la lógica de validación de cabeceras y decodificación de bytes a valores flotantes.
- `test_app/`: aplicación independiente para ejecutar las pruebas unitarias de Unity.

## 9. Licencias y atribuciones

Este proyecto utiliza la licencia MIT (ver archivo `LICENSE`).

- **Firmware ESP-IDF (este repositorio):** desarrollado en C utilizando ESP-IDF v6.1 y Unity para pruebas unitarias. Lógica de decodificación y estructura I2C propia.
- **Hardware y extensiones externas:** este proyecto está diseñado para interactuar con el módulo de hardware SparkFun weather:bit.
- **Firmware micro:bit (requisito externo):** para reproducir este montaje, la placa micro:bit debe estar programada para enviar las tramas I2C correspondientes. Este código externo (no incluido aquí) se desarrolla típicamente usando MakeCode e implementando la extensión oficial de SparkFun para weather:bit.