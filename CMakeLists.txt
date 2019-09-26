cmake_minimum_required(VERSION 3.0)
project(lantern_controller C CXX)

# Options
################################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/PahoMqttCpp/")


# Source Files
################################################################################

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

# target definitions
################################################################################

add_executable(lantern_control ${SRC_FILES})

target_compile_options(lantern_control PUBLIC -Wall -g -Werror)

target_include_directories(lantern_control PUBLIC src/main)

# Dependiencies
################################################################################
# Locate libraries and headers
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)
find_package(PahoMqttC)
FIND_PACKAGE(Boost REQUIRED COMPONENTS log)

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS}) 
include_directories(${Boost_INCLUDE_DIRS})

# Link against libraries
target_link_libraries(lantern_control ${WIRINGPI_LIBRARIES}) 
target_link_libraries(lantern_control ${CMAKE_THREAD_LIBS_INIT}) 
target_link_libraries(lantern_control paho-mqttpp3 ${PAHO_MQTT_C_LIBRARIES})
target_link_libraries(lantern_control ${Boost_LIBRARIES})

# Testing
################################################################################

# Deployment
################################################################################