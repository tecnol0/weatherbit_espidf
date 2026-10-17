cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS "../components")

set(TEST_COMPONENTS "weather_logic" CACHE STRING "Components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(weatherbit_tests)
