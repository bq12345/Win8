#include "mainwindow.h"
#include "dp.h"
#include "ga.h"
#include "conway.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <QMessageBox>
using namespace std;
vector<line> vec;
vector<conway> conways1;
MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	timer = new QTimer(this);
	show_num = 0;
	ui->setupUi(this);
	QPropertyAnimation *animation = new QPropertyAnimation(this,
			"windowOpacity");
	animation->setDuration(1500);
	animation->setStartValue(0.2);
	animation->setEndValue(1.0);
	animation->setEasingCurve(QEasingCurve::OutCubic);
	animation->start();
	m_font.setStyleHint(QFont::SansSerif);
	m_font.setPixelSize(16);
	m_palette.setColor(QPalette::ButtonText, QColor(240, 240, 240));
	//把窗体边框取消
	this->setWindowOpacity(1);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setMinimumSize(25, 45);
	p_closeBtn = new QPushButton(this);
	p_minBtn = new QPushButton(this);

	p_homeBtn = new QPushButton(this);
        p_homeBtn->setMinimumSize(48, 27);
        p_homeBtn->setGeometry(9, 9, 48, 27);
	p_returnBtn = new QPushButton(this);
	p_siderBar = new SideBar(this);
	p_inputBar = new InputBar(this);
	p_siderBar->setGeometry(-200, 20, 200, 560);
	p_aboutBar = new AboutBar(this);
	setInitStyle();
	//p_siderBar->setVisible(false);
	p_aboutBar->setVisible(false);
	p_returnBtn->setVisible(false);
	p_inputBar->setVisible(false);
	okButton = new QPushButton();
	lineEditx = new QLineEdit;
	lineEdity = new QLineEdit;
	delpoint = new QAction(tr("删除"), this);
	addpoint = new QAction(tr("新增"), this);
	startpoint = new QAction(tr("设置为断路起始点"), this);
	endpoint = new QAction(tr("设置为断路结束点"), this);
	wm = new QAction(tr("设置仓库"), this);
	this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
	QString str = ui->statusBar->tr("请点击选取坐标点，第一个点为仓库点");
	ui->statusBar->setSizeGripEnabled(FALSE);
	ui->statusBar->setStyleSheet(
			QString(
					"QStatusBar{color: white;background:transparent;font-size:14px;}"));
	ui->statusBar->showMessage(str);

	connect(timer, SIGNAL(timeout()), this, SLOT(showCar()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(systemAddPoint()));
	connect(delpoint, SIGNAL(triggered()), this, SLOT(delPoint()));
	connect(addpoint, SIGNAL(triggered()), this, SLOT(addPoint()));
	connect(startpoint, SIGNAL(triggered()), this, SLOT(startLine()));
	connect(endpoint, SIGNAL(triggered()), this, SLOT(endLine()));
	connect(p_closeBtn, SIGNAL(clicked()), this, SLOT(systemClose()));
	connect(p_minBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(p_homeBtn, SIGNAL(clicked()), this, SLOT(homeClicked()));
	connect(p_returnBtn, SIGNAL(clicked()), this, SLOT(returnClicked()));
	//初始化控件值
	if (p.isEmpty()) {
		ui->lineButton->setEnabled(false);
		ui->timeButton->setEnabled(false);
		ui->breakButton->setEnabled(false);
	}

}
//常用按钮点击事件及页面绘制
void MainWindow::systemClose() {
	this->close();
	if (d.isVisible()) {
		d.close();
	}
}
void MainWindow::setInitStyle() {

	ui->lineButton->setStyleSheet(
			"QPushButton{border-image: url(:/images/shortest.png);}"
					"QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/shortest.png);}");

	ui->timeButton->setStyleSheet(
			"QPushButton{border-image: url(:/images/time.png);}"
					"QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/time.png);}");
	ui->breakButton->setStyleSheet(
			"QPushButton{border-image: url(:/images/cut.png);}"
					"QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/cut.png);}");

	p_closeBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/close.png);}"
                                        "QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/close.png);}");

	p_minBtn->setStyleSheet("QPushButton{border-image: url(:/images/min.png);}"
                        "QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
			"QPushButton:pressed{border-image: url(:/images/min.png);}");

	p_homeBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/home.png);}"
					"QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
					"QPushButton:pressed{border-image: url(:/images/home.png);}");

	p_returnBtn->setStyleSheet(
			"QPushButton{border-image: url(:/images/return.png);}"
					"QPushButton:hover{border-image: url(:/images/return_on_2.png);}"
					"QPushButton:pressed{border-image: url(:/images/return.png);}");

}

