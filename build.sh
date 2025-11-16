#!/bin/bash

# 编译脚本
set -e

echo "=========================================="
echo "  学生成绩管理系统 - 编译脚本"
echo "=========================================="
echo ""

# 检查Qt是否安装
if command -v qmake &> /dev/null; then
    QMAKE_CMD="qmake"
elif command -v qmake-qt5 &> /dev/null; then
    QMAKE_CMD="qmake-qt5"
elif command -v qmake6 &> /dev/null; then
    QMAKE_CMD="qmake6"
else
    echo "错误: 未找到qmake命令"
    echo ""
    echo "请安装Qt开发库:"
    echo "  Ubuntu/Debian: sudo apt-get install qt5-default qtbase5-dev"
    echo "  或: sudo apt-get install qt6-base-dev"
    exit 1
fi

echo "使用: $QMAKE_CMD"
echo ""

# 创建build目录
mkdir -p build

# 使用qmake编译
echo "正在生成Makefile..."
cd build
$QMAKE_CMD ../grade_gui.pro

if [ $? -ne 0 ]; then
    echo "qmake失败"
    exit 1
fi

echo "正在编译..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "make失败"
    exit 1
fi

cd ..

echo ""
echo "=========================================="
echo "编译成功！"
echo "=========================================="
echo ""
echo "运行程序:"
echo "  ./build/grade_gui"
echo "  或"
echo "  cd build && ./grade_gui"
echo ""
