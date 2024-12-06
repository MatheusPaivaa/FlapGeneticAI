# Diretórios
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Nome do arquivo executável
EXEC = $(BIN_DIR)/flappy_bird

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Arquivos fonte e objetos
SOURCES = main.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# Regras para o Makefile

# Regra padrão: compila o programa
all: $(EXEC)

# Cria o diretório de objetos se necessário
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Cria o diretório bin se necessário
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compilar o arquivo .cpp em .o
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linkar os arquivos objetos para gerar o executável
$(EXEC): $(OBJ_DIR) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Limpar arquivos de compilação
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Forçar uma recompilação dos objetos
rebuild: clean all

# Regra para executar o programa
run: $(EXEC)
	./$(EXEC)