void MainWindow::normal() {
	this->showNormal();
	this->update();

}

void MainWindow::homeClicked() {

	p_siderBar->show();
	QPropertyAnimation *anim = new QPropertyAnimation(p_siderBar, "geometry");
	anim->setDuration(600);
	anim->setStartValue(QRect(-200, 10, 200, 560));
	anim->setEndValue(QRect(10, 10, 200, 560));
	anim->setEasingCurve(QEasingCurve::OutBounce);
	anim->start();

}

void MainWindow::returnClicked() {
	p_homeBtn->setVisible(true);
	p_returnBtn->setVisible(false);
	setInitStyle();
}

void MainWindow::paintEvent(QPaintEvent *) {

        QPainter p1(this);
        QColor bgColor(60, 60, 60);
        QColor menuColor(90, 90, 90);
	if (this->width() > 45 && this->height() > 45)
		drawWindowShadow(p1);
	p1.setPen(Qt::NoPen);
	p1.setBrush(bgColor);
        p1.drawRoundedRect(QRect(9, 34, this->width() - 18, this->height() - 18),2.0f, 2.0f);
        p1.setBrush(menuColor);
        p1.drawRoundedRect(QRect(9, 9, this->width() - 18, 25),2.0f, 2.0f);

	g_mainWindowRect = geometry();
	int initWidth = g_mainWindowRect.width();
	int initHeight = g_mainWindowRect.height();
	p_closeBtn->setGeometry(initWidth - 35, 10, 25, 25);
	p_minBtn->setGeometry(initWidth - 65, 10, 25, 25);
	p_returnBtn->setGeometry(30, 30, 60, 60);
	pen1.setWidth(1);
	pen1.setStyle(Qt::DotLine);
	pen1.setColor(QColor(Qt::white));
	pen2.setWidth(5);
	pen2.setColor(QColor(Qt::white));
	pen3.setWidth(10);
	pen3.setColor(QColor(Qt::yellow));
	pen4.setWidth(4);
	pen4.setColor(QColor(Qt::green));
	QPainter paint(this);
	QPainterPath path;
	paint.setBrush(QBrush(QColor(0, 255, 0, 0)));
	QVector<QPoint>::iterator i = p.begin(); //从开头遍历
	QVector<QPoint>::iterator c = city.begin(); //从开头遍历
	QVector<QLine>::iterator q = cityline.begin(); //遍历所有预置线路
	int s = 0;
	paint.setPen(pen2);
	paint.drawPoint(*i);

	path.moveTo(*i);
	while (i != p.end()) {
		if (i == p.begin()) {
			paint.setPen(pen3);
			paint.drawPoint(*i);
		} else {
			paint.setPen(pen2);
			paint.drawPoint(*i);
		}
		i++;
	}
	while (c != city.end()) {

		paint.setPen(pen4);
		paint.drawPoint(*c);
		char ch[10];
		paint.drawText(*c, QString(itoa(city.indexOf(*c), ch, 10)));
		c++;
	}
	while (q != cityline.end()) {

		paint.setPen(pen1);
		paint.drawLine(*q);
		q++;
	}

	paint.setPen(pen3);

	while (s != show_timer.size()) {

		path.lineTo(p[(show_timer[s])]);

		char c[10];
		paint.drawText(p[(show_timer[s])], QString(itoa(result[s], c, 10)));
		paint.drawPixmap(p[(result[s])], QPixmap(":/images/close.png"));
		s++;
	}
	paint.setPen(pen1);
	paint.drawPath(path);

}

