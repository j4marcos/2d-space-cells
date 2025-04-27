# Nome do executável
TARGET = simulacao

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Fontes
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Objetos
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Flags de compilação
CFLAGS = -Wall -I$(INC_DIR) -pthread

# Flags de link
LDFLAGS = -pthread -lm

# Compilador
CC = gcc

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar tudo
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
