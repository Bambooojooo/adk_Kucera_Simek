#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>
#include "edge.h"


class Algorithms
{
public:
    Algorithms();
    int getPointLinePosition(QPoint &a,QPoint &p1,QPoint &p2);
    std::tuple<QPoint,double> getCircleCenterAndRadius(QPoint &p1,QPoint &p2,QPoint &p3);
    int getDelaunayPoint(QPoint &s,QPoint &e,std::vector<QPoint> &points);
    int getNearestPoint(QPoint &p, std::vector<QPoint> &points);
    std::vector<Edge> dT(std::vector<QPoint> &points);
    void updateAEL(Edge &e, std::list<Edge> &ael);
};

#endif // ALGORITHMS_H