void MainWindow::drawWindowShadow(QPainter &p) {
	QList < QPixmap > pixmaps;
	pixmaps.append(QPixmap(":/resources/shadow_left.png"));
	pixmaps.append(QPixmap(":/resources/shadow_right.png"));
	pixmaps.append(QPixmap(":/resources/shadow_top.png"));
	pixmaps.append(QPixmap(":/resources/shadow_bottom.png"));
	pixmaps.append(QPixmap(":/resources/shadow_corner1.png"));
	pixmaps.append(QPixmap(":/resources/shadow_corner2.png"));
	pixmaps.append(QPixmap(":/resources/shadow_corner3.png"));
	pixmaps.append(QPixmap(":/resources/shadow_corner4.png"));
	//绘制主窗体
	p.drawPixmap(0, 0, 10, 10, pixmaps[4]);
	p.drawPixmap(this->width() - 10, 0, 10, 10, pixmaps[5]);
	p.drawPixmap(0, this->height() - 10, 10, 10, pixmaps[6]);
	p.drawPixmap(this->width() - 10, this->height() - 10, 10, 10, pixmaps[7]);

	p.drawPixmap(0, 10, 10, this->height() - 20,
			pixmaps[0].scaled(10, this->height() - 20));
	p.drawPixmap(this->width() - 10, 10, 10, this->height() - 20,
			pixmaps[1].scaled(10, this->height() - 20));
	p.drawPixmap(10, 0, this->width() - 20, 10,
			pixmaps[2].scaled(this->width() - 20, 10));
	p.drawPixmap(10, this->height() - 10, this->width() - 20, 10,
			pixmaps[3].scaled(this->width() - 20, 10));

}

const QFont& MainWindow::getButtonFont() const {
	return this->m_font;
}

const QPalette& MainWindow::getButtonPalette() const {
	return this->m_palette;
}
//重写鼠标点击事件
void MainWindow::mousePressEvent(QMouseEvent *event) {

        if (event->pos().y() < 40) {
		this->windowPos = event->pos();
		this->mousePos = event->globalPos();
		this->dPos = windowPos;

	} else if (event->button() == Qt::LeftButton && city.isEmpty()) {
		this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
		pos = event->pos();
		p.append(pos);
		ui->statusBar->showMessage(
				QString("x:%1,y:%2").arg(pos.x()).arg(pos.y()));
		this->update();
	} else {
		ps = event->pos();

	}

	if (d.isVisible() || p_aboutBar->isVisible() || p_siderBar->isVisible()) {
		d.close();
		p_aboutBar->setVisible(FALSE);
		p_siderBar->setVisible(FALSE);
	}
	ui->lineButton->setEnabled(true);
	ui->timeButton->setEnabled(true);
	ui->breakButton->setEnabled(true);
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	if (event->pos().y() < 30) {
		this->move(event->globalPos() - this->dPos);
	}
	QPoint pos = event->pos();

	ui->statusBar->showMessage(QString("x:%1,y:%2").arg(pos.x()).arg(pos.y()));
}
void MainWindow::contextMenuEvent(QContextMenuEvent *event)

{

	QCursor cur = this->cursor();
	menu = new QMenu(this);
	menu->setStyleSheet(QString("QMenu{color:white;background:#404040;}"));
	menu->addAction(delpoint);
	menu->addAction(addpoint);
	menu->addAction(startpoint);
	menu->addAction(endpoint);
	menu->exec(cur.pos());

}
/*
 菜单事件
 */

void MainWindow::startLine() {
	for (int j = (ps.x() - 10); j < (ps.x() + 10); j++) {
		for (int k = (ps.y() - 10); k < (ps.y() + 10); k++) {
			QVector<QPoint>::iterator i = p.begin(); //从开头遍历
			while (i != p.end()) {
				if (j == (*i).x() && k == (*i).y()) {
					linepoint = *i;
					this->update();
					break;
				}
				i++;
			}
		}
	}

	this->update();
	ui->statusBar->showMessage(
			QString("已设置x:%1,y:%2为断路起始点").arg(ps.x()).arg(ps.y()));

}
void MainWindow::endLine() {
	for (int j = (ps.x() - 10); j < (ps.x() + 10); j++) {
		for (int k = (ps.y() - 10); k < (ps.y() + 10); k++) {
			QVector<QPoint>::iterator i = p.begin(); //从开头遍历
			while (i != p.end()) {
				if (j == (*i).x() && k == (*i).y()) {
					QLine qline(linepoint, *i);
					ql.append(qline);
					this->update();
					break;
				}
				i++;
			}
		}
	}
	ui->statusBar->showMessage(
			QString("x:%1,y:%2为断路起始点,x:%3,y:%4为断路结束点,已加入该路段").arg(linepoint.x()).arg(
					linepoint.y()).arg(ps.x()).arg(ps.y()));

}
void MainWindow::addPoint() {
	p.append(ps);
	this->update();
}

