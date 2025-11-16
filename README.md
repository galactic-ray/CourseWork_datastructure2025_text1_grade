# 学生成绩管理系统

一个基于Qt的学生成绩管理系统，提供现代化的图形用户界面。

## 项目结构

```
project/
│
├── src/                    # 源代码目录
│   ├── main_gui.cpp        # 主程序入口
│   ├── mainwindow.cpp      # 主窗口实现
│   ├── mainwindow.h         # 主窗口头文件
│   ├── datastructures.h     # 数据结构定义
│   └── printmanager.h       # 打印管理器（可选）
│
├── data/                   # 数据文件目录
│   ├── test.csv            # 默认数据文件
│   └── *.csv.bak           # 备份文件
│
├── build/                  # 编译输出目录（自动生成）
│
├── CMakeLists.txt          # CMake构建配置
├── grade_gui.pro           # qmake项目文件
├── build.sh                # 编译脚本
├── README.md               # 本文件
└── .gitignore              # Git忽略文件
```

## 功能特性

- ✅ 现代化的图形用户界面
- ✅ 添加、插入、修改、删除成绩记录
- ✅ 按学号、姓名、科目查询
- ✅ 显示全部记录（顺序表/按科目分组）
- ✅ CSV文件导入导出
- ✅ 自动备份功能
- ✅ 示例数据加载

## 编译要求

### Ubuntu/Debian系统

```bash
# 安装Qt5开发库
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev qtbase5-dev-tools

# 或安装Qt6
sudo apt-get install qt6-base-dev qt6-base-dev-tools
```

## 编译方法

### 方法1：使用编译脚本（推荐）

```bash
chmod +x build.sh
./build.sh
```

### 方法2：使用qmake

```bash
mkdir -p build
cd build
qmake ../grade_gui.pro
make
cd ..
```

### 方法3：使用CMake

```bash
mkdir -p build
cd build
cmake ..
make
cd ..
```

## 运行程序

编译完成后，运行：

```bash
./build/grade_gui
```

或

```bash
cd build && ./grade_gui
```

## 使用方法

1. **启动程序**: 运行编译后的可执行文件
2. **添加成绩**: 在左侧输入框中填写信息，点击"添加(追加)"按钮
3. **查询成绩**: 使用查询区域的输入框和按钮
4. **修改/删除**: 在表格中选择记录，信息会自动填充到输入框，然后修改并点击相应按钮
5. **文件操作**: 使用菜单栏或左侧的文件操作按钮

## 数据文件

- 默认数据文件位置: `data/test.csv`
- 备份文件: `data/test.csv.bak`
- 程序启动时会自动尝试加载 `data/test.csv`
- 如果文件不存在，会自动创建示例数据

## 注意事项

- 程序启动时会自动创建 `data/` 目录（如果不存在）
- 每次保存时会自动创建备份文件 `.bak`
- 成绩范围: 0-100分
- 支持Qt5和Qt6

## 开发说明

### 代码结构

- `datastructures.h`: 包含 `Grade` 结构体、`SeqList` 模板类和 `SubjectIndex` 类
- `mainwindow.h/cpp`: 主窗口类，包含所有UI逻辑和业务逻辑
- `main_gui.cpp`: 程序入口点
- `printmanager.h`: 打印/格式化工具（可选）

### 扩展功能

如需添加新功能，可以：

1. 在 `mainwindow.h` 中添加新的槽函数声明
2. 在 `mainwindow.cpp` 中实现槽函数
3. 在 `setupUI()` 中连接信号和槽

## 许可证

本项目用于数据结构课程作业。
