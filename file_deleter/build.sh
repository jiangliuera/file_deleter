#!/bin/bash

# 文件删除工具编译脚本

echo "=========================================="
echo "  文件删除工具编译脚本"
echo "=========================================="

# 检查qmake是否可用
if command -v qmake &> /dev/null; then
    echo "使用 qmake 编译..."
    qmake file_deleter.pro
    if [ $? -eq 0 ]; then
        make
        if [ $? -eq 0 ]; then
            echo ""
            echo "✓ 编译成功！"
            echo "可执行文件位置: ../bin/file_deleter"
            echo ""
            # 检查文件是否存在
            if [ -f "../bin/file_deleter" ]; then
                chmod +x ../bin/file_deleter
                echo "可以运行: ../bin/file_deleter"
            fi
            exit 0
        else
            echo "✗ make 失败"
            exit 1
        fi
    else
        echo "✗ qmake 失败"
        exit 1
    fi
else
    echo "qmake 未找到，尝试使用 Makefile..."
    
    # 检查Qt路径
    if [ -z "$QTDIR" ]; then
        # 尝试自动检测Qt路径
        if [ -d "/usr/lib/qt5" ]; then
            export QTDIR=/usr/lib/qt5
            echo "自动检测到 QTDIR: $QTDIR"
        elif [ -d "/usr/lib/x86_64-linux-gnu/qt5" ]; then
            export QTDIR=/usr/lib/x86_64-linux-gnu/qt5
            echo "自动检测到 QTDIR: $QTDIR"
        else
            echo "错误: 未找到Qt安装路径"
            echo "请设置 QTDIR 环境变量，例如:"
            echo "  export QTDIR=/usr/lib/qt5"
            exit 1
        fi
    fi
    
    make -f Makefile
    if [ $? -eq 0 ]; then
        echo ""
        echo "✓ 编译成功！"
        echo "可执行文件位置: ./file_deleter"
        echo ""
        chmod +x file_deleter
        echo "可以运行: ./file_deleter"
        exit 0
    else
        echo "✗ 编译失败"
        exit 1
    fi
fi




