#include "testqt_opencv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	testQT_OPENCV w;  // w������ջ�ռ䣬��֤������ MainWindow ��Ӧ�ùر�ʱ�ܹ���ȷ�ͷ�
					  // �����������w��Ϊ parent �ܹ����ӳ������Ϳ��Ա�֤ÿһ���������ȷ�ͷ�
	w.show();
	return a.exec();
}
