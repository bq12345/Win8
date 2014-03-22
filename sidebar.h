#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QPushButton>
#include <QString>
#include <QRect>

#include "mainwindow.h"

namespace Ui {
class SideBar;
}

class MainWindow;
class SideBar: public QFrame {
	Q_OBJECT

public:
	explicit SideBar(QWidget *parent = 0);
	~SideBar();public slots:
	void returnBtnClicked();
	void showInput();
	void aboutBtnClicked();
	void addBtnClicked();
	void clearBtnClicked();
	void mapBtnClicked();

private:
	Ui::SideBar *ui;

	QPushButton *p_returnBtn;
	QPushButton *p_AboutBtn;
	QPushButton *p_mapBtn;
	QPushButton *p_QuitBtn;
	QPushButton *p_AddBtn;
	QPushButton *p_HighBtn;
	QPushButton *p_ClearBtn;
	MainWindow *p_parent;
};

#endif // SIDEBAR_H
