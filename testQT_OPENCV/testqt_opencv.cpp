#include <Qlabel>
#include <Qmenubar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QIcon>
#include <iostream>

#include "testqt_opencv.h"
#include "fileDialog.h"

testQT_OPENCV::testQT_OPENCV(QWidget *parent)
	: QMainWindow(parent), age(50)
{
	ui.setupUi(this);
	ui.lineEdit->setText(QString::number(age));
	setWindowTitle(tr("Main Window"));        // 文本使用tr()函数，这是一个用于 Qt 国际化的函数。
											  // 可以使用 Qt 提供的国际化工具，将tr()函数的字符串提取出来，进行国际化
											  // tr()函数里面一般是英文文本。

	openAction = new QAction(QIcon(tr(":/images/open")), tr("&Modal..."), this);   // QAction类代表了窗口的一个“动作”，可显示在菜单，也可在工具栏，是抽象出的公共动作
																				   // 在堆上创建了openAction对象
																				   // QIcon传入值是一个字符串，以 : 开始意味着从资源文件中查找资源
																				   // 第二个参数文本值前面有一个 &，意味着这将成为一个快捷键
																				   // C++ 要求堆上面创建的对象必须释放，Qt 当然也是。这里传入了 this 指针作为 parent，Qt 保证，在 parent 析构时，所有子元素全部析构。
																				   // 由于 MainWindow 是建立在栈上的，当 main() 函数退出时，MainWindow 会被自动析构，此时，所有以其为 parent 的对象全部自动析构。
	/* Qt 资源系统是一个跨平台的资源机制，用于将程序运行时所需要的资源以二进制的形式存储于可执行文件内部。 */
	/* QObject是以对象树的形式组织起来的。当在堆上创建一个QObject对象时，会看到QObject的构造函数接收一个QObject指针作为参数（父对象指针）。
	   创建的QObject对象会自动添加到其父对象的children()列表。当父对象析构的时候，这个列表中的所有对象也会被析构。（注意，这里的父对象并不是继承意义上的父类！）
       任何对象树中的 QObject对象 delete 的时候，如果这个对象有 parent，则自动将其从 parent 的children()列表中删除；如果有孩子，则自动 delete 每一个孩子。
	   Qt 保证没有QObject会被 delete 两次，这是由析构顺序决定的。
	   main中最外层的对象不应该创建在堆空间而应该创建在栈空间，main在堆空间创建对象时应该将最外层栈空间的对象作为parent
	   如果 parent 为空的话，堆空间上的QObject也是要自己 delete 的
	*/

	openAction->setShortcuts(QKeySequence::Open);     // 用于说明这个QAction的快捷键。Qt 的QKeySequence定义了很多内置的快捷键，这是 Qt 跨平台性的体现，因为 PC 键盘和 Mac 键盘是不一样的

	openAction->setStatusTip(tr("Open a Modal Dialog"));   // 实现了当用户鼠标滑过这个 action 时，会在主窗口下方的状态栏显示相应的提示。

	connect(openAction, &QAction::triggered, this, &testQT_OPENCV::actionOpenModalDialog);   // 将这个QAction的triggered()信号与MainWindow类的open()函数连接起来

	// 增加Setting选项
	openModelessDialog = new QAction(QIcon(tr(":/images/setting")), tr("&Modeless..."), this);
	openModelessDialog->setShortcut(QKeySequence::Preferences);
	openModelessDialog->setStatusTip(tr("Open a Modeless Dialog"));
	QObject::connect(openModelessDialog, &QAction::triggered, [=](){emit sendUserAge(age);});
		/* 当openModelessDialog被点击时，其发出QAction::triggered信号，匿名函数发出sendUserAge信号，将当前的age值传递出去并激活actionOpenModelessDialog槽函数 */
	QObject::connect(this, &testQT_OPENCV::sendUserAge, this, &testQT_OPENCV::actionOpenModelessDialog);
		/* 发出sendUserAge信号后，构造dialog的槽函数被激活，由于sendUserAge附带了age值所以成功向dialog进行传值 */

    openFileDialog = new QAction(tr("&FileDialog..."), this);
    QObject::connect(openFileDialog, &QAction::triggered, [=](){FileDialog* f = new FileDialog(this); f->show();});

    QMenu *file = menuBar()->addMenu(tr("&File"));   // menuBar()会返回窗口的菜单栏，如果没有菜单栏则会新创建
    file->addAction(openAction);                     // 把这个QAction对象添加到菜单
    QMenu *set = menuBar()->addMenu(tr("Setting"));
    set->addAction(openModelessDialog);

    QToolBar *toolBar = addToolBar(tr("&File"));   // 菜单栏只有一个，但工具栏可能有多个，所以要先addToolBar()
    toolBar->addAction(openAction);
    toolBar->addAction(openModelessDialog);
    toolBar->addAction(openFileDialog);

	statusBar();  // 状态栏，则是出现在窗口最下方，用于显示动作对象的提示信息。QStatusBar继承了QWidget，因此，我们可以将其它任意QWidget子类添加到状态栏
}

void testQT_OPENCV::changeUserAge(int age) {
	this->age = age;
	ui.lineEdit->setText(QString::number(age));
}

