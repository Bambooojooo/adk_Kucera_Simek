#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "csv.h"

#include <iostream>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>

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


void Widget::on_pushButton_clicked()
{
    ui->Canvas->switchSource();
}


void Widget::on_pushButton_2_clicked()
{
    //Get polygons and Boolean operation
    TPolygon A = ui->Canvas->getA();
    TPolygon B = ui->Canvas->getB();
    TBooleanOperation op = (TBooleanOperation)ui->comboBox->currentIndex();

    //Create overlay of polygons
    Algorithms a;
    TEdges res = a.createOverlay(A, B, op);

    //Set result
    ui->Canvas->setEdges(res);

    repaint();
}


void Widget::on_pushButton_3_clicked()
{
    //Clear results
    ui->Canvas->clear();
    repaint();
}


void Widget::on_pushButton_4_clicked()
{
    //Clear all
    ui->Canvas->clearAll();
    repaint();
}

void Widget::on_pushButton_LoadA_clicked()
{
    //Minmax box coors
    double x_min =  10.e10;
    double x_max = -10.e10;
    double y_min =  10.e10;
    double y_max = -10.e10;

    //Open dialog to choose data file and store its path to QString
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with points"), "../Data/BooleanOperations", tr("CSV Files (*.csv)")));

    //Convert QString path to string path
    std::string filename = path.toStdString();

    if (filename.length() > 0)
    {
        //Read the file with chosen path
        std::vector<std::vector<std::string>> csv_content = CSV::read_csv(filename);

        std::vector<std::pair<std::string, QPointFBO>> points = CSV::getPoints(csv_content, x_min, x_max, y_min, y_max);

        //Get canvas size
        int canvas_width = ui->Canvas->size().width();
        int canvas_height = ui->Canvas->size().height();

        //Get size ratio for transformation to canvas
        double dataset_width = x_max - x_min;
        double dataset_height = y_max - y_min;

        //Get size ratio for transformation to canvas
        double x_ratio = canvas_width/dataset_width;
        double y_ratio = canvas_height/dataset_height;

        //Get proper scale for whole dataset visibility without deformation
        double scale;
        if (x_ratio < y_ratio)
            scale = x_ratio;
        else
            scale = y_ratio;
        ui->Canvas->setScale(scale);

        //Get canvas left top corner coors
        int x_left_top = ui->Canvas->geometry().x();
        int y_left_top = ui->Canvas->geometry().y();

        //Get left top corner (origin offset)
        int offset_x = ui->Canvas->x();
        int offset_y = ui->Canvas->y();

        //Set offsets
        ui->Canvas->setOffsets(offset_x, offset_y);

        //Get translation parameter for transformation
        double x_trans = x_min - x_left_top;
        double y_trans = y_min - y_left_top;

        //Set translations
        ui->Canvas->setTrans(x_trans, y_trans);

        //Draw points
        ui->Canvas->drawCSVPoints(points);
    }
}
