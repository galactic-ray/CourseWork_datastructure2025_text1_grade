QT += core widgets
CONFIG += c++17

TARGET = grade_gui
TEMPLATE = app

SOURCES += main_gui.cpp

# 默认构建类型
CONFIG(release, debug|release) {
    DESTDIR = $$PWD
} else {
    DESTDIR = $$PWD
}

