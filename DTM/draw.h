#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3d.h"
#include <QtGui>
#include "edge.h"


class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    std::vector<QPoint3D> getPoints(){return points;}
    void setDT(std::vector<Edge> &dt_){dt = dt_;}
    void clearDT();



signals:

public slots:
};

#endif // DRAW_H
