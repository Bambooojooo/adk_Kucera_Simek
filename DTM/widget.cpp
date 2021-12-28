#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "edge.h"
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
    zmin = 0;
    zmax = 1000.0;
    dz = 1;
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
    //Clear all points
    ui->Canvas->clearPoints();
    repaint();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT
    if (points.size() > 0)
    {
        std::vector<Edge> dt = Algorithms::dT(points);

        //Set DT
        ui->Canvas->setDT(dt);

        repaint();
    }
        //No points
    else
    {
        QMessageBox msg;
        msg.setText("No points to process. Click them or load them first!");
        msg.setStandardButtons(QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret = msg.exec();
    }
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
    dz = ui->lineEdit_3->text().toInt();
}

void Widget::on_pushButton_3_clicked()
{
     //Create contours
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Triangle> triangles = ui->Canvas->getTriangles();

    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        double z_min = ui->lineEdit->text().toDouble();
        double z_max = ui->lineEdit_2->text().toDouble();
        int dz = ui->lineEdit_3->text().toInt();
        int contour_interval = ui->lineEdit_4->text().toInt();
        double label_distance_threshold = 250;
        double edge_length_threshold = 6;
        double label_offset = rand()%50;

        //Create contours
        std::vector<Edge> contours = Algorithms::getContourLines(dt, z_min, z_max, dz);

        //Get main labeled contour lines
        std::vector<Edge> contours_main;
        std::vector<Edge> contours_labeled = Algorithms::getLabeledContours(contours, contours_main, contour_interval, dz, label_distance_threshold, edge_length_threshold, label_offset);

        ui->Canvas->setContoursMain(contours_main);
        ui->Canvas->setContoursLabeled(contours_labeled);

        std::cout << "zmin " << zmin << " zmax " << zmax << std::endl;

        //Set contours
        ui->Canvas->setContours(contours);
        ui->Canvas->setdZ(dz);
        ui->Canvas->setK(k);
        ui->Canvas->setz_min(zmin);

        repaint();
    }
    else
    {
        QMessageBox msg;
        msg.setText("No Delaunay triangulation! Create DT first!");
        msg.setStandardButtons(QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret = msg.exec();
    }
}

void Widget::on_pushButton_4_clicked()
{
    //Draw slope
    std::vector<Edge> dt = ui->Canvas->getDT();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        //Analyze DTM
        std::vector<Triangle> triangles = Algorithms::analyzeDTM(dt);

        //Set triangles
        ui->Canvas->setTriangles(triangles);
        QString col = ui->comboBox_2->currentText();
        QString method = ui->comboBox_3->currentText();
        bool contoursUp = ui->checkBox->isChecked();
        ui->Canvas->setAnalyzeMethod(method);
        ui->Canvas->setColor(col);
        ui->Canvas->setContourUp(contoursUp);
        double max = Algorithms::getMaxSlope(triangles);
        double min = Algorithms::getMinSlope(triangles);
        ui->Canvas->setMinSlope(min);
        ui->Canvas->setMaxSlope(max);



        repaint();
    }
}

void Widget::on_lineEdit_4_editingFinished()
{
    k = ui->lineEdit_4->text().toInt();
}



void Widget::on_pushButton_5_clicked()
{
    ui->Canvas->clearContours();
    ui->Canvas->clearTriangles();
    repaint();
}

void Widget::on_pushButton_7_clicked()
{
    QString shape = ui->comboBox->currentText();

    QSize size = ui->Canvas->size();

    std::vector<QPoint3D> points = Algorithms::generateRandomPoints(size, n_points);
    if (shape == "Saddle")
        points = Algorithms::generateSaddle(points);
    else if (shape == "Pile")
        points = Algorithms::generatePile(points);
    else if (shape == "Ridge")
        points = Algorithms::generateRidge(points);
//    else if (shape == "Rest")
//        points = a.generateRest(points);

    ui->Canvas->setPoints(points);
    points.pop_back();

    int max = Algorithms::findMaxZ(points);
    int min = Algorithms::findMinZ(points);


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


void Widget::on_pushButton_Load_clicked()
{
    //Minmax box coors
    double x_min =  10.e10;
    double x_max = -10.e10;
    double y_min =  10.e10;
    double y_max = -10.e10;

    //Open dialog to choose data file and store its path to QString
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../Data/DMT", tr("CSV Files (*.csv)")));

    //Convert QString path to string path
    std::string filename = path.toStdString();

    if (filename.length() > 0)
    {
        //Read the file with chosen path
        std::vector<std::vector<std::string>> csv_content = CSV::read_csv(filename);

        std::vector<QPoint3D> points_3d = CSV::getPoints3D(csv_content, x_min, x_max, y_min, y_max);

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
        ui->Canvas->drawCSVPoints(points_3d);
    }
}

void Widget::on_pushButton_6_clicked()
{
    //Draw Exposition
    std::vector<Edge> dt = ui->Canvas->getDT();

    //Is the triangulation not empty?
    if (dt.size() > 0)
    {
        //Analyze DTM
        std::vector<Triangle> triangles = Algorithms::analyzeDTM(dt);

        //Set triangles
        ui->Canvas->setTriangles(triangles);
        QString col = ui->comboBox_2->currentText();
        ui->Canvas->setColor(col);

        repaint();
    }
}

void Widget::on_checkBox_clicked()
{
    bool contoursUp = ui->checkBox->isChecked();

    ui->Canvas->setContourUp(contoursUp);

    repaint();
}

void Widget::on_save_canvas_clicked()
{
    QString path(QFileDialog::getSaveFileName(this, tr("Save Canvas"), tr("Canvas"), tr("PNG Files (*.png)")));
    ui->Canvas->grab().save(path);
}

void Widget::on_dmtUP_clicked()
{
    bool dmtUp = ui->dmtUP->isChecked();

    ui->Canvas->setDmtUp(dmtUp);

    repaint();
}


void Widget::on_checkBox_2_stateChanged(int arg1)
{

}


void Widget::on_checkBox_2_clicked()
{

}


void Widget::on_save_clicked()
{

}

