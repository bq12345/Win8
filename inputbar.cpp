#include "inputbar.h"
#include "ui_inputbar.h"
#include "conway.h"
InputBar::InputBar(QWidget *parent) :
		QFrame(parent), ui(new Ui::InputBar), p_parent(
				static_cast<MainWindow*>(this->parentWidget()))

{
	ui->setupUi(this);
	setStyleSheet("QFrame{border-image: url(:/images/sidebar_bg.png);}");
	p_returnBtn = new QPushButton(this);
	p_returnBtn->setGeometry(10, 20, 35, 35);
	this->setGeometry(10, 10, 200, 560);
	p_returnBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/return.png);}"
					"QPushButton:hover{border-image: url(:/images/return_on.png);}"
					"QPushButton:pressed{border-image: url(:/images/return.png);}");
	ui->okButton->setStyleSheet(
			"QPushButton{border-image: url(:/images/okBtn.png);}"
					"QPushButton:hover{background-image: url(:/images/bg.png);}"
					"QPushButton:pressed{background-image: url(:/images/bg.png);}");

	connect(p_returnBtn, SIGNAL(clicked()), this, SLOT(returnBtnClicked()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okBtnClicked()));
}

InputBar::~InputBar() {
	delete ui;
}

void InputBar::returnBtnClicked() {
	this->setVisible(false);
}
void InputBar::okBtnClicked() {
	double x = ui->distance->text().toDouble();
	double y = ui->time->text().toDouble();
	double z = ui->cost->text().toDouble();
	int a = ui->pop_size->text().toInt();
	int b = ui->generation->text().toInt();
	double c = ui->crossover->text().toDouble();
	double d = ui->mutation->text().toDouble();
	p_parent->advanceAlgo(x, y, z, a, b, c, d);
	this->setVisible(false);
}

