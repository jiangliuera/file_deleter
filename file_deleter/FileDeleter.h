#ifndef FILEDELETER_H
#define FILEDELETER_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QTextCodec>

class FileDeleter : public QMainWindow
{
    Q_OBJECT

public:
    FileDeleter(QWidget *parent = NULL);
    ~FileDeleter();

private slots:
    void browseFile();
    void browseFolder();
    void deleteSelected();
    void clearLog();

private:
    void setupUI();
    bool deleteFileOrFolder(const QString &path);
    void logMessage(const QString &message);
    void updateStatus(const QString &status);
    
    // GBK编码转换辅助函数（将UTF-8字符串转换为GBK）
    static QString toGBK(const char* utf8Str);
    
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    QLabel *pathLabel;
    QLineEdit *pathEdit;
    QHBoxLayout *pathLayout;
    QPushButton *browseFileBtn;
    QPushButton *browseFolderBtn;
    
    QHBoxLayout *buttonLayout;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;
    
    QLabel *logLabel;
    QTextEdit *logText;
    
    QLabel *statusLabel;
};

#endif // FILEDELETER_H

