#include "testqt_opencv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	testQT_OPENCV w;  // w创建在栈空间，保证了最顶层的 MainWindow 在应用关闭时能够正确释放
					  // 并且子组件将w作为 parent 能够连接成链，就可以保证每一个组件被正确释放
	w.show();
	return a.exec();
}
