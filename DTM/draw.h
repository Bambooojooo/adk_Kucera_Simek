#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3d.h"
#include <QtGui>
#include "edge.h"
#include "triangle.h"


class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;
    std::vector<Edge> contours;
    std::vector<Triangle> triangles;
    int dz;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    std::vector<QPoint3D> getPoints(){return points;}
    void setDT(std::vector<Edge> &dt_){dt = dt_;}
    std::vector<Edge> getDT(){return dt;}
    void setContours(std::vector<Edge> &contours_){contours = contours_;}
    void setdZ(int dz_){dz=dz_;}
    std::vector<Edge> getContours(){return contours;}
    std::vector<Triangle> getTriangles(){return triangles;}
    void setTriangles(std::vector<Triangle> &triangles_){triangles = triangles_;}
    void clearDT();



signals:

public slots:
};

#endif // DRAW_H
