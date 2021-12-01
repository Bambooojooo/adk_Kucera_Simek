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

    //Draw clicked points
    int r=4;
    QPolygon pol;
    Algorithms a;

    for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(QPoint(points[i].x(), points[i].y()));
    }

    //Draw csv_points
    for (QPoint3D point_3d: csv_points)
    {
	qp.drawEllipse(point_3d.x()-r,point_3d.y()-r,2*r,2*r);
    }

    //Draw triangulation
    for(Edge e : dt)
    {
        //Get start point, get end point
        QPoint3D s_point = e.getStart();
        QPoint3D e_point = e.getEnd();

        //Draw line
        qp.setPen(QPen(Qt::green, 1));
        qp.drawLine(s_point,e_point);
    }

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

	//Height interval for main contour lines
        int d=dz*k;

        QColor brown("#a52a2a");
        //Set pen
        qp.setPen(QPen(brown,1));

	//Main contour lines
        if ((zz+z_min)%d == 0)
        {
            //Draw main contour line
            qp.setPen(QPen(brown,2));
            qp.drawLine(s_point,e_point);

            //Draw description of contour line in 50% procent of events
            if (a.pointDist(s_point, e_point) > 50)
            {
                if (rand() % 100 < 50)
                {
                    //Angle of contour line
                    double s = atan((e_point.y()-s_point.y())/ (e_point.x() - s_point.x()));
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
                    qp.setPen(QPen(brown,1));

                    //Draw text
                    qp.drawText(QPoint3D(5,5), QString::number(s_point.getZ()));
                    qp.resetTransform();
                }
            }
            qp.setPen(QPen(brown,1));
        }

	//Regular contour line
        else
            qp.drawLine(s_point,e_point);
        }

    std::cout << set_col.QString::toStdString() << std::endl;

    //Draw slope
    //Koeficient for slope drawing (bits and radians relation)
    double k = 255/M_PI;
//    QColor color;
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




