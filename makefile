# === 編譯器與編譯選項 ===
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinc

# === 資料夾 ===
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN = maze_game

# === 搜尋所有的 .cpp ===
SRC = $(wildcard $(SRC_DIR)/*.cpp) main.cpp

# === 將所有 .cpp 對應成 .o 檔放在 obj/ ===
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# === 預設目標 ===
all: $(BIN)

# === 編譯連結 ===
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === 個別 .cpp 編成 .o 的規則 ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === 建立 obj 資料夾（如果還沒存在） ===
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# === 清除所有編譯產物 ===
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# === 顯示訊息用的目標 ===
run: all
	./$(BIN)

.PHONY: all clean run
