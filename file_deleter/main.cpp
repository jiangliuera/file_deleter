#include "FileDeleter.h"
#include <QApplication>
#include <QTextCodec>

#define LOCAL_LANGUAGE "gbk"  // 设置界面显示为GBK中文格式

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置GBK编码支持（Qt4兼容）
    QTextCodec::setCodecForTr( QTextCodec::codecForName(LOCAL_LANGUAGE) );
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName(LOCAL_LANGUAGE) );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName(LOCAL_LANGUAGE) );
    
    // 设置应用程序信息
    app.setApplicationName("File Deleter");
    
    FileDeleter window;
    window.show();
    
    return app.exec();
}

