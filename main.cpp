#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
int main(int argc, char *argv[]) {
        QApplication::addLibraryPath("./plugins");
        QTextCodec *codec = QTextCodec::codecForName("System");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
	QApplication a(argc, argv);
	MainWindow w;
	w.resize(800, 600);
	w.show();
	return a.exec();
}
