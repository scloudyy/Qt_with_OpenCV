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
	void changeUserAge(int);  // ����private int age������textEdit
	cv::Mat image;

signals:
	void sendUserAge(int);  // ���Դ���age���ź�

private slots:
	void actionOpenModelessDialog(int);  // ���open modeless dialog���action�󴥷��Ĳۺ���
	void actionOpenModalDialog();  // ���open modal dialog���action�󴥷��Ĳۺ���
	void openImageClicked();  // ���Load���Button�󴥷��Ĳۺ���
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
	void openModelessDialog(int);  // ������dialog��action��trigger�󷢳����źţ��������캯����ֵ
	void closeModelessDialog(int);  // ���ر�dialog��button��click�󷢳����źţ�������MainWindow��ֵ
};

#endif // TESTQT_OPENCV_H
