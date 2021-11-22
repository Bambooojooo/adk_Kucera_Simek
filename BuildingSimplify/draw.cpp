#include "draw.h"
#include "ui_widget.h"
#include "widget.h"

#include <iostream>
#include <QFileDialog>
#include <QtGui>

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

	//Draw loaded polygons
	for (QPolygon polygon : Draw::polygons)
	{
		qp.setBrush(Qt::cyan);
		qp.drawPolygon(polygon);
	}

	//Draw points
	qp.setBrush(Qt::NoBrush);
	for (int i=0; i<points.size(); i++)
	{
		qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
		pol.append(points[i]);
	}

	//Draw polygon
	qp.setBrush(Qt::yellow);
	qp.drawPolygon(pol);

	//Draw convex hull
	qp.setBrush(Qt::NoBrush);
	qp.setPen(Qt::red);
	if (chs.size() > 0)
		for (QPolygon chull : chs)
			qp.drawPolygon(chull);

	//Draw enclosing rectangle
	qp.setBrush(Qt::NoBrush);
	qp.setPen(Qt::green);
	if (ers.size() > 0)
		for (QPolygon enrect : ers)
			qp.drawPolygon(enrect);

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

void Draw::clearDrawing()
{
    points.clear();
    chs.clear();
    ers.clear();

    repaint();
}

void Draw::clearData()
{
    polygons.clear();
    chs.clear();
    ers.clear();

    repaint();
}
void Draw::drawPolygons(std::vector<QPolygon> &pols)
{
	//Draw vector of polygons by simply pushing back to a polygons vector (private variable)
	Draw::polygons.clear();
	for (QPolygon pol : pols)
		Draw::polygons.push_back(pol);

	repaint();
}

void Draw::drawPolygons(std::vector<QPolygon> &pols, double &x_trans, double &y_trans, double &x_ratio, double &y_ratio)
{
	//Draw vector of polygons by simply pushing back to a polygons vector (private variable)
	QPolygon transformedPolygon;

	Draw::polygons.clear();
	for (QPolygon pol : pols)
	{
		transformedPolygon = transformPolygon(pol, x_trans, y_trans, x_ratio, y_ratio);
		Draw::polygons.push_back(transformedPolygon);
	}

	repaint();
}

QPolygon Draw::transformPolygon(QPolygon &pol, double &x_trans, double &y_trans, double &x_ratio, double &y_ratio)
{
	//Transform polygon coorinates by basic transformation based on minmax box of dataset
	//x_min, x_max, y_min, y_max represent boundaries of dataset minmax box
	QPolygon polygonTransformed;

	for (QPoint p : pol)
	{
		//Translation with slight offset
		double dx = p.x()-x_trans;
		double dy = p.y()-y_trans;

		//Scale
		double x0 = dx/x_ratio;
		double y0 = dy/y_ratio;

		QPoint point(x0, y0);
		polygonTransformed << point;
	}

	//Compute transformation key
	return polygonTransformed;
}
