#include "sidebar.h"
#include "ui_sidebar.h"
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPainter>
#include <QRect>
#include <QSize>

SideBar::SideBar(QWidget *parent) :
		QFrame(parent), ui(new Ui::SideBar), p_parent(
				static_cast<MainWindow*>(this->parentWidget())) {
	ui->setupUi(this);
	setStyleSheet("QFrame{border-image: url(:/images/sidebar_bg.png);}");
	p_returnBtn = new QPushButton(this);
	p_AddBtn = new QPushButton(this);
	p_mapBtn = new QPushButton(this);
	p_ClearBtn = new QPushButton(this);
	p_HighBtn = new QPushButton(this);
	p_AboutBtn = new QPushButton(this);
	p_QuitBtn = new QPushButton(this);
	p_returnBtn->setGeometry(10, 20, 35, 35);

	p_AddBtn->setGeometry(0, 125, 140, 45);
	p_HighBtn->setGeometry(0, 175, 140, 45);
	p_mapBtn->setGeometry(0, 225, 140, 45);
	p_ClearBtn->setGeometry(0, 275, 140, 45);
	p_AboutBtn->setGeometry(0, 325, 140, 45);
	p_QuitBtn->setGeometry(0, 375, 140, 45);
	QFont font = p_parent->getButtonFont();
	;
	QPalette palette = p_parent->getButtonPalette();

	p_AddBtn->setText("增加点");
	p_AddBtn->setFont(font);
	p_AddBtn->setPalette(palette);

	p_HighBtn->setText("高级");
	p_HighBtn->setFont(font);
	p_HighBtn->setPalette(palette);

	p_mapBtn->setText("载入地图");
	p_mapBtn->setFont(font);
	p_mapBtn->setPalette(palette);

	p_ClearBtn->setText("清除");
	p_ClearBtn->setFont(font);
	p_ClearBtn->setPalette(palette);

	p_AboutBtn->setText("关于");
	p_AboutBtn->setFont(font);
	p_AboutBtn->setPalette(palette);

	p_QuitBtn->setText("退出");
	p_QuitBtn->setFont(font);
	p_QuitBtn->setPalette(palette);

	p_returnBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/return.png);}"
					"QPushButton:hover{border-image: url(:/images/return_on.png);}"
					"QPushButton:pressed{border-image: url(:/images/return.png);}");

	p_AddBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	p_mapBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	p_HighBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	p_ClearBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	p_AboutBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	p_QuitBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/btn_bg.png);}"
					"QPushButton:hover{border-image: url(:/images/bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/bg.png);}");

	connect(p_AddBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
	connect(p_mapBtn, SIGNAL(clicked()), this, SLOT(mapBtnClicked()));
	connect(p_HighBtn, SIGNAL(clicked()), this, SLOT(showInput()));
	connect(p_ClearBtn, SIGNAL(clicked()), this, SLOT(clearBtnClicked()));
	connect(p_returnBtn, SIGNAL(clicked()), this, SLOT(returnBtnClicked()));
	connect(p_AboutBtn, SIGNAL(clicked()), this, SLOT(aboutBtnClicked()));
	connect(p_QuitBtn, SIGNAL(clicked()), p_parent, SLOT(close()));

}

SideBar::~SideBar() {
	delete ui;
}
void SideBar::mapBtnClicked() {

	p_parent->initCity();
	this->setVisible(false);
}
void SideBar::returnBtnClicked() {
	this->setVisible(false);
	QPropertyAnimation *anim = new QPropertyAnimation(this, "geometry");
	anim->setDuration(600);
	anim->setStartValue(QRect(0, 20, 200, 560));
	anim->setEndValue(QRect(-200, 20, 200, 560));
	anim->setEasingCurve(QEasingCurve::OutBounce);
	anim->start();
}

void SideBar::showInput() {
	p_parent->p_inputBar->setVisible(true);
	this->setVisible(false);
}

void SideBar::aboutBtnClicked() {
	p_parent->p_aboutBar->setVisible(true);
}

void SideBar::addBtnClicked() {
	this->setVisible(false);
	p_parent->systemAdd();
}
void SideBar::clearBtnClicked() {
	this->setVisible(false);
	p_parent->clearWork();
}

