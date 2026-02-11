#include "FileDeleter.h"
#include <QApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QTextCodec>

// GBK编码转换辅助函数（将UTF-8字符串转换为GBK编码的QString）
// 如果源代码文件是UTF-8编码，字符串字面量是UTF-8字节序列
// 需要先转换为Unicode QString，然后Qt会自动使用GBK编码显示
QString FileDeleter::toGBK(const char* utf8Str)
{
    // 如果源文件是UTF-8编码，字符串字面量是UTF-8字节
    // 使用fromUtf8转换为Unicode QString
    // 由于已经设置了setCodecForLocale为GBK，Qt会自动使用GBK编码显示
    return QString::fromUtf8(utf8Str);
}

FileDeleter::FileDeleter(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle(toGBK("文件/文件夹删除工具"));
    setMinimumSize(600, 500);
    resize(800, 600);
}

FileDeleter::~FileDeleter()
{
}

void FileDeleter::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // 路径选择区域
    pathLabel = new QLabel(toGBK("选择要删除的文件或文件夹:"), this);
    pathLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    mainLayout->addWidget(pathLabel);
    
    pathLayout = new QHBoxLayout();
    pathEdit = new QLineEdit(this);
    pathEdit->setPlaceholderText(toGBK("输入文件或文件夹路径，或点击浏览按钮选择..."));
    pathEdit->setStyleSheet("padding: 8px; font-size: 11pt;");
    pathLayout->addWidget(pathEdit);
    
    browseFileBtn = new QPushButton(toGBK("浏览文件"), this);
    browseFileBtn->setStyleSheet("padding: 8px 15px; font-size: 11pt;");
    connect(browseFileBtn, SIGNAL(clicked()), this, SLOT(browseFile()));
    pathLayout->addWidget(browseFileBtn);
    
    browseFolderBtn = new QPushButton(toGBK("浏览文件夹"), this);
    browseFolderBtn->setStyleSheet("padding: 8px 15px; font-size: 11pt;");
    connect(browseFolderBtn, SIGNAL(clicked()), this, SLOT(browseFolder()));
    pathLayout->addWidget(browseFolderBtn);
    
    mainLayout->addLayout(pathLayout);
    
    // 按钮区域
    buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    deleteBtn = new QPushButton(toGBK("删除"), this);
    deleteBtn->setStyleSheet("padding: 10px 30px; font-size: 12pt; font-weight: bold; background-color: #e74c3c; color: white; border: none; border-radius: 5px;");
    deleteBtn->setCursor(Qt::PointingHandCursor);
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteSelected()));
    buttonLayout->addWidget(deleteBtn);
    
    clearBtn = new QPushButton(toGBK("清空日志"), this);
    clearBtn->setStyleSheet("padding: 10px 20px; font-size: 11pt; background-color: #95a5a6; color: white; border: none; border-radius: 5px;");
    clearBtn->setCursor(Qt::PointingHandCursor);
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearLog()));
    buttonLayout->addWidget(clearBtn);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // 日志区域
    logLabel = new QLabel(toGBK("操作日志:"), this);
    logLabel->setStyleSheet("font-weight: bold; font-size: 11pt;");
    mainLayout->addWidget(logLabel);
    
    logText = new QTextEdit(this);
    logText->setReadOnly(true);
    logText->setStyleSheet("background-color: #2c3e50; color: #ecf0f1; font-family: 'Courier New', monospace; font-size: 10pt; padding: 10px;");
    mainLayout->addWidget(logText);
    
    // 状态栏
    statusLabel = new QLabel(toGBK("就绪"), this);
    statusLabel->setStyleSheet("padding: 5px; background-color: #ecf0f1; border-top: 1px solid #bdc3c7;");
    mainLayout->addWidget(statusLabel);
    
    logMessage(toGBK("文件删除工具已启动"));
    logMessage(toGBK("请选择要删除的文件或文件夹"));
}

void FileDeleter::browseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        toGBK("选择要删除的文件"),
        QDir::homePath(),
        toGBK("所有文件 (*.*)")
    );
    
    if (!fileName.isEmpty()) {
        pathEdit->setText(fileName);
        logMessage(QString(toGBK("已选择文件: %1")).arg(fileName));
    }
}

