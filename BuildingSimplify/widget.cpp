#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
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


void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clear();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint> points = ui->Canvas->getPoints();

    //Create enclosing rectangle
    Algorithms a;
    QPolygon er;

    if (ui->comboBox->currentText() == "Minimum Bounding Rectangle")
    {
        //Minimum area enclosing rectangle
        QPolygon ch = a.cHull(points);
        er = a.minAreaEnclosingRectangle(points);
        ui->Canvas->setCh(ch);
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
    ui->Canvas->setEr(er);

    //Repaint
    repaint();
}