void MainWindow::delPoint() {
	for (int j = (ps.x() - 10); j < (ps.x() + 10); j++) {
		for (int k = (ps.y() - 10); k < (ps.y() + 10); k++) {
			QVector<QPoint>::iterator i = p.begin(); //从开头遍历
			while (i != p.end()) {
				if (j == (*i).x() && k == (*i).y()) {
					p.erase(i);
					this->update();
					break;
				}
				i++;
			}
		}
	}
}

void MainWindow::clearWork() {
	show_timer.clear();
	result.clear();
	show_num = 0;
	p.clear();
        ql.clear();
	city.clear();
	cityline.clear();
	timer->stop();
	if (!(ui->breakButton->isVisible())) {
		ui->breakButton->setVisible(true);
		ui->breakButton->setEnabled(true);
	}
	this->update();

}

void MainWindow::systemAddPoint() {
	int x = lineEditx->text().toInt();
	int y = lineEdity->text().toInt();
	QPoint pp(x, y);
	p.append(pp);
	d.hide();
	this->update();

}
//输入框
void MainWindow::systemAdd() {

	d.resize(400, 300);
	d.setWindowFlags(Qt::FramelessWindowHint);
	d.setAttribute(Qt::WA_TranslucentBackground);
	QLabel *label = new QLabel(tr("输入x及y坐标"));
	label->setStyleSheet(
			"QLabel{background-color: rgb(40,40, 40);color: rgb(255, 255, 255);}");
	okButton->setDefault(true);
	QHBoxLayout *topLeftLayout = new QHBoxLayout;
	topLeftLayout->addWidget(label);
	lineEditx->setStyleSheet(
			"QLineEdit{background-color: rgb(40,40, 40);color: rgb(255, 255, 255);}");
	lineEdity->setStyleSheet(
			"QLineEdit{background-color: rgb(40,40, 40);color: rgb(255, 255, 255);}");
	topLeftLayout->addWidget(lineEditx);
	topLeftLayout->addWidget(lineEdity);
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addLayout(topLeftLayout);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	okButton->setStyleSheet("QPushButton{border-image: url(:/images/ok.png);}"
			"QPushButton:hover{background-image: url(:/images/btn_bg.png);}"
			"QPushButton:pressed{border-image: url(:/images/ok.png);}");
	okButton->setMinimumSize(140, 30);
	rightLayout->addWidget(okButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	d.setLayout(mainLayout);
	d.setWindowFlags(d.windowFlags() & ~Qt::WindowContextHelpButtonHint);
	d.show();

}

MainWindow::~MainWindow() {

	delete ui;
}

void MainWindow::showCar() {

	if (show_num < result.size()) {
		show_timer.push_back(result[show_num]);
		show_num++;
		this->update();
	}
	if (p[show_timer.back()] == linepoint) {
		timer->stop();
	}

}
//最短路径点击调用算法
void MainWindow::on_lineButton_clicked() {

	int npq[100][100];
	if (city.isEmpty()) {
		ql.clear();
		result.clear();
		show_timer.clear();
		show_num = 0;
		node pl;
		if (p.size() < 20) {
			result = cal(p, pl, min_ditance, 1, speed, ql, npq);
		} else {
			result = cal2(p, pl, min_ditance, 0, 1, 0, 0, ql, 1);

		}
		QString s;
		for (int i = 0; i < result.size(); i++) {
			cout << result[i] << " " << endl;
			char c[10];
			s += "->" + QString(itoa(result[i], c, 10));
		}
		ui->statusBar->showMessage("连接顺序：" + s);
		ui->linelabel->setText(QString("最短路径是：%1").arg(min_ditance));

		timer->start(20);
		this->update();
	} else {
		initCity();
		result.clear();
		show_timer.clear();
		show_num = 0;
		node pl;
		for (int i = 0; i < city.size(); i++) {
			p.push_back(city[i]);
		}
		int tance[100][100];
		for (int i = 0; i < 100; i++)
			for (int j = 0; j < 100; j++) {
				if (i != j)
					tance[i][j] = 10000000;
				else
					tance[i][j] = 0;
			}
		for (int i = 0; i < cityline.size(); i++) {
			int x;
			int y;
			for (int j = 0; j < p.size(); j++) {
				if (p[j].x() == cityline[i].x1()
						&& p[j].y() == cityline[i].y1()) {
					x = j;

				}
				if (p[j].x() == cityline[i].x2()
						&& p[j].y() == cityline[i].y2()) {
					y = j;
				}
			}

			tance[x][y] = sqrt(
					(p[x].x() - p[y].x()) * (p[x].x() - p[y].x())
							+ (p[x].y() - p[y].y()) * (p[x].y() - p[y].y()));
			tance[y][x] = tance[x][y];
		}

		for (int i = 0; i < p.size(); i++)
			for (int j = 0; j < p.size(); j++) {
				if (tance[i][j] == 10000000) {
					vector<int> pe;
					int tempdis;

					pe = caldij(p.size(), i + 1, j + 1, tance, tempdis);
					conway conwaynew;
					conwaynew.x1 = i;
					conwaynew.x2 = j;
					conwaynew.replaceway = pe;
					conways1.push_back(conwaynew);

					pe.clear();
					tance[i][j] = tempdis;

				}
			}

		vector<int> temp;
		temp = cal(p, pl, min_ditance, 4, speed, ql, tance);

		for (int i = 0; i < temp.size() - 1; i++) {
			int x1 = temp[i];
			int x2 = temp[i + 1];

			int flag = 0;
			for (int j = 0; j < conways1.size(); j++) {

				if (conways1[j].x1 == x1 && conways1[j].x2 == x2) {

					for (int k = 0; k < conways1[j].replaceway.size() - 1;
							k++) {
						result.push_back(conways1[j].replaceway[k]);
						cout << conways1[j].replaceway[k] << " ";
					}
					flag++;
				}

			}
			if (flag == 0)

			{
				result.push_back(x1);
				cout << x1;
			}
		}
		conways1.clear();
		result.push_back(temp[temp.size() - 1]);
		QString s;
		for (int i = 0; i < result.size(); i++) {
			cout << result[i] << " " << endl;
			char c[10];
			s += "->" + QString(itoa(result[i], c, 10));
		}
		ui->statusBar->showMessage("连接顺序：" + s);
		ui->linelabel->setText(QString("最短路径是：%1").arg(min_ditance));
		cityline.clear();
		timer->start(1020);

		this->update();

	}

}

void MainWindow::on_timeButton_clicked() {
	if (!city.isEmpty()) {
		on_lineButton_clicked();
	} else {
		speed = ui->speedEdit->text().toInt();
		result.clear();
		show_timer.clear();
		show_num = 0;
		node pl;
		int npq[100][100];

		if (p.size() < 20) {
			result = cal(p, pl, min_time, 2, speed, ql, npq);
			//min_time = 60 * min_time;
			ui->timelabel->setText(QString("最短时间是：%1").arg(min_time));
		}

		else {
			result = cal2(p, pl, min_time, 70, 0, 1, 0, ql, 1);
			//min_time = 60 * min_time;
			ui->timelabel->setText(QString("最短时间是：%1").arg(min_time * 2 / 50));
		}
		QString s;
		for (int i = 0; i < result.size(); i++) {
			cout << result[i] << " " << endl;
			char c[10];
			s += "->" + QString(itoa(result[i], c, 10));
		}
		ui->statusBar->showMessage("连接顺序：" + s);

		timer->start(20);
	}
}
void MainWindow::on_breakButton_clicked() {
	if (ql.isEmpty()) {
		int npq[100][100];
		ql.clear();
		result.clear();
		show_timer.clear();
		show_num = 0;
		node pl;
		if (p.size() < 20) {
			result = cal(p, pl, min_ditance, 1, speed, ql, npq);
		} else {
			result = cal2(p, pl, min_ditance, 0, 1, 0, 0, ql, 1);

		}
		QString s;
		for (int i = 0; i < result.size(); i++) {
			cout << result[i] << " " << endl;
			char c[10];
			s += "->" + QString(itoa(result[i], c, 10));
		}
		ui->statusBar->showMessage("连接顺序：" + s);
		ui->linelabel->setText(QString("最短路径是：%1").arg(min_ditance));
		timer->start(1200);
		this->update();
	} else {
		vector<int> saveresult = result;
		result.clear();
		int start = show_timer[show_timer.size() - 1];
		int startplace = show_timer.size();
		int changevalue[100]; //储存经过改变以后的编号的原有编号；
		QVector < QPoint > newpoint; //储存剩下的点
		newpoint.push_back(p[0]); //将起始点放进剩余点中
		changevalue[0] = 0; //起始点的原有编号为0
		int m = 1; //剩余的点的个数
		for (int i = 0; i < p.size(); i++) {
			int flaga = 0;
			for (int j = 0; j < show_timer.size(); j++) {
				if (show_timer[j] == i) {
					flaga = 1;
					break;
				}
			}
			if (flaga == 0) {
				changevalue[m++] = i;
				newpoint.push_back(p[i]);
			}
		} //提取剩余的点
		newpoint.push_back(p[show_timer[show_timer.size() - 1]]); //将断路起始点压入剩余点中
		changevalue[m++] = show_timer[show_timer.size() - 1]; //断路起始点编号

		vector<int> temp;
		for (int i = 0; i < show_timer.size(); i++) {
			result.push_back(show_timer[i]);

		} //储存前半段结果

		int x, y; //找出断路的编号

		for (int j = 0; j < p.size(); j++) {
			if (p[j].x() == ql[0].x1() && p[j].y() == ql[0].y1()) {
				x = j;

			}
			if (p[j].x() == ql[0].x2() && p[j].y() == ql[0].y2()) {
				y = j;
			}
		}
		int Distance[100][100]; //储存大图的距离矩阵
		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.size(); j++) {
				Distance[i][j] = sqrt(
						(p[i].x() - p[j].x()) * (p[i].x() - p[j].x())
								+ (p[i].y() - p[j].y())
										* (p[i].y() - p[j].y()));

			}

		}
		int old_distan[100][100];
		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.size(); j++) {
				old_distan[i][j] = Distance[i][j];

			}

		}

		//在大图中寻找替代路径
		Distance[x][y] = 1000000;
		Distance[y][x] = 1000000;
		vector<int> pe;
		int tempdis;

		pe = caldij(p.size(), x + 1, y + 1, Distance, tempdis);
		conway conwaynew;
		conwaynew.x1 = x;
		conwaynew.x2 = y;
		conwaynew.replaceway = pe;
		conways.push_back(conwaynew);

		pe.clear();

		//找到断路点所对应的现在序号
		int tempx;
		int tempy;
		for (int i = 0; i < m; i++) {
			if (changevalue[i] == x)
				tempx = i;
			if (changevalue[i] == y)
				tempy = i;
		}
		node denode;
		denode.x = tempx;
		denode.y = tempy;
		denode.distance = tempdis;

		show_timer.clear();
		show_num = 0;

		int old_distance;
		if (newpoint.size() < 20) {
			temp = cal(newpoint, denode, old_distance, 3, speed, ql, Distance);
		} else {
			temp = cal2(newpoint, denode, old_distance, 0, 1, 0, 0, ql, 2);
		}

		if (changevalue[temp[temp.size() - 2]] != start) {
			if (y == saveresult[startplace]) {
				vector<int> temp1;
				for (int i = 0; i < saveresult.size(); i++) {
					if (saveresult[i] == start) {
						for (int i = 0; i < conwaynew.replaceway.size(); i++) {
							temp1.push_back(conwaynew.replaceway[i]);
						}
						int place1 = i + 1;
						for (int j = 0; j < temp1.size(); j++) {
							if (temp1[j] == saveresult[place1]) {
								i++;
								break;
							}
						}
					} else {
						temp1.push_back(saveresult[i]);
					}
				}

				result = temp1;
			} else
				result = saveresult;
		} else {
			if (changevalue[temp[temp.size() - 3]] == y) {
				for (int i = 1; i < conwaynew.replaceway.size(); i++) {
					result.push_back(conwaynew.replaceway[i]);
				}
				if (temp.size() > 4) {
					int yui = 0;

					for (int j = 0; j < result.size(); j++) {
						yui++;
						break;
					}
					if (!yui) {

						for (int i = temp.size() - 5; i >= 0; i--) {

							result.push_back(changevalue[temp[i]]);
							cout << changevalue[temp[i]] << "->";
						}

					} else {
						for (int i = temp.size() - 4; i >= 0; i--) {

							result.push_back(changevalue[temp[i]]);
							cout << changevalue[temp[i]] << "->";
						}
					}

				} else {
					result.push_back(0);
				}

			} else {
				for (int i = temp.size() - 3; i >= 0; i--) {

					result.push_back(changevalue[temp[i]]);
					cout << changevalue[temp[i]] << "->";
				}
			}

		}

		int old_distance2 = 0;
		for (int i = 0; i < result.size() - 1; i++) {
			old_distance2 = old_distance2
					+ old_distan[result[i]][result[i + 1]];
		}

		QString s;
		for (int i = 0; i < result.size(); i++) {
			cout << result[i] << " " << endl;
			char c[10];
			s += "->" + QString(itoa(result[i], c, 10));
		}
		ui->statusBar->showMessage("连接顺序：" + s);
		ui->linelabel->setText(QString("最短路径是：%1").arg(old_distance2));
		linepoint.setX(0);
		ql.clear();
		timer->start(1200);
	}
}
void MainWindow::advanceAlgo(double x, double y, double z, int a, int b,
		double c, double d) {
	ui->breakButton->setVisible(false);
	result.clear();
	show_timer.clear();
	show_num = 0;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;
	node pl;
	speed = ui->speedEdit->text().toInt();
	result = cal2(p, pl, min_ditance, speed, x, y, z, ql, 1, a, b, c, d);

	double mixdistance;
	double mixtime;
	double mixprice;
	if (x != 0)
		mixdistance = min_ditance * x;
	else
		mixdistance = 0;
	if (y != 0)
		mixtime = min_ditance * y;
	else
		mixtime = 0;
	if (z != 0)
		mixprice = min_ditance * z;
	else
		mixprice = 0;

	ui->out->setText(QString("总权值是：%1").arg(min_ditance));
	QString s;
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " " << endl;
		char c[10];
		s += "->" + QString(itoa(result[i], c, 10));
	}
	ui->statusBar->showMessage("连接顺序：" + s);
	timer->start(100);

	this->update();

}
void MainWindow::initCity() {
	clearWork();
	city.push_back(QPoint(40, 168));
	city.push_back(QPoint(523, 133));
	city.push_back(QPoint(480, 200));
	city.push_back(QPoint(407, 244));
	city.push_back(QPoint(212, 283));
	city.push_back(QPoint(301, 321));
	city.push_back(QPoint(363, 310));
	city.push_back(QPoint(83, 385));
	city.push_back(QPoint(242, 373));
	city.push_back(QPoint(225, 450));
	city.push_back(QPoint(297, 479));
	city.push_back(QPoint(370, 373));
	city.push_back(QPoint(465, 450));
	cityline.push_back(QLine(city[1], city[2]));
	line newline1(1, 2);
	vec.push_back(newline1);
	cityline.push_back(QLine(city[2], city[3]));
	line newline2(2, 3);
	vec.push_back(newline2);
	cityline.push_back(QLine(city[3], city[6]));
	line newline3(3, 6);
	vec.push_back(newline3);
	cityline.push_back(QLine(city[6], city[5]));
	line newline4(6, 5);
	vec.push_back(newline4);
	cityline.push_back(QLine(city[6], city[11]));
	line newline5(6, 11);
	vec.push_back(newline5);
	cityline.push_back(QLine(city[5], city[4]));
	line newline6(5, 4);
	vec.push_back(newline6);
	cityline.push_back(QLine(city[4], city[0]));
	line newline7(4, 0);
	vec.push_back(newline7);
	cityline.push_back(QLine(city[5], city[8]));
	line newline8(5, 8);
	vec.push_back(newline8);
	cityline.push_back(QLine(city[8], city[7]));
	line newline9(8, 7);
	vec.push_back(newline9);
	cityline.push_back(QLine(city[8], city[9]));
	line newline10(8, 9);
	vec.push_back(newline10);
	cityline.push_back(QLine(city[8], city[10]));
	line newline11(8, 10);
	vec.push_back(newline11);
	cityline.push_back(QLine(city[5], city[11]));
	line newline12(5, 11);
	vec.push_back(newline12);
	cityline.push_back(QLine(city[9], city[10]));
	line newline13(9, 10);
	vec.push_back(newline13);
	cityline.push_back(QLine(city[10], city[11]));
	line newline14(10, 11);
	vec.push_back(newline14);
	cityline.push_back(QLine(city[11], city[12]));
	line newline15(11, 12);
	vec.push_back(newline15);
	cityline.push_back(QLine(city[10], city[12]));
	line newline16(10, 12);
	vec.push_back(newline16);
	ui->breakButton->setVisible(false);
	this->update();
}
