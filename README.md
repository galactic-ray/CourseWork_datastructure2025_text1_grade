# 学生成绩管理系统 - GUI版本

## 作业要求
7、学生成绩管理
【问题描述】
对计算机学院计算机科学与技术专业的2011级本科生的学生成绩管理作一个简单的模拟。
【实验要求】
设计学生成绩管理的模拟程序。
（1）采用顺序表登录学生成绩。
（2）可以登记、查询、插入、删除学生成绩。
（3）将成绩按科目存储到链表中。



这是一个带有图形界面的学生成绩管理系统，使用Qt框架开发。

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

# 或者安装Qt6
sudo apt-get install qt6-base-dev qt6-base-dev-tools
```

### 使用CMake编译（推荐）

```bash
mkdir build
cd build
cmake ..
make
./grade_gui
```

### 使用qmake编译

```bash
qmake grade_gui.pro
make
./grade_gui
```

### 手动编译（如果已安装Qt）

```bash
# Qt5
qmake-qt5 grade_gui.pro
make

# Qt6
qmake6 grade_gui.pro
make

# 运行
./grade_gui
```

## 使用方法

1. **启动程序**: 运行 `./grade_gui`
2. **添加成绩**: 在左侧输入框中填写信息，点击"添加(追加)"按钮
3. **查询成绩**: 使用查询区域的输入框和按钮
4. **修改/删除**: 在表格中选择记录，信息会自动填充到输入框，然后修改并点击相应按钮
5. **文件操作**: 使用菜单栏或左侧的文件操作按钮

## 文件说明

- `main_gui.cpp`: GUI版本主程序
- `test.csv`: 数据文件（自动创建）
- `test.csv.bak`: 备份文件（自动创建）

## 界面说明

- **左侧面板**: 数据输入、查询、文件操作
- **右侧面板**: 成绩列表表格和操作日志
- **菜单栏**: 文件和数据操作
- **状态栏**: 显示记录总数

## 注意事项

- 程序启动时会自动尝试加载 `test.csv`
- 如果文件不存在，会自动创建示例数据
- 每次保存时会自动创建备份文件 `.bak`
- 成绩范围: 0-100分

