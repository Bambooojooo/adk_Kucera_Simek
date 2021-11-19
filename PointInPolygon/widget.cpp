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
    //Destructor
    delete ui;
}


void Widget::on_pushButtonClear_clicked()
{
    //Clear canvas by calling clear method (Draw class)
    ui->Canvas->clear();
}

void Widget::on_pushButton_clicked()
{
    //Just switch between point and vertices drawing
    ui->Canvas->changeStatus();
}

void Widget::on_pushButtonAnalyze_clicked()
{

  //Analyze position of the point and vertex
    QPoint q = ui->Canvas->getPoint();
    QString Alg = ui->comboBox->currentText();
    QPolygon pol;
    std::vector<int> results;
    std::vector<QPoint> vertices = ui->Canvas->getVertices();

    //Make polygon from vertices and add them to pols for processing
    for (QPoint vertice : vertices)
	pol << vertice;
    //pols.push_back(pol);

    ui->Canvas->pushPolygon(pol);
    std::vector<QPolygon> pols = ui->Canvas->getPolygons();

    //Get position
    Algorithms a;
    int pos;

    //Iterate trough all polygons to analyze position of the point and return position. Also fill results vector with position of the point to every single polygon
    pos = a.processPolygons(q, pols, Alg, results);
    std::cout << "pos: " << pos << std::endl;

    //Store position of the point to every single polygon
    ui->Canvas->addResults(results);
    results.clear();


//    //Print results
//    if (pos == 1)
//	ui->label->setText("Inside");
//    else if (pos == 0)
//	ui->label->setText("Outside");
//    else if (pos == -1)
//	ui->label->setText("On the border");
//    else if (pos == 2)
//	ui->label->setText("On the vertex");

    ui->Canvas->repaint();
}

void Widget::on_pushButtonLoad_clicked()
{
	CSV csvObject;

	//Open dialog to choose data file and store its path to QString
	QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("CSV Files (*.csv)")));

	//Convert QString path to string path
	std::string filename = path.toStdString();

	//Read the file with chosen path
	std::vector<QPolygon> polygonVector = csvObject.read_csv(filename);

	//Draw polygons
	ui->Canvas->drawPolygons(polygonVector);
}

