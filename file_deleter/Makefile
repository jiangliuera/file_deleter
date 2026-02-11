# Makefile for File Deleter
# 使用 qmake 生成，或手动编译

# Qt 路径（如果QTDIR未设置，请修改此处）
QTDIR ?= /usr/lib/qt5
QT_INCLUDE = $(QTDIR)/include
QT_LIB = $(QTDIR)/lib

# 编译器
CXX = g++
MOC = $(QTDIR)/bin/moc

# 编译选项
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC
INCLUDES = -I$(QT_INCLUDE) -I$(QT_INCLUDE)/QtCore -I$(QT_INCLUDE)/QtGui -I$(QT_INCLUDE)/QtWidgets
LIBS = -L$(QT_LIB) -lQt5Core -lQt5Gui -lQt5Widgets

# 源文件
SOURCES = main.cpp FileDeleter.cpp
HEADERS = FileDeleter.h
MOC_SOURCES = moc_FileDeleter.cpp

# 目标文件
OBJECTS = $(SOURCES:.cpp=.o) $(MOC_SOURCES:.cpp=.o)
TARGET = file_deleter

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# 编译源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 生成MOC文件
moc_FileDeleter.cpp: FileDeleter.h
	$(MOC) FileDeleter.h -o moc_FileDeleter.cpp

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET) moc_FileDeleter.cpp

# 安装（可选）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install




