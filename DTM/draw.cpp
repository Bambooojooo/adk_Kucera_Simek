#include "draw.h"
#include "algorithms.h"

#include <stdlib.h>
#include <iostream>
#include <string>

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
    int r=2;
    QPolygon pol;
    Algorithms a;

    for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(QPoint(points[i].x(), points[i].y()));
    }

//    //Draw triangulation
    double max = 200;
    if (!dmtUp)
    {
        for(Edge e : dt)
        {
            //Get start point, get end point
            QPoint3D s_point = e.getStart();
            QPoint3D e_point = e.getEnd();

            //Draw line
            qp.setPen(QPen(Qt::black, 0.15));
            qp.drawLine(s_point,e_point);
        }
    }

    if (contoursUp)
    {
        //Draw slope
        for (Triangle t:triangles)
        {
            //Get vertices of each triangle
            QPoint3D p1 = t.getP1();
            QPoint3D p2 = t.getP2();
            QPoint3D p3 = t.getP3();

            //Get slope/exposition
            double val;
            int col;
            if (method == "Slope")
            {
                //Transform <0,pi/2> -> <0,255>
                val = t.getSlope() - minSlope;
                val = val*((M_PI/2)/(maxSlope-minSlope));
                col = (255/(M_PI/2)) * val;
            }
            else if (method == "Exposition")
            {
                //Transform <0,2*pi> -> <0,255>
                val = t.getExposition();

                if (val < 0)
                    val += 2*M_PI;

                //Transform <0,2*pi> -> <0,200>
                col = -(max/M_PI)*fabs(val - M_PI) + max;
                //Invert color
                col = 255 - col;


            }



            QColor color;
            if (set_col == "Red")
                color.setRgb(col, 0, 0);
            else if (set_col == "Green")
                color.setRgb(0, col, 0);
            else if (set_col == "Blue")
                color.setRgb(0, 0, col);
            else if (set_col == "Grey")
                color.setRgb(col, col, col);

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
    }

    //Draw contour lines
    for (Edge c:contours)
    {
        //Set color and pen
        QColor brown("#a52a2a");
        qp.setPen(QPen(brown,1));

        //Get start point, get end point
        QPoint3D s_point = c.getStart();
        QPoint3D e_point = c.getEnd();

        //Height of contour line
        int zz=s_point.getZ();

        //Height interval for main contour lines
        int d=dz*k;

        //Regular contour line
        qp.drawLine(s_point,e_point);
    }


    //Draw main contour lines
    for (Edge c:contours_main)
        {
            //Defining QColor and pen
            QColor brown("#a52a2a");
            qp.setPen(QPen(brown,2));

            //Get start point, get end point
            QPoint3D s_point = c.getStart();
            QPoint3D e_point = c.getEnd();

            //Draw main contour line
            qp.drawLine(s_point,e_point);
        }

    //Draw contour line labes
    for (Edge c:contours_labeled)
    {
        //Get start point, get end point
        QPoint3D s_point = c.getStart();
        QPoint3D e_point = c.getEnd();

        //Index of delaunay points (nearest points to the edge)
        int i_right = Algorithms::getDelaunayPoint(s_point, e_point, points);
        int i_left = Algorithms::getDelaunayPoint(e_point, s_point, points);

        //Get points
        QPoint3D p_right = points[i_right];
        QPoint3D p_left = points[i_left];

        //Get height difference between contour line and nearest points on both sides
        double dz_right = p_right.getZ() - s_point.getZ();
        double dz_left = p_left.getZ() - s_point.getZ();

        //Angle of contour line
        double s = atan2((e_point.y()-s_point.y()), (e_point.x() - s_point.x()));

        //Orientate labels towards uphill
        if (dz_right > 0 || dz_left < 0)
            s+=M_PI;

        //Center of contour line
        QPoint3D z((s_point.x()+e_point.x())/2, (s_point.y()+e_point.y())/2);

        //Transform canvas to origin of axes
        QTransform t;
        t.translate(z.x(), z.y());
        t.rotate(s*180/M_PI);
        qp.setTransform(t);

        //Draw hidding line of contour line
        qp.setPen(QPen(Qt::white,5));
        qp.drawLine(QPoint(5,0),QPoint(25,0));
        QColor brown("#a52a2a");
        qp.setPen(QPen(brown,1));

        //Draw text
        qp.drawText(QPoint3D(5,5), QString::number(s_point.getZ()));
        qp.resetTransform();
    }

    if (!contoursUp)
    {
        //Draw slope
        for (Triangle t:triangles)
        {
            //Get vertices of each triangle
            QPoint3D p1 = t.getP1();
            QPoint3D p2 = t.getP2();
            QPoint3D p3 = t.getP3();

            //Get slope/exposition
            double val ;
            int col;
            if (method == "Slope")
            {

                val = t.getSlope() - minSlope;
                val = val*((M_PI/2)/(maxSlope-minSlope));
                //Transform <0,pi/2> -> <0,255>
                col =  (255/(M_PI/2)) * val;
            }
            else if (method == "Exposition")
            {

                val = t.getExposition();

                if (val < 0)
                    val += 2*M_PI;

                //Transform <0,2*pi> -> <0,200>
                col = -(max/M_PI)*fabs(val - M_PI) + max;
                //Invert color
                col = 255 - col;


            }



            QColor color;
            if (set_col == "Red")
                color.setRgb(col, 0, 0);
            else if (set_col == "Green")
                color.setRgb(0, col, 0);
            else if (set_col == "Blue")
                color.setRgb(0, 0, col);
            else if (set_col == "Grey")
                color.setRgb(col, col, col);

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
    }

    if (dmtUp)
    {
        for(Edge e : dt)
        {
            //Get start point, get end point
            QPoint3D s_point = e.getStart();
            QPoint3D e_point = e.getEnd();

            //Draw line
            qp.setPen(QPen(Qt::black, 0.2));
            qp.drawLine(s_point,e_point);
        }
    }

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

void Draw::generateShapes()
{
//    std::cout <<
}

void Draw::drawCSVPoints(std::vector<QPoint3D> &points_3d)
{
    //Get transformation parameters
    double trans_x = getTransX();
    double trans_y = getTransY();
    double scale = getScale();
    int delta_x = getDeltaX();
    int delta_y = getDeltaY();

    //Draw vector of points
    std::vector<QPoint3D> transformedPoints = Algorithms::transformPoints(points_3d, trans_x, trans_y, scale, delta_x, delta_y);
    Draw::setCSVPoints(transformedPoints);

    repaint();
}




