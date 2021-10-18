#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "csv.h"
#include "draw.h"

#include <QFileDialog>
#include <string>
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButtonClear_clicked()
{
    ui->Canvas->clear();
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->changeStatus();
}

void Widget::on_pushButtonAnalyze_clicked()
{

  //Analyze position of the point and vertex
    QPoint q = ui->Canvas->getPoint();
    std::vector<QPoint> pol = ui->Canvas->getVertices();
    std::vector<QPolygon> pols = ui->Canvas->getPolygons();

    //Get position
    Algorithms a;
    Draw drawObject;
    int pos;
    QString Alg = ui->comboBox->currentText();
    std::vector<int> results;

    pos = a.processPolygons(q, pols, Alg, results);
    ui->Canvas->addResults(results);
    results.clear();


    //Print results
    if (pos == 1)
	ui->label->setText("Inside");
    else if (pos == 0)
	ui->label->setText("Outside");
    else if (pos == -1)
	ui->label->setText("On the border");
    else if (pos == 2)
	ui->label->setText("On the vertex");

    ui->Canvas->repaint();
}

void Widget::on_pushButtonLoad_clicked()
{
	CSV csvObject;

	QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("CSV Files (*.csv)")));
	std::string filename = path.toStdString();

	std::vector<QPolygon> polygonVector = csvObject.read_csv(filename);

	//Polygonstatus changes add_polygon bool to true, cause we are painting polygons right now
	ui->Canvas->polygonStatus();

	//Draw polygons
	ui->Canvas->drawPolygons(polygonVector);

	//Change add_polygon back to false
	ui->Canvas->polygonStatus();
}

