# 文件/文件夹删除工具

一个带图形界面的文件删除工具，可以安全地删除服务器上的文件或文件夹。

## 功能特性

- ✅ 图形化界面，操作简单
- ✅ 支持删除文件和文件夹
- ✅ 删除前确认提示，防止误操作
- ✅ 详细的操作日志记录
- ✅ 支持通过浏览按钮选择文件/文件夹
- ✅ 支持直接输入路径

## 编译方法

### 方法一：使用 qmake（推荐）

```bash
cd file_deleter
qmake file_deleter.pro
make
```

编译完成后，可执行文件位于 `../bin/file_deleter`

### 方法二：使用 Makefile

```bash
cd file_deleter
# 确保 QTDIR 环境变量已设置，或修改 Makefile 中的 QTDIR 路径
export QTDIR=/usr/lib/qt5  # 根据实际Qt安装路径修改
make
```

### 方法三：手动编译

```bash
cd file_deleter

# 生成MOC文件
moc FileDeleter.h -o moc_FileDeleter.cpp

# 编译
g++ -std=c++11 -fPIC -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtWidgets \
    main.cpp FileDeleter.cpp moc_FileDeleter.cpp \
    -lQt5Core -lQt5Gui -lQt5Widgets -o file_deleter
```

## 依赖要求

- Qt5 或 Qt6（Qt Core, Qt Widgets, Qt GUI）
- C++11 或更高版本的编译器（g++/clang++）
- Linux 系统


## 使用方法

1. 运行程序：
   ```bash
   ./file_deleter
   ```

2. 选择要删除的文件或文件夹：
   - 点击"浏览文件"按钮选择文件
   - 点击"浏览文件夹"按钮选择文件夹
   - 或直接在输入框中输入完整路径

3. 点击"删除"按钮

4. 在确认对话框中确认删除操作

5. 查看操作日志了解删除结果

## 注意事项

⚠️ **警告**：此工具会永久删除文件和文件夹，操作不可撤销！

- 删除操作需要相应的文件系统权限
- 如果文件或文件夹被其他程序占用，删除可能会失败
- 删除系统关键文件可能导致系统不稳定，请谨慎操作
- 建议在删除前备份重要数据

## 安全建议

- 仅在信任的环境中使用
- 不要以root权限运行（除非确实需要）
- 删除前仔细检查路径
- 重要数据请先备份

## 故障排除

### 编译错误：找不到Qt头文件
- 确保已安装Qt开发包
- 检查QTDIR环境变量是否正确设置
- 检查Makefile或.pro文件中的路径配置

### 运行时错误：找不到Qt库
```bash
export LD_LIBRARY_PATH=/usr/lib/qt5/lib:$LD_LIBRARY_PATH
```

### 权限错误
- 确保对目标文件/文件夹有写权限
- 某些系统文件可能需要root权限





