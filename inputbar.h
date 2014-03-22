#ifndef INPUTBAR_H
#define INPUTBAR_H

#include <QFrame>
#include <QPushButton>
#include <QString>
#include <QRect>
#include <QLineEdit>
#include "mainwindow.h"
namespace Ui {
class InputBar;
}
class MainWindow;
class InputBar: public QFrame {
	Q_OBJECT

public:
	explicit InputBar(QWidget *parent = 0);
	~InputBar();public slots:
	void returnBtnClicked();
	void okBtnClicked();
private:
	Ui::InputBar *ui;
	QPushButton *p_returnBtn;
	QPushButton *p_okBtn;
	MainWindow *p_parent;

};
#endif // INPUTBAR_H
