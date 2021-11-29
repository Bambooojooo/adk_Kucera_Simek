#include "draw.h"
#include <stdlib.h>
#include "algorithms.h"
#include <iostream>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Initialize random number generator
    srand (time(NULL));
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
        pol.append(QPoint(points[i].x(), points[i].y()));

    }

    //Draw triangulation
    for(Edge e : dt)
    {
        //Get start point, get end point
        QPoint3D s_point = e.getStart();
        QPoint3D e_point = e.getEnd();

        //Draw line
        qp.drawLine(s_point,e_point);
    }

    Algorithms a;
    //Draw contour lines
    for (Edge c:contours)
    {
        //Get start point, get end point
        QPoint3D s_point = c.getStart();
        QPoint3D e_point = c.getEnd();

        //Center of contour line
        QPoint3D z((s_point.x()+e_point.x())/2, (s_point.y()+e_point.y())/2);

        //Height of contour line
        int zz=s_point.getZ();
        //
        int d=dz*k;

        //Set pen
        qp.setPen(QPen(Qt::black,1));
        if (zz%d == 0)
        {
            //Draw main contour line
            qp.setPen(QPen(Qt::black,2));
            qp.drawLine(s_point,e_point);
            //Draw description of contour line in 50% procent of events
            if (rand() % 100 < 50)
            {
                //Angle of contour line
                double s = atan2(e_point.y()-s_point.y(), e_point.x() - s_point.x());
                if  (s < 0)
                    s+=2*M_PI;

                //Transform canvas to origin of axes
                QTransform t;
                t.translate(z.x(), z.y());
                t.rotate(s*180/M_PI);
                qp.setTransform(t);
                //Draw hidding line of contour line
                qp.setPen(QPen(Qt::white,5));
                qp.drawLine(QPoint(5,0),QPoint(25,0));
                qp.setPen(QPen(Qt::black,1));
                //Draw text
                qp.drawText(QPoint3D(5,5), QString::number(s_point.getZ()));
                qp.resetTransform();
            }
            qp.setPen(QPen(Qt::black,1));

        }
        else
            qp.drawLine(s_point,e_point);

    }


    //Draw slope
    double k = 255/M_PI;

    for (Triangle t:triangles)
    {
        //Get vertices of each triangle
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope
        double slope = t.getSlope();

        //Convert to color
        int col = 255 - k * slope;
        QColor color(col, col, col);

        //Set pen and brush
        qp.setBrush(color);
        qp.setPen(color);

        //Create polygon for triangle
        QPolygon pol;
        pol.push_back(QPoint(p1.x(), p1.y()));
        pol.push_back(QPoint(p2.x(), p2.y()));
        pol.push_back(QPoint(p3.x(), p3.y()));

        //Draw triangle
        qp.drawPolygon(pol);
    }

    //Stop drawing
    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();
    int z = rand() % 1000;

    //Create point
    QPoint3D p(x, y, z);

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

