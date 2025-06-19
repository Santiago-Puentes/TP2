# Compilador
CXX := g++

# Opciones de compilación
CXXFLAGS := -std=c++23 -Iinclude/. -Wall -Wextra -Wpedantic -Werror \
           -Wconversion -Wsign-compare -Wshadow -Wpointer-arith \
           -Wfloat-equal -Wcast-align -Wformat -Wmissing-declarations \
           -Wunreachable-code -Wundef -Winline -Wunused \
           -Wnon-virtual-dtor -Wduplicated-cond -Wduplicated-branches \
           -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wuseless-cast \
           -Wzero-as-null-pointer-constant

# Nombre del ejecutable
TARGET := programm

# Archivos fuente (cpp)
SRC_FILES := $(wildcard src/*.cpp) # Todos los cpp en src
SRC_FILES += $(wildcard *.cpp)  # Todos los archivos .cpp en el directorio

# Archivos objeto (o)
OBJ_FILES := $(SRC_FILES:.cpp=.o)  # Convierte .cpp a .o para los archivos objeto

# Regla principal
all: $(TARGET)
	@echo "Compilación completada."
	@echo "Ejecutando el programa..."
	./$(TARGET)
	@echo "Ejecutando limpieza..."
	$(MAKE) clean

# Limpiar archivos generados
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Generar ejecutable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generar objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean