#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication::setApplicationName("Dragon's Drop");
	QApplication::setOrganizationName(QApplication::applicationName());
	
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	
	return a.exec();
}
