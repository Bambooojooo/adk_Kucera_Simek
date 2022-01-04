#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "qpointfbo.h"
#include "types.h"


class Algorithms
{
public:
    Algorithms(){};

    TPointLinePosition getPointLinePosition(QPointFBO &a,QPointFBO &p1,QPointFBO &p2);
    double get2LinesAngle(QPointFBO &p1, QPointFBO &p2, QPointFBO &p3, QPointFBO &p4);
    TPointPolygonPosition getPositionWinding(QPointFBO &q, TPolygon &pol);
    std::tuple<QPointFBO,T2LinesPosition> get2LinesIntersection(QPointFBO &p1, QPointFBO &p2, QPointFBO&p3, QPointFBO &p4);
    void updatePolygons(TPolygon &A, TPolygon &B);
    void processIntersection(QPointFBO &b, double t, int &index, TPolygon &P);
    void setEdgePositions(TPolygon &A, TPolygon &B);
    void selectEdges(TPolygon &P, TPointPolygonPosition pos, TEdges &edges);
    TEdges createOverlay(TPolygon &A, TPolygon &B, TBooleanOperation &op);
};

#endif // ALGORITHMS_H
