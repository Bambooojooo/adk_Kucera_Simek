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

    //set color of highlighted polygon
    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    QPainterPath path;

    //Create new polygon
    QPolygon pol;

    //If clicking on the canvas, add_polygons is false and then single vertices from clicking are appended to pol which will be created.
    if (not add_polygons)
    {
	    for (int i = 0; i < vertices.size(); i++)
	    {
		pol.append(vertices[i]);
	    }

	    polygons.push_back(pol);
    }

    //Draw polygons
    for(int i = 0; i<polygonResults.size(); i++)
    {
	    if (polygonResults[i] != 0)
	    {
		    path.addPolygon(polygons[i]);
		    painter.fillPath(path, brush);
		    painter.drawPolygon(polygons[i]);
	    }

    }

    for(int i = 0; i<polygons.size(); i++)
    {
	painter.drawPolygon(polygons[i]);
    }


    //Draw point q
    painter.drawEllipse(q.x()-4, q.y()-4, 8, 8);

    painter.end();

    polygonResults.clear();
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
    polygons.clear();
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

void Draw::addResults(std::vector<int> &results)
{
	for (int result : results)
		polygonResults.push_back(result);
}

