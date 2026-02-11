# Qt4 兼容配置
unix{
	QTDIR = $$system(printenv QTDIR)
	CURRENT_PROJECT_PATH = $$system(pwd)
}

win32{
	QTDIR = $$system("echo %QTDIR%")
	CURRENT_PROJECT_PATH = .
}

!include($$QTDIR/lib/qmake.conf){
	error(the file $$QTDIR/lib/qmake.conf is not exist!!)
}

TARGET = file_deleter
TEMPLATE = app

# 输出目录
DESTDIR = ../bin

# 源文件
SOURCES += \
    main.cpp \
    FileDeleter.cpp

# 头文件
HEADERS += \
    FileDeleter.h

# 编译选项
QMAKE_CXXFLAGS += -Wall -Wextra

# Release模式优化
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
}

# Debug模式
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
    DEFINES += QT_DEBUG
}

!include($$QTDIR/lib/target.conf){
	error(the file $$QTDIR/lib/target.conf is not exist!!)
}

