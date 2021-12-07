#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>
#include <map>

#include "edge.h"
#include "qpoint3d.h"
#include "triangle.h"


class Algorithms
{
public:
    Algorithms();
    static double pointDist(QPoint3D &p1, QPoint3D &p2);
    int getPointLinePosition(QPoint3D &a,QPoint3D &p1,QPoint3D &p2);
    std::tuple<QPoint3D,double> getCircleCenterAndRadius(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3);
    int getDelaunayPoint(QPoint3D &s,QPoint3D &e,std::vector<QPoint3D> &points);
    int getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points);
    std::vector<Edge> dT(std::vector<QPoint3D> &points);
    void updateAEL(Edge &e, std::list<Edge> &ael);

    double getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    double getExposition(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    std::vector<Triangle> analyzeDTM(std::vector<Edge> &dt);
    QPoint3D getRotatedPoint(QPoint3D &p, double om);
    std::vector<QPoint3D> generatePile(std::vector<QPoint3D> &points);
    int findMaxZ(std::vector<QPoint3D> &points);
    int findMinZ(std::vector<QPoint3D> &points);
//    QPoint3D getProjectedPoint(QPoint3D &p, double om);
    std::vector<QPoint3D> generateRandomPoints(QSize &size_canvas, int n);
    std::vector<QPoint3D> generateSaddle(std::vector<QPoint3D> &points);
    std::vector<QPoint3D> generateRidge(std::vector<QPoint3D> &points);
//    std::vector<QPoint3D> generateRest(std::vector<QPoint3D> &points);
    static QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    static std::vector<Edge> getContourLines(std::vector<Edge> &dt, double zmin, double zmax, int dz);
    static std::map<double, std::vector<Edge>> getMainContourLines(std::vector<Edge> &contours, int contour_interval, double dz);
    static std::vector<QPoint3D> transformPoints(std::vector<QPoint3D> &points_3d, double &trans_x, double &trans_y, double &scale, int &delta_x, int &delta_y);
    static std::vector<Edge> getLabeledContours(std::vector<Edge> &contours, std::vector<Edge> &contours_main, int contour_interval, double dz, double &threshold);

    static std::vector<Edge> getDistancedEdges(std::vector<Edge> &edges, double &threshold);
    static double getMinSlope(std::vector<Triangle> &triangles);
    static double getMaxSlope(std::vector<Triangle> &triangles);
};

#endif // ALGORITHMS_H
