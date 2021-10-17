#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "csv.h"
#include "draw.h"

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
    int pos;
    QString Alg = ui->comboBox->currentText();

    pos = a.processPolygons(q, pols, Alg);

//    if (a.ifCloseToPoint(q, pol))
//	    pos = 2;
//    else
//    {
//	    if (ui->comboBox->currentText() == "Winding number")
//		    pos = a.getPositionWinding(q, pol);
//	    else if (ui->comboBox->currentText() == "Ray crossing")
//		    pos = a.getPositionRay(q, pol);
//    }


    //Print results
    if (pos == 1)
	ui->label->setText("Inside");
    else if (pos == 0)
	ui->label->setText("Outside");
    else if (pos == -1)
	ui->label->setText("On the border");
    else if (pos == 2)
	ui->label->setText("On the vertex");


}

void Widget::on_pushButtonLoad_clicked()
{
	// Read three_cols.csv and ones.csv
//	std::vector<std::pair<std::string, std::vector<int>>> three_cols = read_csv("three_cols.csv");
//	std::vector<std::pair<std::string, std::vector<int>>> ones = read_csv("ones.csv");

	CSV csvObject;

	std::string filename = "csv_body.csv";

	std::vector<QPolygon> polygonVector = csvObject.read_csv(filename);

//	int n = polygonVector.size();
//	std::cout << "Printing result with size: " << n << std::endl;
//	for (int i = 0; i < n; i++)
//	{
//			std::cout << "Polygon: [x="  << polygonVector[i].x() << ",y=" << polygonVector[i].y() << "]" << std::endl;
//	}
	//Polygonstatus changes add_polygon bool to true, cause we are painting polygons right now
	ui->Canvas->polygonStatus();

	//Draw polygons
	ui->Canvas->drawPolygons(polygonVector);

	//Change add_polygon back to false
	ui->Canvas->polygonStatus();
}

