#include "draw.h"
#include <QtGui>


Draw::Draw(QWidget *parent) : QWidget(parent)
{
    q.setX(-100);
    q.setY(-100);
    add_vertex = true;
    add_polygons = false;

}

void Draw::paintEvent(QPaintEvent *event)
{
    // Create graphic object
    QPainter painter(this);
    painter.begin(this);

    //Create new polygon
    QPolygon pol;

    //Convert vector to polygon
    if (not add_polygons)
    {
	    for (int i = 0; i < vertices.size(); i++)
	    {
		pol.append(vertices[i]);
	    }

	    polygons.push_back(pol);
    }

    //Draw point q
    painter.drawEllipse(q.x()-4, q.y()-4, 8, 8);

    //Draw polygons
    for(QPolygon polg : polygons)
    {
	painter.drawPolygon(polg);
    }

    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get cursor coordinates
    int x = event->pos().x();
    int y = event->pos().y();

    //Add new vertex
    if (add_vertex)
    {
        //Create new point
	QPoint p(x,y);

        //Add point to vector
        vertices.push_back(p);
    }

    //Modify coordinates of point Q
    else
    {
        q.setX(x);
        q.setY(y);
    }

    //Repaint screen
    repaint();
}

void Draw::clear()
{
    //Clear and repaint
    vertices.clear();
    repaint();
}

void Draw::drawPolygons(std::vector<QPolygon> &pols)
{
	vertices.clear();
	polygons.clear();
	for (QPolygon pol : pols)
		polygons.push_back(pol);

	repaint();

}
