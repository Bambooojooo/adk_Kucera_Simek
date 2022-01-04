#ifndef QPOINTFBO_H
#define QPOINTFBO_H

#include <QtGui>
#include "types.h"

class QPointFBO : public QPointF
{
private:
    double alpha, beta;
    TPointPolygonPosition pos;

public:
    QPointFBO():alpha(0.0), beta(0.0), pos(Inner), QPointF(0, 0){}
    QPointFBO(double alpha_, double beta_, TPointPolygonPosition pos_, double x, double y):alpha(alpha_), beta(beta_), pos(pos_), QPointF(x, y){}
    QPointFBO(double x, double y):alpha(0.0), beta(0.0), pos(Inner), QPointF(x,y){}
    double getAlpha(){return alpha;}
    double getBeta(){return beta;}
    TPointPolygonPosition getPosition(){return pos;}


    void setAlpha(double alpha_){alpha=alpha_;}
    void setBeta(double beta_){beta=beta_;}
    void setPosition(TPointPolygonPosition pos_){pos=pos_;}
};

#endif // QPOINTFBO_H
