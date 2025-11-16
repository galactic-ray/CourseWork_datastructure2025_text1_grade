#!/bin/bash

echo "正在编译GUI版本..."

# 检查Qt是否安装
if command -v qmake &> /dev/null; then
    QMAKE_CMD="qmake"
elif command -v qmake-qt5 &> /dev/null; then
    QMAKE_CMD="qmake-qt5"
elif command -v qmake6 &> /dev/null; then
    QMAKE_CMD="qmake6"
else
    echo "错误: 未找到qmake命令"
    echo "请安装Qt开发库:"
    echo "  Ubuntu/Debian: sudo apt-get install qt5-default qtbase5-dev"
    echo "  或: sudo apt-get install qt6-base-dev"
    exit 1
fi

echo "使用: $QMAKE_CMD"

# 使用qmake编译
$QMAKE_CMD grade_gui.pro
if [ $? -ne 0 ]; then
    echo "qmake失败"
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "make失败"
    exit 1
fi

echo ""
echo "编译成功！"
echo "运行程序: ./grade_gui"

