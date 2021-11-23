#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>
#include "edge.h"
#include "qpoint3d.h"


class Algorithms
{
public:
    Algorithms();
    int getPointLinePosition(QPoint3D &a,QPoint3D &p1,QPoint3D &p2);
    std::tuple<QPoint3D,double> getCircleCenterAndRadius(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3);
    int getDelaunayPoint(QPoint3D &s,QPoint3D &e,std::vector<QPoint3D> &points);
    int getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points);
    std::vector<Edge> dT(std::vector<QPoint3D> &points);
    void updateAEL(Edge &e, std::list<Edge> &ael);
    QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    std::vector<Edge> getContourLines(std::vector<Edge> &dt, double zmin, double zmax, double dz);
};

#endif // ALGORITHMS_H
