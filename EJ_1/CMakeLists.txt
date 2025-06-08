cmake_minimum_required(VERSION 3.10)
project(Pokemons LANGUAGES CXX)

# Indicamos que use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1) Buscamos todos los .cpp dentro de src/
file(GLOB SRC_FILES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# 2) Creamos el ejecutable antes de configurar sus includes
add_executable(Pokemons
    ${SRC_FILES}
)

# 3) Ahora que 'Pokemons' ya existe, agregamos su carpeta de headers
#    Esto hará que, cuando en poke.cpp se ponga #include "poke.hpp", 
#    CMake busque en <ruta-a-EJ_1>/include
target_include_directories(Pokemons
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 4) (Opcional) Hacer que el binario se genere en bin/ dentro de este directorio
set_target_properties(Pokemons PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)
