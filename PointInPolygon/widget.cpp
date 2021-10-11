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
    std::cout << "cus" << std::endl;

  //Analyze position of the point and vertex
    QPoint q = ui->Canvas->getPoint();
    std::vector<QPoint> pol = ui->Canvas->getPolygon();

    //Get position
    Algorithms a;
    int pos;

    if (ui->comboBox->currentText() == "Winding number")
	    pos = a.getPositionWinding(q, pol);
    else if (ui->comboBox->currentText() == "Ray crossing")
	    pos = a.getPositionRay(q, pol);

    //Print results
    if (pos == 1)
	ui->label->setText("Inside");
    else if (pos == 0)
	ui->label->setText("Outside");
    else
	ui->label->setText("On the border");


}
