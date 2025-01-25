# コンパイラとフラグ
CXX = g++
CXXFLAGS = -g

# プロジェクトのディレクトリ構造
PROJECT_ROOT = .
SRC_ROOT = src
INCLUDES = -Iinclude
OBJ_ROOT = obj

# ソースファイルの自動検出
SRC_DIR = $(PROJECT_ROOT)/$(SRC_ROOT)
# SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")
# $(shell)関数はwindowsでは使えないので、wildcard関数を使う
SRC = $(wildcard $(SRC_ROOT)/*.cpp)

# オブジェクトファイルのリストを生成
OBJ_DIR = $(PROJECT_ROOT)/$(OBJ_ROOT)
# $(patsubst)関数は置換関数です。引数が3つあります。$(patsubst pattern,replacement,text)
# pattern: 置換対象の文字列を指定します。
# replacement: 置換後の文字列を指定します。
# text: 置換対象の文字列を指定します。
OBJ = $(patsubst $(SRC_ROOT)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# 実行ファイル名
TARGET = UI_Event_Getter

# デフォルトのターゲット
all: $(TARGET)

# 実行ファイルの生成ルール
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# オブジェクトファイルの生成ルール
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d "$(OBJ_DIR)" ]; then mkdir "$(OBJ_DIR)"; fi
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

# クリーンアップ
clean:
	rm -f $(OBJ) $(TARGET)
