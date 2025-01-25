# コンパイラとフラグ
CXX = g++
CXXFLAGS = -g

# プロジェクトのディレクトリ構造
PROJECT_ROOT = .
SRC_ROOT = src
INCLUDES = -Iinclude

# ソースファイルの自動検出
# $(shell)関数は後述のターミナルコマンドを直接叩けます
# findコマンドでsrcフォルダ内のファイルを検索
# -typeフラグにfを指定するとファイルのみ検索対象にします
# -nameは検索したいファイル名で、*.cppとあるので拡張子がcppなファイルのみ全検索しています
SRC_DIR = $(PROJECT_ROOT)/$(SRC_ROOT)
SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")

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
