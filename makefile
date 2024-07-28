CC = g++
CFLAGS = -Wall

# Arquivos objeto
SRC_DIR = src
OBJ_DIR = obj
DATA_DIR = cathalogC
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = exec
LOGGERS = *.log

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(MODE)

.PHONY: clean cleanobj cleandata

clean: cleanobj cleandata
	@rm -f $(TARGET) $(LOGGERS)

cleanobj:
	@rm -rf $(OBJ_DIR)

cleandata:
	@rm -rf $(DATA_DIR)/*