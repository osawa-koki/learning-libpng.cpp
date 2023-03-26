CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++11
SRC_DIR=./src
BIN_DIR=./bin

# srcディレクトリにあるすべての.cppファイルをビルド
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SOURCES))

# 実行ファイルのビルド
all: $(OBJECTS)

# 依存関係の生成
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -lpng -o $@

clean:
	rm -f $(OBJECTS)
