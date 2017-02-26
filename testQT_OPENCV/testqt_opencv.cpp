#include <Qlabel>
#include <Qmenubar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QIcon>
#include <iostream>

#include "testqt_opencv.h"

testQT_OPENCV::testQT_OPENCV(QWidget *parent)
	: QMainWindow(parent), age(50)
{
	ui.setupUi(this);
	ui.lineEdit->setText(QString::number(age));
	setWindowTitle(tr("Main Window"));        // �ı�ʹ��tr()����������һ������ Qt ���ʻ��ĺ�����
											  // ����ʹ�� Qt �ṩ�Ĺ��ʻ����ߣ���tr()�������ַ�����ȡ���������й��ʻ�
											  // tr()��������һ����Ӣ���ı���

	openAction = new QAction(QIcon(tr(":/images/open")), tr("&Modal..."), this);   // QAction������˴��ڵ�һ����������������ʾ�ڲ˵���Ҳ���ڹ��������ǳ�����Ĺ�������
																				   // �ڶ��ϴ�����openAction����
																				   // QIcon����ֵ��һ���ַ������� : ��ʼ��ζ�Ŵ���Դ�ļ��в�����Դ
																				   // �ڶ��������ı�ֵǰ����һ�� &����ζ���⽫��Ϊһ����ݼ�
																				   // C++ Ҫ������洴���Ķ�������ͷţ�Qt ��ȻҲ�ǡ����ﴫ���� this ָ����Ϊ parent��Qt ��֤���� parent ����ʱ��������Ԫ��ȫ��������
																				   // ���� MainWindow �ǽ�����ջ�ϵģ��� main() �����˳�ʱ��MainWindow �ᱻ�Զ���������ʱ����������Ϊ parent �Ķ���ȫ���Զ�������
	/* Qt ��Դϵͳ��һ����ƽ̨����Դ���ƣ����ڽ���������ʱ����Ҫ����Դ�Զ����Ƶ���ʽ�洢�ڿ�ִ���ļ��ڲ��� */
	/* QObject���Զ���������ʽ��֯�����ġ����ڶ��ϴ���һ��QObject����ʱ���ῴ��QObject�Ĺ��캯������һ��QObjectָ����Ϊ������������ָ�룩��
	   ������QObject������Զ���ӵ��丸�����children()�б���������������ʱ������б��е����ж���Ҳ�ᱻ��������ע�⣬����ĸ����󲢲��Ǽ̳������ϵĸ��࣡��
       �κζ������е� QObject���� delete ��ʱ�������������� parent�����Զ������ parent ��children()�б���ɾ��������к��ӣ����Զ� delete ÿһ�����ӡ�
	   Qt ��֤û��QObject�ᱻ delete ���Σ�����������˳������ġ�
	   main�������Ķ���Ӧ�ô����ڶѿռ��Ӧ�ô�����ջ�ռ䣬main�ڶѿռ䴴������ʱӦ�ý������ջ�ռ�Ķ�����Ϊparent
	   ��� parent Ϊ�յĻ����ѿռ��ϵ�QObjectҲ��Ҫ�Լ� delete ��
	*/

	openAction->setShortcuts(QKeySequence::Open);     // ����˵�����QAction�Ŀ�ݼ���Qt ��QKeySequence�����˺ܶ����õĿ�ݼ������� Qt ��ƽ̨�Ե����֣���Ϊ PC ���̺� Mac �����ǲ�һ����

	openAction->setStatusTip(tr("Open a Modal Dialog"));   // ʵ���˵��û���껬����� action ʱ�������������·���״̬����ʾ��Ӧ����ʾ��

	connect(openAction, &QAction::triggered, this, &testQT_OPENCV::actionOpenModalDialog);   // �����QAction��triggered()�ź���MainWindow���open()������������

	QMenu *file = menuBar()->addMenu(tr("&File"));   // menuBar()�᷵�ش��ڵĲ˵��������û�в˵�������´���
	file->addAction(openAction);                     // �����QAction������ӵ��˵�

	QToolBar *toolBar = addToolBar(tr("&File"));   // �˵���ֻ��һ�����������������ж��������Ҫ��addToolBar()
	toolBar->addAction(openAction);

	// ����Settingѡ��
	openModelessDialog = new QAction(QIcon(tr(":/images/setting")), tr("&Modeless..."), this);
	openModelessDialog->setShortcut(QKeySequence::Preferences);
	openModelessDialog->setStatusTip(tr("Open a Modeless Dialog"));
	QObject::connect(openModelessDialog, &QAction::triggered, [=](){emit sendUserAge(age);});
		/* ��openModelessDialog�����ʱ���䷢��QAction::triggered�źţ�������������sendUserAge�źţ�����ǰ��ageֵ���ݳ�ȥ������actionOpenModelessDialog�ۺ��� */
	QObject::connect(this, &testQT_OPENCV::sendUserAge, this, &testQT_OPENCV::actionOpenModelessDialog);
		/* ����sendUserAge�źź󣬹���dialog�Ĳۺ������������sendUserAge������ageֵ���Գɹ���dialog���д�ֵ */
	QMenu *set = menuBar()->addMenu(tr("Setting"));
	set->addAction(openModelessDialog);
	toolBar->addAction(openModelessDialog);

	statusBar();  // ״̬�������ǳ����ڴ������·���������ʾ�����������ʾ��Ϣ��QStatusBar�̳���QWidget����ˣ����ǿ��Խ���������QWidget������ӵ�״̬��
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

	/* QSpinBox�������źţ�
	void valueChanged(int)
	void valueChanged(const QString &)
	ֱ����ͬ��������Ӻ����� QSpinBox::valueChanged ���� connect ��Ϊ signal �������ᱨ����Ϊ��֪��Ӧ��ȡ�ĸ�����
	����һ������һ������ָ��spinBoxSignal���������ָ�����ָ��Ϊ int ����ʾָ������valueChanged(int)
		void (QSpinBox:: *spinBoxSignal)(int) = &QSpinBox::valueChanged;
		QObject::connect(spinBox, spinBoxSignal, slider, &QSlider::setValue);
	��������
		static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged)
	*/
	QObject::connect(spinBox, static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),
		slider, &QSlider::setValue);

	QObject::connect(buttonAccept, &QPushButton::clicked,
		[=](){emit closeModelessDialog(spinBox->value()); QDialog::close(); });
		/* ��button������󣬷���closeModelessDialog�źţ���Я�����µ�ageֵ�����رյ�ǰdialog */

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(spinBox);
	layout->addWidget(slider);
	layout->addWidget(buttonAccept);
	/* QT���ֹ�������
	QHBoxLayout������ˮƽ��������Ҳ��֣�
	QVBoxLayout��������ֱ������ϵ��²��֣�
	QGridLayout����һ�������н��в��֣������� HTML �� table��
	QFormLayout�����ձ�񲼾֣�ÿһ��ǰ����һ���ı����ı��������һ�������ͨ��������򣩣����� HTML �� form��
	QStackedLayout������Ĳ��֣��������ǽ������������ Z �᷽��ѵ��������γ�������һҳһҳ��Ч����
	*/
	this->setLayout(layout);
}

