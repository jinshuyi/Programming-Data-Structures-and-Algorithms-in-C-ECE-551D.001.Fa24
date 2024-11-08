# 指定编译器
CXX = g++

# 编译器选项
CXXFLAGS = -std=c++11 -Wall

# 可执行文件名称
TARGET = test1

# 源文件
SRC = test1.cpp

# 头文件
HEADERS = poly.hpp

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# 运行测试
run: $(TARGET)
	./$(TARGET)

# 清理编译文件
clean:
	rm -f $(TARGET)
