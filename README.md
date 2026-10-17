Estación Meteorológica ESP32 + micro:bit
1. Descripción
Este proyecto es un sistema meteorológico basado en ESP32 que recibe, valida y decodifica tramas de datos atmosféricos (Temperatura, Humedad, Presión) enviadas por una micro:bit a través del protocolo I2C.
2. Hardware
Microcontrolador ESP32
Placa BBC micro:bit
Placa de expansión SparkFun weather:bit
3. Arquitectura
El proyecto utiliza una arquitectura modular. La comunicación se realiza mediante I2C en modo esclavo. Los paquetes de 14 bytes recibidos se envían a través de colas de FreeRTOS hacia el bucle principal para su decodificación matemática usando la librería weather_logic.
4. Requisitos
ESP-IDF v6.1
Target: esp32
5. Instalación
Para clonar y preparar el proyecto:

git clone <url-del-repositorio-github>

cd weatherbit_espidf

idf.py set-target esp32

idf.py build
6. Ejecución y Conexiones
Conexión Física:

micro:bit configurado como maestro I2C.
ESP32 configurado como esclavo en la dirección 0x42.
SDA: GPIO21
SCL: GPIO22
GND: Tierra común.
Nota: Utilizar niveles lógicos de 3,3 V.

Después de compilar y conectar el hardware, graba el firmware y abre el monitor serial:

idf.py -p COM12 flash monitor

El puerto COM12 corresponde al dispositivo utilizado durante las pruebas. Debe reemplazarse por el puerto asignado al ESP32 en cada equipo.
7. Pruebas Unity
El proyecto incluye pruebas unitarias para validar la lógica matemática de decodificación. Nota importante: test_app es una aplicación separada para Unity. Su firmware reemplaza temporalmente el programa meteorológico del ESP32.

Para ejecutar las pruebas, navega a la carpeta de tests, compila y graba usando el puerto de tu equipo (ej. COM12):

cd test_app

idf.py set-target esp32

idf.py build

idf.py -p COM12 flash monitor

(Nota: set-target solo se ejecuta cuando se prepara el proyecto de pruebas por primera vez).

Resultado obtenido:

Running Decodifica mediciones correctas...

test_weather_logic.c:6:Decodifica mediciones correctas:PASS

Running Rechaza cabecera incorrecta...

test_weather_logic.c:43:Rechaza cabecera incorrecta:PASS

-----------------------

2 Tests 0 Failures 0 Ignored 

OK

Retorno al firmware principal: Una vez finalizadas las pruebas de Unity, para que la estación meteorológica vuelva a funcionar normalmente, debes regresar a la carpeta principal del proyecto y volver a flashear la aplicación:

cd ..

idf.py -p COM12 flash monitor
8. Estructura
main/: Contiene el bucle principal de la aplicación y la configuración I2C/FreeRTOS.
components/weather_logic/: Librería que encapsula la lógica de validación de cabeceras y decodificación de bytes a valores flotantes.
test_app/: Aplicación independiente para ejecutar las pruebas unitarias de Unity.
9. Licencias y atribuciones
Este proyecto utiliza licencia MIT (ver archivo LICENSE).

Firmware ESP-IDF (Este repositorio): Desarrollado en C utilizando ESP-IDF v6.1 y Unity para pruebas unitarias. Lógica de decodificación y estructura I2C propia.
Hardware y Extensiones Externas: Este proyecto está diseñado para interactuar con el módulo de hardware SparkFun weather:bit.
Firmware micro:bit (Requisito Externo): Para reproducir este montaje, la placa micro:bit debe estar programada para enviar las tramas I2C correspondientes. Este código externo (no incluido aquí) se desarrolla típicamente usando MakeCode e implementando la extensión oficial de SparkFun para el weather:bit.
