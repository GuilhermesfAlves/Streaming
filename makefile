CC = g++
CFLAGS = -Wall

# Arquivos objeto
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = exec
MODE = none

server: MODE = server
server: run

client: MODE = client
client: run

run: all
	sudo ./$(TARGET) $(MODE) 12345
	@$(MAKE) --no-print-directory cleanobj

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean cleanobj client server run

clean: cleanobj
	@rm -f $(TARGET)

cleanobj:
	@rm -rf $(OBJ_DIR)
