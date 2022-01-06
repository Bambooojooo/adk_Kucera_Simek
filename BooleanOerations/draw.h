#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "types.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    TPolygon A, B;
    TEdges res;
    bool addA;

    double scale;
    double trans_x, trans_y;
    int offset_x, offset_y;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void switchSource(){addA = !addA;}
    void drawPolygon(TPolygon &pol, QPainter &qp);
    TPolygon getA(){return A;}
    TPolygon getB(){return B;}
    void setEdges(TEdges &edg){res = edg;}
    void clear(){res.clear();}
    void clearAll(){A.clear(); B.clear(); res.clear();}

    double getScale(){return scale;}
    double getTransX(){return trans_x;}
    double getTransY(){return trans_y;}
    int getDeltaX(){return offset_x;}
    int getDeltaY(){return offset_y;}
    void setScale(double &scale_){scale = scale_;}
    void setTrans(double &trans_x_, double &trans_y_){trans_x = trans_x_; trans_y = trans_y_;}
    void setOffsets(int &offset_x_, int &offset_y_){offset_x = offset_x_; offset_y = offset_y_;}
    void drawCSVPoints(std::vector<std::pair<std::string, QPointFBO>> &points_3d);
    void setCSVPoints(std::vector<std::pair<std::string, QPointFBO>> &csv_points);
signals:

public slots:
};

#endif // DRAW_H
