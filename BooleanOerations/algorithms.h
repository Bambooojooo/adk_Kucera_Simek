#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "qpointfbo.h"
#include "types.h"


class Algorithms
{
public:
    Algorithms(){};

    static TPointLinePosition getPointLinePosition(QPointFBO &a,QPointFBO &p1,QPointFBO &p2);
    static double get2LinesAngle(QPointFBO &p1, QPointFBO &p2, QPointFBO &p3, QPointFBO &p4);
    static TPointPolygonPosition getPositionWinding(QPointFBO &q, TPolygon &pol);
    static std::tuple<QPointFBO,T2LinesPosition> get2LinesIntersection(QPointFBO &p1, QPointFBO &p2, QPointFBO&p3, QPointFBO &p4);
    static void updatePolygons(TPolygon &A, TPolygon &B);
    static void processIntersection(QPointFBO &b, double t, int &index, TPolygon &P);
    static void setEdgePositions(TPolygon &A, TPolygon &B);
    static void selectEdges(TPolygon &P, TPointPolygonPosition pos, TEdges &edges);
    static TEdges createOverlay(TPolygon &A, TPolygon &B, TBooleanOperation &op);
    static std::vector<TPolygon> transformPolygons(std::vector<TPolygon> &polygons, double &trans_x, double &trans_y, double &scale, int &offset_x, int &offset_y);
    static bool BBoxIntersection(TPolygon &A, TPolygon &B);
};

#endif // ALGORITHMS_H
