cmake_minimum_required(VERSION 3.15.3)
project(lantern_controller)

# Options
################################################################################

set(RUNTIME_OUTPUT_NAME_DEBUG   ${CMAKE_BINARY_DIR}/bin/Debug)
set(RUNTIME_OUTPUT_NAME_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake/PahoMqttCpp/")

# Definitions
################################################################################
set(SRC_FILES 
"src/comms/mqtt/mqtt_client.hpp"
"src/comms/mqtt/mqtt_client.cpp"
"src/effects/effect.hpp"
"src/effects/effect.cpp"
"src/effects/lightning.hpp"
"src/effects/lightning.cpp"
"src/effects/solid.hpp"
"src/effects/solid.cpp"
"src/apa102c.hpp"
"src/apa102c.cpp"
"src/color.hpp"
"src/color.cpp"
"src/control_packet.hpp"
"src/effects.hpp"
"src/kkbb01.hpp"
"src/kkbb01.cpp"
"src/kklp01.hpp"
"src/kklp01.cpp"
"src/main.cpp"
)

# Build Definition
################################################################################
add_executable(lantern_control ${SRC_FILES})

target_compile_options(lantern_control PRIVATE -std=c++17 -Wall -g -Werror)
target_link_options(lantern_control PRIVATE -lwiringPi)

# Dependiencies
################################################################################

# System packages 
find_package(Threads REQUIRED)
#find_package(WiringPi REQUIRED)
find_package(PahoMqttC)


include_directories(${WIRINGPI_INCLUDE_DIRS})

#target_link_libraries(lantern_control PRIVATE ${WIRINGPI_LIBRARIES}) 
target_link_libraries(lantern_control PRIVATE ${CMAKE_THREAD_LIBS_INIT}) 
target_link_libraries(lantern_control PRIVATE paho-mqttpp3 ${PAHO_MQTT_C_LIBRARIES})

# Third-party packages (git subtrees)
# rapidjson
add_subdirectory(external/rapidjson)

#find_package(RapidJSON REQUIRED)
target_include_directories(lantern_control PUBLIC ${RapidJSON_INCLUDE_DIRS})

# Import Boost Libraries
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost REQUIRED COMPONENTS log)

target_link_libraries(lantern_control PRIVATE Boost::log)

message(STATUS "Boost version: ${Boost_VERSION}")

# Testing
################################################################################

# Deployment
################################################################################