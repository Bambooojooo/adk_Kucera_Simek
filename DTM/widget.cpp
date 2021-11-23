#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "edge.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    zmin = 100.0;
    zmax = 1000.0;
    dz = 5.0;

    ui->lineEdit->setText(QString::number(zmin));
    ui->lineEdit_2->setText(QString::number(zmax));
    ui->lineEdit_3->setText(QString::number(dz));
}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    //Clear all
    ui->Canvas->clear();
    repaint();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT
    Algorithms a;
    std::vector<Edge> dt = a.dT(points);

    //Set DT
    ui->Canvas->setDT(dt);

    repaint();

}

void Widget::on_pushButton_cleardt_clicked()
{
    //Clear DT
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_lineEdit_editingFinished()
{
    zmin = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_2_editingFinished()
{
    zmax = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished()
{
    dz = ui->lineEdit->text().toDouble();
}

void Widget::on_pushButton_3_clicked()
{
}

