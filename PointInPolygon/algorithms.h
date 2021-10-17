#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
#include <vector>


class Algorithms
{
public:
    Algorithms();
    int getPointLinePosition(QPoint &a,QPoint &p1,QPoint &p2);
    double get2LinesAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4);
    int getPositionWinding(QPoint &q, std::vector<QPoint> &pol);
    int getPositionRay(QPoint &q, std::vector<QPoint> &pol);
    std::vector<QPoint> getLocalCoords(QPoint &q, std::vector<QPoint> &pol);
    bool ifCloseToPoint (QPoint &q, std::vector<QPoint> &pol);

    int processPolygons(QPoint &q, std::vector<QPolygon> &pols, QString &Alg);
};

#endif // ALGORITHMS_H
