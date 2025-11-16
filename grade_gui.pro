QT += core widgets
CONFIG += c++17

TARGET = grade_gui
TEMPLATE = app

# 源文件
SOURCES += \
    src/main_gui.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/mainwindow.h \
    src/datastructures.h \
    src/printmanager.h

# 输出目录
CONFIG(release, debug|release) {
    DESTDIR = $$PWD/build
} else {
    DESTDIR = $$PWD/build
}

# 忽略构建文件
OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui
