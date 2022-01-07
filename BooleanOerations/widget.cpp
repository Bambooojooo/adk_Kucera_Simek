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
    std::vector<TPolygon> polygons = ui->Canvas->getPolygons();
    TBooleanOperation op = (TBooleanOperation)ui->comboBox->currentIndex();

    //Create overlay of drawn polygons A and B
    TEdges res = Algorithms::createOverlay(A, B, op);

    //Create overlay of all loaded polygons using mmb operation optimization
    if (polygons.size() > 0)
    {
        //Go through each polygon
        for (int i=0; i<polygons.size()-1; i++)
        {
            //For each polygon pair
            for (int j=i+1; j<polygons.size(); j++)
            {
                //If there is polygon mmb intersection
                if (Algorithms::MMBoxIntersection(polygons[i], polygons[j]))
                {
                    TEdges polygon_res = Algorithms::createOverlay(polygons[i], polygons[j], op);
                    res.insert(res.end(), polygon_res.begin(), polygon_res.end());
                }
            }
        }
    }

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

        std::vector<TPolygon> polygons = CSV::getCSVPolygons(csv_content, x_min, x_max, y_min, y_max);

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

        //Set scale
        if (ui->Canvas->polygonsSize() == 0)
            ui->Canvas->setScale(scale);

        //Get canvas left top corner coors
        int x_left_top = ui->Canvas->geometry().x();
        int y_left_top = ui->Canvas->geometry().y();

        //Get left top corner (origin offset)
        int offset_x = ui->Canvas->x();
        int offset_y = ui->Canvas->y();

        //Set offsets
        if (ui->Canvas->polygonsSize() == 0)
            ui->Canvas->setOffsets(offset_x, offset_y);

        //Get translation parameter for transformation
        double x_trans = x_min - x_left_top;
        double y_trans = y_min - y_left_top;

        //Set translations
        if (ui->Canvas->polygonsSize() == 0)
            ui->Canvas->setTrans(x_trans, y_trans);

        //Draw points
        ui->Canvas->drawCSVPolygons(polygons);
    }
}
