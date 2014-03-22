#include "aboutbar.h"
#include "ui_aboutbar.h"

AboutBar::AboutBar(QWidget *parent) :
		QFrame(parent), ui(new Ui::AboutBar) {
	ui->setupUi(this);
	setStyleSheet("QFrame{border-image: url(:/images/sidebar_bg.png);}");

	p_returnBtn = new QPushButton(this);
	p_returnBtn->setGeometry(10, 20, 35, 35);
	this->setGeometry(10, 10, 200, 560);
	p_returnBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/return.png);}"
					"QPushButton:hover{border-image: url(:/images/return_on.png);}"
					"QPushButton:pressed{border-image: url(:/images/return.png);}");

	connect(p_returnBtn, SIGNAL(clicked()), this, SLOT(returnBtnClicked()));
}

AboutBar::~AboutBar() {
	delete ui;
}

void AboutBar::returnBtnClicked() {
	this->setVisible(false);
}
