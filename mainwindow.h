#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QtGui>
#include <QPushButton>
#include <QRect>
#include <QString>
#include <QPoint>
#include "sidebar.h"
#include "aboutbar.h"
#include "inputbar.h"
#include <QVector>
#include <qiterator.h>
#include <QLine>
#include <QPen>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QStatusBar>
#include <QtCore>
#include <QPoint>
#include <QBrush>
#include <iostream>
#include <QPointF>
#include <QTextCodec>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;
namespace Ui {
class MainWindow;
}
class SideBar;
class InputBar;
class MainWindow: public QMainWindow {
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void contextMenuEvent(QContextMenuEvent *event);public slots:
	void normal();
	void homeClicked();
	void returnClicked();
	void systemAdd();
	void systemAddPoint();
	void systemClose();
	void clearWork();
	void delPoint();
	void addPoint();
	void startLine();
	void endLine();
	void on_lineButton_clicked();
	void on_timeButton_clicked();
	void on_breakButton_clicked();
	void showCar();

public:
	explicit MainWindow(QWidget *parent = 0);
	void drawWindowShadow(QPainter &p);
	void setInitStyle();
	void initCity();
	const QFont& getButtonFont() const;
	const QPalette& getButtonPalette() const;
	void advanceAlgo(double x, double y, double z, int a, int b, double c,
			double d);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QRect g_mainWindowRect;
	QPoint windowPos;
	QPoint dPos;
	QPoint mousePos;
protected:
	QFont m_font;
	QPalette m_palette;
public:
	QPushButton *p_closeBtn;
	QPushButton *p_minBtn;
	QPushButton *p_homeBtn;
	QPushButton *p_returnBtn;
	SideBar *p_siderBar;
	AboutBar *p_aboutBar;
	InputBar *p_inputBar;
	QVector<QPoint> p;
	QVector<QPoint> city;
	QVector<QLine> ql, cityline;
	QPen pen1, pen2, pen3, pen4;
	QPoint pos, ps, linepoint;
	QDialog d;
	QPushButton *okButton;
	QLineEdit *lineEditx;
	QLineEdit *lineEdity;
	QAction *delpoint, *addpoint, *wm, *startpoint, *endpoint;
	QMenu *menu;
	vector<int> result;
	vector<int> show_timer;
	QLabel *la;
	int speed, min_ditance, min_time;
	QPainter p1;
	QTimer *timer;
	int show_num;
};

#endif // MAINWINDOW_H
