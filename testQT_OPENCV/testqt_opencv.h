#ifndef TESTQT_OPENCV_H
#define TESTQT_OPENCV_H

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include "ui_testqt_opencv.h"

#include <opencv/cv.hpp>
#include <opencv2/highgui.hpp>


class testQT_OPENCV : public QMainWindow
{
	Q_OBJECT

public:
	testQT_OPENCV(QWidget *parent = 0);
	~testQT_OPENCV() {}

private:
	Ui::testQT_OPENCVClass ui;
	int age;
	QAction *openAction;
	QAction *openModelessDialog;
    QAction *openFileDialog;
	void changeUserAge(int);  // 更新private int age并更新textEdit
	cv::Mat image;

signals:
	void sendUserAge(int);  // 用以传递age的信号

private slots:
	void actionOpenModelessDialog(int);  // 点击open modeless dialog这个action后触发的槽函数
	void actionOpenModalDialog();  // 点击open modal dialog这个action后触发的槽函数
	void openImageClicked();  // 点击Load这个Button后触发的槽函数
};

class ModelessDialog : public QDialog
{
	Q_OBJECT

public:
	ModelessDialog(int age, QWidget *parent = 0);
	~ModelessDialog() {}

private:
	QSpinBox *spinBox;
	QSlider *slider;
	QPushButton* buttonAccept;

signals:
	void openModelessDialog(int);  // 当开启dialog的action被trigger后发出此信号，用以向构造函数传值
	void closeModelessDialog(int);  // 当关闭dialog的button被click后发出此信号，用以向MainWindow传值
};

#endif // TESTQT_OPENCV_H
