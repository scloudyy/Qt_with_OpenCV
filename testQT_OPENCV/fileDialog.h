#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;

class FileDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = 0);
    ~FileDialog();

    private slots:
    void openFile();
    void saveFile();

private:
    QAction *openAction;
    QAction *saveAction;

    QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
