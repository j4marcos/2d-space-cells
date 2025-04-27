# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Nome do executável
TARGET = $(BIN_DIR)/SudokuValidator

# Regras principais
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar os arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regra para rodar o programa
run: all
	./$(TARGET)

# Declaração de phony targets
.PHONY: all clean run