#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    //Draw points
    int r=4;
    QPolygon pol;

    for (int i=0; i<A.size(); i++)
    {
       //qp.drawEllipse(A[i].x()-r,A[i].y()-r,2*r,2*r);
       //pol.append(QPoint(A[i].x(),A[i].y()));
    }

    //Draw polygon
    qp.setBrush(Qt::yellow);
    qp.drawPolygon(pol);

    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();

    //Create point
    QPointFBO p(x,y);

    //Add point to the vector
    A.push_back(p);

    //Update screen
    repaint();
}

