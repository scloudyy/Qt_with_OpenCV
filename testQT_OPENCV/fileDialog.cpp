#include <QtGui>
#include <QtWidgets>
#include "fileDialog.h"

FileDialog::FileDialog(QWidget *parent) :
QMainWindow(parent)
{
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &FileDialog::openFile);

    saveAction = new QAction(tr("&Save..."), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a new file"));
    connect(saveAction, &QAction::triggered, this, &FileDialog::saveFile);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);
    file->addAction(saveAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);

    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);
}

FileDialog::~FileDialog()
{
}

void FileDialog::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files(*.txt)"));
    /*
    QString getOpenFileName(QWidget * parent = 0,
    const QString & caption = QString(),
    const QString & dir = QString(),
    const QString & filter = QString(),
    QString * selectedFilter = 0,
    Options options = 0)

    2. caption：对话框标题;
    3. dir：对话框打开时的默认目录，“.” 代表程序运行目录;
    4. filter：过滤器。我们使用文件对话框可以浏览很多类型的文件，但是，很多时候我们仅希望打开特定类型的文件。
       比如，文本编辑器希望打开文本文件，图片浏览器希望打开图片文件。
       过滤器就是用于过滤特定的后缀名。如果我们使用“Image Files(*.jpg *.png)”，则只能显示后缀名是 jpg 或者 png 的文件。
       如果需要多个过滤器，使用“;;”分割，比如“JPEG Files(*.jpg);;PNG Files(*.png)”；
       selectedFilter：默认选择的过滤器；
    5. options：对话框的一些参数设定，比如只显示文件夹等等，它的取值是enum QFileDialog::Option，每个选项可以使用 | 运算组合起来。

    QFileDialog::getOpenFileName()返回值是选择的文件路径
    */

    if (!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);   // QTextStream 在内部统一使用 Unicode 编码，所以用 QTextStream 写入和读取都不会有乱码
        textEdit->setText(in.readAll());
        file.close();
    }
    else {
        QMessageBox::warning(this, tr("Path"),
            tr("You did not select any file."));
    }
}

void FileDialog::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this,
        tr("Save File"),
        ".",
        tr("Text Files(*.txt)"));
    if (!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
    }
    else {
        QMessageBox::warning(this, tr("Path"),
            tr("You did not select any file."));
    }
}