void testQT_OPENCV::actionOpenModelessDialog(int age)
{
	ModelessDialog *dialog = new ModelessDialog(age, this);
	/*
	Ӧ�ü�ģ̬�Ի���(ʹ��QDialog::exec()ʵ��)������ͬһӦ�ó������������ڵ����룬�������硰���ļ������ܡ�
										      exec()���������������ǿ���һ���µ��¼�ѭ��
	���ڼ�ģ̬�Ի���(ʹ��QDialog::open()ʵ��)����ģ̬����������Ի�������Ĵ��ڣ�������Ȼ�����û���������������ڽ����������ڶര��ģʽ
		ģ̬�Ի���һ����ջ�ϴ�����QDialog dialog(this); dialog.exec();
	��ģ̬�Ի���(ʹ��QDialog::show()ʵ��)��������������ҶԻ��򣬿�������ʾ�Ų��ҶԻ����ͬʱ�������Լ��±������ݽ��б༭
		��ģ̬�Ի���һ���ڶ��ϴ�����QDialog* dialog = new QDialog(this); dialog->setAttribute(Qt::WA_DeleteOnClose); dialog->show();
	*/
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	/*
	���ʹ�ã�
	  QDialog dialog(this);
	  dialog.show();
	�Ի����һ�������� ��Ϊ��ʱdialog��ջ�ϴ�����show()����������ǰ�̣߳����Ե��Ի������ʾ������dialog.show()�������أ�
	�������ִ�У�testQT_OPENCV::ChangeSetting()������������ջ�ռ�� dialog ��������������(dialog(this)��dialog�������������ջ�ռ��dialog��ʱ������Ҫ����ȡ���ں�����ʱ����������dialog��parent�ȱ�����)
	���Է�ģ̬�Ի���Ҫ�ڶ��ϴ�����
	  QDialog *dialog = new QDialog;
	����dialog ʹ�� new �ڶ��Ϸ���ռ䣬ȴһֱû�� delete��
	����������� MainWindow ��ָ�븳�� dialog ��ʹ����������
	  QDialog *dialog = new QDialog(this);
	������������Ȼ�����⣺1.QWidget��parent������QWidgetָ�룬���Բ��ܽ�һ����ͨ�� C++ ��ָ�봫�� Qt �Ի���
	2.����������Ϊ parent ʱ�������ڲ��رգ��Ի���Ͳ��ᱻ���٣�dialog��һֱռ���ڴ档
	���Ի�Ӧ�����öԻ���ر�ʱ���Զ����ٶԻ���
	  QDialog *dialog = new QDialog;
	  dialog->setAttribute(Qt::WA_DeleteOnClose);
	*/

	dialog->setWindowTitle(tr("Setting"));
	QObject::connect(dialog, &ModelessDialog::closeModelessDialog, this, &testQT_OPENCV::changeUserAge);
		/* ���ܽ���dialog�ر�ʱ���ͻ�����ֵ��������changeUserAge���������ڵ���ʾ */
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
	/* ʹ��exec()��ʾģ̬�Ի���dialog��ջ�ռ䴴������������ʱ���Զ�������(this)������������������parent����������dialogҲ�ᱻ����
	exec()��ʼ��һ���¼�ѭ�������뱻������exec()����δ����ʱ����Ĵ��벻�ᱻִ�С��Ի���رգ�exec()�������أ���ʱ�Ϳ���ȡ�öԻ�������ݡ�
	������� dialog ������ΪWA_DeleteOnClose����ô���Ի���ر�ʱ���������٣��Ͳ���ͨ��exec()�ķ���ֵ��ȡ�����ˡ�
	����������£����Կ���ʹ�� parent ָ��ķ�ʽ�����Ի��򣬱�������WA_DeleteOnClose����

	ֻ��ģ̬�Ի�����÷���ֵ��QDialog::Accepted����QDialog::Rejected��
	*/
}

void testQT_OPENCV::openImageClicked()
{
	image = cv::imread("image.jpg");//��ȡͼ��
	cv::namedWindow("Input image", 1);
	cv::imshow("Input image", image);
}