ModelessDialog::ModelessDialog(int age, QWidget *parent) : QDialog(parent) {
    spinBox = new QSpinBox(this);
	slider = new QSlider(Qt::Horizontal, this);
	buttonAccept = new QPushButton("Accept", this);
	spinBox->setRange(0, 100);
	spinBox->setValue(age);
	slider->setRange(0, 100);
	slider->setValue(age);

	QObject::connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);

	/* QSpinBox有两个信号：
	void valueChanged(int)
	void valueChanged(const QString &)
	直接如同上面的连接函数将 QSpinBox::valueChanged 传入 connect 作为 signal 编译器会报错因为不知道应该取哪个函数
	方法一：创建一个函数指针spinBoxSignal，这个函数指针参数指定为 int ，显示指定函数valueChanged(int)
		void (QSpinBox:: *spinBoxSignal)(int) = &QSpinBox::valueChanged;
		QObject::connect(spinBox, spinBoxSignal, slider, &QSlider::setValue);
	方法二：
		static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged)
	*/
	QObject::connect(spinBox, static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),
		slider, &QSlider::setValue);

	QObject::connect(buttonAccept, &QPushButton::clicked,
		[=](){emit closeModelessDialog(spinBox->value()); QDialog::close(); });
		/* 当button被点击后，发出closeModelessDialog信号，其携带了新的age值，并关闭当前dialog */

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(spinBox);
	layout->addWidget(slider);
	layout->addWidget(buttonAccept);
	/* QT布局管理器：
	QHBoxLayout：按照水平方向从左到右布局；
	QVBoxLayout：按照竖直方向从上到下布局；
	QGridLayout：在一个网格中进行布局，类似于 HTML 的 table；
	QFormLayout：按照表格布局，每一行前面是一段文本，文本后面跟随一个组件（通常是输入框），类似 HTML 的 form；
	QStackedLayout：层叠的布局，允许我们将几个组件按照 Z 轴方向堆叠，可以形成向导那种一页一页的效果。
	*/
	this->setLayout(layout);
}

void testQT_OPENCV::actionOpenModelessDialog(int age)
{
	ModelessDialog *dialog = new ModelessDialog(age, this);
	/*
	应用级模态对话框(使用QDialog::exec()实现)：阻塞同一应用程序中其它窗口的输入，适用于如“打开文件”功能。
										      exec()函数的真正含义是开启一个新的事件循环
	窗口级模态对话框(使用QDialog::open()实现)：该模态仅仅阻塞与对话框关联的窗口，但是依然允许用户与程序中其它窗口交互。适用于多窗口模式
		模态对话框一般在栈上创建：QDialog dialog(this); dialog.exec();
	非模态对话框(使用QDialog::show()实现)：非阻塞，如查找对话框，可以在显示着查找对话框的同时，继续对记事本的内容进行编辑
		非模态对话框一般在堆上创建：QDialog* dialog = new QDialog(this); dialog->setAttribute(Qt::WA_DeleteOnClose); dialog->show();
	*/
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	/*
	如果使用：
	  QDialog dialog(this);
	  dialog.show();
	对话框会一闪而过， 因为此时dialog在栈上创建，show()不会阻塞当前线程，所以当对话框会显示出来后，dialog.show()立即返回，
	代码继续执行，testQT_OPENCV::ChangeSetting()函数结束，在栈空间的 dialog 超出作用域被析构(dialog(this)将dialog加入对象树，但栈空间的dialog何时析构主要还是取决于函数何时结束，除非dialog的parent先被析构)
	所以非模态对话框要在堆上创建：
	  QDialog *dialog = new QDialog;
	但是dialog 使用 new 在堆上分配空间，却一直没有 delete。
	解决方案：将 MainWindow 的指针赋给 dialog ，使其加入对象树
	  QDialog *dialog = new QDialog(this);
	不过这样做仍然有问题：1.QWidget的parent必须是QWidget指针，所以不能将一个普通的 C++ 类指针传给 Qt 对话框。
	2.将主窗口作为 parent 时，主窗口不关闭，对话框就不会被销毁，dialog会一直占用内存。
	所以还应该设置对话框关闭时，自动销毁对话框。
	  QDialog *dialog = new QDialog;
	  dialog->setAttribute(Qt::WA_DeleteOnClose);
	*/

	dialog->setWindowTitle(tr("Setting"));
    QObject::connect(dialog, &ModelessDialog::closeModelessDialog, this, &testQT_OPENCV::changeUserAge);
		/* 接受将来dialog关闭时发送回来的值，并调用changeUserAge更新主窗口的显示 */
	dialog->show();
}

void testQT_OPENCV::actionOpenModalDialog()
{
	QDialog dialog(this);
	dialog.setWindowTitle(tr("Modal Dialog"));
	QPushButton *Accept = new QPushButton(tr("Accept"), this);
	QPushButton *Cancel = new QPushButton(tr("Cancel"), this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(Accept);
	layout->addWidget(Cancel);
	dialog.setLayout(layout);
	QObject::connect(Accept, &QPushButton::clicked, &dialog, &QDialog::accept);
	QObject::connect(Cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
	int returncode = dialog.exec();
	if (returncode == QDialog::Accepted) {
		QMessageBox::information(this, tr("Info"), tr("Dialog return accept"));
	}
	else if (returncode == QDialog::Rejected) {
		QMessageBox::information(this, tr("Info"), tr("Dialog return reject"));
	}
	/* 使用exec()显示模态对话框。dialog在栈空间创建，函数结束时会自动析构，(this)将对象加入对象树，当parent先行析构，dialog也会被析构
	exec()开始了一个事件循环，代码被阻塞。exec()函数未返回时后面的代码不会被执行。对话框关闭，exec()函数返回，此时就可以取得对话框的数据。
	如果设置 dialog 的属性为WA_DeleteOnClose，那么当对话框关闭时，对象被销毁，就不能通过exec()的返回值获取数据了。
	在这种情况下，可以考虑使用 parent 指针的方式构建对话框，避免设置WA_DeleteOnClose属性

	只有模态对话框采用返回值，QDialog::Accepted或者QDialog::Rejected。
	*/
}

void testQT_OPENCV::openImageClicked()
{
	image = cv::imread("image.jpg");//读取图像
	cv::namedWindow("Input image", 1);
	cv::imshow("Input image", image);
}
