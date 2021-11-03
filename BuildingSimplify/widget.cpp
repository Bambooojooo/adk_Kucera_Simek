#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "csv.h"

#include <string>
#include <iostream>
#include <QFileDialog>

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


void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearDrawing();
}

void Widget::on_pushButton_clear_data_clicked()
{
	ui->Canvas->clearData();
}

void Widget::processPoints(std::vector <QPoint> &points)
{
	//Create enclosing rectangle
	Algorithms a;
	QPolygon er;

	if (ui->comboBox->currentText() == "Minimum Bounding Rectangle")
	{

	    //Minimum area enclosing rectangle
	    QPolygon ch = a.cHull(points);
	    er = a.minAreaEnclosingRectangle(points);
	    ui->Canvas->addCh(ch);
	}

	else if (ui->comboBox->currentText() == "Wall Average")
	{
	    //Wall average
	    er = a.wallAverage(points);
	}

	else if (ui->comboBox->currentText() == "Longest Edge")
	{
	    //Longest Edge
	    er = a.longestEdge(points);
	}

	else if (ui->comboBox->currentText() == "Weighted Bisector")
	{
	    //Weighted Bisector
	    er = a.weightedBisector(points);
	}

	//Update enclosing rectangle
	ui->Canvas->addEr(er);

	//Repaint
	repaint();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint> points = ui->Canvas->getPoints();
    std::vector<QPolygon> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    std::vector<QPoint> polygon_points;

    //Process drawn points
    if (points.size() > 1)
	processPoints(points);

    //Process loaded data
    if (polygons.size() > 0)
	    for (QPolygon polygon : polygons)
	    {
		    polygon_points.clear();
		    for (QPoint point : polygon)
			    polygon_points.push_back(point);

		    if (polygon_points.size() > 0)
			processPoints(polygon_points);
	    }

    ui->Canvas->clearChs();
    ui->Canvas->clearErs();
}

void Widget::on_pushButton_load_clicked()
{
	//Open dialog to choose data file and store its path to QString
	QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("CSV Files (*.csv)")));

	//Convert QString path to string path
	std::string filename = path.toStdString();

	//Read the file with chosen path
	std::vector<QPolygon> polygon_vector = CSV::read_csv(filename);

	//Draw polygons
	ui->Canvas->drawPolygons(polygon_vector);
}

void Widget::createHulls(std::vector <QPoint> &points)
{
	//Create enclosing rectangle
	Algorithms a;
	QPolygon ch;
	std::cout << "3" << std::endl;

	if (ui->comboBox_HullsMethods->currentText() == "Jarvis Scan")
	{
	    //Jarvis Scan
	    std::cout << "2" << std::endl;
	    ch = a.cHull(points);
	}

	else if (ui->comboBox_HullsMethods->currentText() == "Graham Scan")
	{
	    //Graham Scan
	    ch = a.cHullGraham(points);
	    std::cout << "1" << std::endl;
	}

	//Update enclosing rectangle
	ui->Canvas->addCh(ch);

	//Repaint
	repaint();
}

void Widget::on_pushButton_createHulls_clicked()
{
	//Get points
	std::vector<QPoint> points = ui->Canvas->getPoints();
	std::vector<QPolygon> polygons = ui->Canvas->getPolygons();

	std::cout << "clicked" << std::endl;

	//Create enclosing rectangle
	Algorithms a;
	std::vector<QPoint> polygon_points;

	//Process drawn points
	if (points.size() > 1)
	    createHulls(points);

	//Process loaded data
	if (polygons.size() > 0)
		for (QPolygon polygon : polygons)
		{
			polygon_points.clear();
			for (QPoint point : polygon)
				polygon_points.push_back(point);

			if (polygon_points.size() > 0)
			    createHulls(polygon_points);
		}
	ui->Canvas->clearChs();
}

