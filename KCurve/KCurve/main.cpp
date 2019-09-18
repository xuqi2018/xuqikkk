#include "KCurve1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KCurve1 w;
	w.show();
	return a.exec();
}
