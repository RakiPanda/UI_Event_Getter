# コンパイラとフラグ
CXX = g++
CXXFLAGS = -g

# プロジェクトのディレクトリ構造
PROJECT_ROOT = .
SRC_ROOT = src
INCLUDES = -Iinclude

# ソースファイルの自動検出
SRC_DIR = $(PROJECT_ROOT)/$(SRC_ROOT)
# SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")
# $(shell)関数はwindowsでは使えないので、wildcard関数を使う
SRC = $(wildcard $(SRC_ROOT)/*.cpp)

# オブジェクトファイルのリストを生成
OBJ = $(SRC:.cpp=.o)

# 実行ファイル名
TARGET = UI_Event_Getter

# デフォルトのターゲット
all: $(TARGET)

# 実行ファイルの生成ルール
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# オブジェクトファイルの生成ルール
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

# クリーンアップ
clean:
	rm -f $(OBJ) $(TARGET)