void FileDeleter::browseFolder()
{
    QString folderName = QFileDialog::getExistingDirectory(
        this,
        toGBK("选择要删除的文件夹"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly
    );
    
    if (!folderName.isEmpty()) {
        pathEdit->setText(folderName);
        logMessage(QString(toGBK("已选择文件夹: %1")).arg(folderName));
    }
}

void FileDeleter::deleteSelected()
{
    QString path = pathEdit->text().trimmed();
    
    if (path.isEmpty()) {
        QMessageBox::warning(this, toGBK("警告"), toGBK("请先选择要删除的文件或文件夹！"));
        return;
    }
    
    QFileInfo fileInfo(path);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, toGBK("错误"), QString(toGBK("路径不存在: %1")).arg(path));
        logMessage(QString(toGBK("错误: 路径不存在 - %1")).arg(path));
        return;
    }
    
    // 确认对话框
    QString itemType = fileInfo.isDir() ? toGBK("文件夹") : toGBK("文件");
    QString itemName = fileInfo.fileName();
    if (itemName.isEmpty()) {
        itemName = path;
    }
    
    int ret = QMessageBox::warning(
        this,
        toGBK("确认删除"),
        QString(toGBK("确定要删除以下%1吗？\n\n%2\n\n此操作不可撤销！")).arg(itemType, path),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (ret != QMessageBox::Yes) {
        logMessage(toGBK("删除操作已取消"));
        return;
    }
    
    updateStatus(toGBK("正在删除..."));
    logMessage(QString(toGBK("开始删除: %1")).arg(path));
    
    // 执行删除
    bool success = deleteFileOrFolder(path);
    
    if (success) {
        logMessage(QString(toGBK("✓ 成功删除: %1")).arg(path));
        updateStatus(toGBK("删除成功"));
        pathEdit->clear();
        QMessageBox::information(this, toGBK("成功"), QString(toGBK("%1已成功删除")).arg(itemType));
    } else {
        logMessage(QString(toGBK("✗ 删除失败: %1")).arg(path));
        updateStatus(toGBK("删除失败"));
        QMessageBox::critical(this, toGBK("错误"), QString(toGBK("删除%1失败，请检查权限或文件是否被占用")).arg(itemType));
    }
}

bool FileDeleter::deleteFileOrFolder(const QString &path)
{
    QFileInfo fileInfo(path);
    
    if (fileInfo.isDir()) {
        // 删除文件夹 - Qt4不支持removeRecursively，使用手动递归删除
        logMessage(toGBK("开始递归删除文件夹..."));
        
        // 使用QDirIterator遍历所有文件和子目录
        // Qt4中QDirIterator::FollowSymlinks可能不支持，使用Subdirectories
        QDirIterator it(path, QDirIterator::Subdirectories);
        QStringList filesToDelete;
        QStringList dirsToDelete;
        
        // 收集所有需要删除的路径（深度优先，先删除深层文件）
        while (it.hasNext()) {
            it.next();
            QFileInfo info = it.fileInfo();
            
            if (info.isFile() || info.isSymLink()) {
                filesToDelete.prepend(info.absoluteFilePath());
            } else if (info.isDir() && info.absoluteFilePath() != path) {
                dirsToDelete.prepend(info.absoluteFilePath());
            }
        }
        
        // 删除所有文件
        bool allFilesDeleted = true;
        foreach (const QString &filePath, filesToDelete) {
            if (!QFile::remove(filePath)) {
                logMessage(QString(toGBK("警告: 无法删除文件 %1")).arg(filePath));
                allFilesDeleted = false;
            }
        }
        
        // 删除所有子文件夹（从最深层的开始）
        bool allDirsDeleted = true;
        foreach (const QString &dirPath, dirsToDelete) {
            QDir d(dirPath);
            if (!d.rmdir(dirPath)) {
                logMessage(QString(toGBK("警告: 无法删除文件夹 %1")).arg(dirPath));
                allDirsDeleted = false;
            }
        }
        
        // 最后删除主文件夹
        QDir parentDir = QFileInfo(path).absoluteDir();
        QString dirName = QFileInfo(path).fileName();
        if (dirName.isEmpty()) {
            dirName = path;
        }
        
        bool mainDirDeleted = parentDir.rmdir(dirName);
        
        return allFilesDeleted && allDirsDeleted && mainDirDeleted;
    } else {
        // 删除文件
        return QFile::remove(path);
    }
}

void FileDeleter::clearLog()
{
    logText->clear();
    logMessage(toGBK("日志已清空"));
}

void FileDeleter::logMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logEntry = QString("[%1] %2").arg(timestamp, message);
    logText->append(logEntry);
    
    // 自动滚动到底部
    QTextCursor cursor = logText->textCursor();
    cursor.movePosition(QTextCursor::End);
    logText->setTextCursor(cursor);
}

void FileDeleter::updateStatus(const QString &status)
{
    statusLabel->setText(status);
    QApplication::processEvents();
}

