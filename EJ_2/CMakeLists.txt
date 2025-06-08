cmake_minimum_required(VERSION 3.10)
project(Drones LANGUAGES CXX)

# Indicamos que use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1) Buscamos todos los .cpp dentro de src/
file(GLOB SRC_FILES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# 2) Creamos el ejecutable antes de configurar sus includes
add_executable(Drones
    ${SRC_FILES}
)

# 3) (Opcional) Hacer que el binario se genere en bin/ dentro de este directorio
set_target_properties(Drones PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)
