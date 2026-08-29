cmake_minimum_required(VERSION 3.19)
project(gas-compositor VERSION 1.0.0 LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)

function(config_target target)
    target_include_directories(${target} PRIVATE include)
    target_compile_features(${target} PRIVATE c_std_11)
    set_target_properties(${target} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endfunction()

add_library(data_structures SHARED
    src/data_structures/ring.c
)
config_target(data_structures)

add_library(input SHARED
    src/input/devices.c
    src/input/keys.c
)
config_target(input)

add_library(config SHARED
    src/config/lua.c
    src/config/keys.c
)
config_target(config)
target_include_directories(config PUBLIC ${LUA_INCLUDE_DIRS})
target_link_libraries(config PUBLIC ${LUA_LIBRARIES})

add_executable(compositor
    src/main.c
)
config_target(compositor)
target_link_libraries(compositor PRIVATE config)
