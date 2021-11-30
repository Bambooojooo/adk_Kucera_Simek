#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "edge.h"
#include <iostream>
#include <QtGui>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    zmin = 100;
    zmax = 1000.0;
    dz = 50;
    k = 5;
    n_points = 100;

    ui->lineEdit->setText(QString::number(zmin));
    ui->lineEdit_2->setText(QString::number(zmax));
    ui->lineEdit_3->setText(QString::number(dz));
    ui->lineEdit_4->setText(QString::number(k));
    ui->lineEdit_5->setText(QString::number(n_points));

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
    zmax = ui->lineEdit_2->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished()
{
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_3_clicked()
{
    //Create contours
    std::vector<Edge> dt = ui->Canvas->getDT();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        Algorithms a;
        //Create contours

//        zmin = ui->lineEdit->text().toDouble();
        zmax = ui->lineEdit_2->text().toDouble();

        std::vector<Edge> contours = a.getContourLines(dt, zmin, zmax, dz);

        std::cout << "zmin " << zmin << " zmax " << zmax << std::endl;

        //Set contours
        ui->Canvas->setContours(contours);
        ui->Canvas->setdZ(dz);
        ui->Canvas->setK(k);
        ui->Canvas->setz_min(zmin);

        repaint();
    }

}


void Widget::on_pushButton_4_clicked()
{
    //Draw slope
    std::vector<Edge> dt = ui->Canvas->getDT();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        Algorithms a;
        //Analyze DTM
        std::vector<Triangle> triangles = a.analyzeDTM(dt);

        //Set triangles
        ui->Canvas->setTriangles(triangles);
        QString col = ui->comboBox_2->currentText();
        ui->Canvas->setColor(col);

        repaint();
    }
}

void Widget::on_lineEdit_4_editingFinished()
{
    k = ui->lineEdit_4->text().toDouble();
}



void Widget::on_pushButton_5_clicked()
{
    ui->Canvas->clearContours();
    repaint();

}


void Widget::on_pushButton_7_clicked()
{
    QString shape = ui->comboBox->currentText();

    QSize size = ui->Canvas->size();

    Algorithms a;
    std::vector<QPoint3D> points = a.generateRandomPoints(size, n_points);
    if (shape == "Saddle")
        points = a.generateSaddle(points);
    else if (shape == "Pile")
        points = a.generatePile(points);
    else if (shape == "Ridge")
        points = a.generateRidge(points);
    else if (shape == "Rest")
        points = a.generateRest(points);

    points.pop_back();

    int max = a.findMaxZ(points);
    int min = a.findMinZ(points);



    ui->Canvas->setPoints(points);
//    ui->Canvas->setz_max(max);
//    ui->Canvas->setz_min(min);

    ui->lineEdit->setText(QString::number(min));
    ui->lineEdit_2->setText(QString::number(max));
    repaint();
}



void Widget::on_lineEdit_5_editingFinished()
{
    n_points = ui->lineEdit_5->text().toDouble();
}


//void Widget::on_pushButton_6_clicked()
//{
//    QString om = ui->lineEdit_6->text();
//    ui->Canvas->setRotate(om.toDouble());
//    repaint();
//}

