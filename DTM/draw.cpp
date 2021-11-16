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

    for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(points[i]);
    }

    //Draw triangulation
    for(Edge e : dt)
    {
        //Get start point, get end point
        QPoint s_point = e.getStart();
        QPoint e_point = e.getEnd();

        //Draw line
        qp.drawLine(s_point,e_point);
    }

    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();

    //Create point
    QPoint p(x,y);

    //Add point to the vector
    points.push_back(p);

    //Update screen
    repaint();
}

void Draw::clear()
{
    points.clear();
    repaint();
}

void Draw::clearDT()
{
    dt.clear();
    repaint();
}

