# コンパイラとフラグ
CXX = g++
CXXFLAGS = -g

# ソースファイルの自動検出
SRC = $(wildcard *.cpp)

# オブジェクトファイルのリストを生成
OBJ = $(SRC:.cpp=.o)

# 実行ファイル名
TARGET = hello_world

# デフォルトのターゲット
all: $(TARGET)

# 実行ファイルの生成ルール
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# オブジェクトファイルの生成ルール
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# クリーンアップ
clean:
	rm -f $(OBJ) $(TARGET)
