#include "draw.h"
#include "algorithms.h"

#include <string>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    addA = true;
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    //Draw polygons
    qp.setPen(QPen(Qt::blue, 1));
    drawPolygon(A, qp);
    qp.setPen(QPen(Qt::green, 1));
    drawPolygon(B, qp);

    //Draw edges
    qp.setPen(QPen(Qt::red, 2));
    for(Edge e:res)
        {
        qp.drawLine(e.getStart(), e.getEnd());
        qp.drawEllipse(e.getStart().x()-4,e.getStart().y()-4,2*4,2*4);
        qp.drawEllipse(e.getEnd().x()-4,e.getEnd().y()-4,2*4,2*4);
        }

    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();

    //Create point
    QPointFBO p(x,y);

    //Add point to A, B
    if (addA)
        A.push_back(p);
    else
        B.push_back(p);

    //Update screen
    repaint();
}

void Draw::drawPolygon(TPolygon &polygon, QPainter &qp)
{
    //Draw polygon on canvas
    int r=4;
    QPolygon pol;

    //Convert polygon to QPolygon
    for (int i=0; i<polygon.size(); i++)
    {
       qp.drawEllipse(polygon[i].x()-r,polygon[i].y()-r,2*r,2*r);
       pol.append(QPoint(polygon[i].x(),polygon[i].y()));
    }

    //Draw polygon
    qp.drawPolygon(pol);
}

void Draw::drawCSVPoints(std::vector<std::pair<std::string, QPointFBO>> &points)
{
    //Get transformation parameters
    double trans_x = getTransX();
    double trans_y = getTransY();
    double scale = getScale();
    int delta_x = getDeltaX();
    int delta_y = getDeltaY();

    //Draw vector of points
    std::vector<std::pair<std::string, QPointFBO>> transformedPoints = Algorithms::transformPoints(points, trans_x, trans_y, scale, delta_x, delta_y);
    Draw::setCSVPoints(transformedPoints);

    repaint();
}

void Draw::setCSVPoints(std::vector<std::pair<std::string, QPointFBO>> &csv_points)
{
    //Set A and B polygon coordinates
    for (std::pair<std::string, QPointFBO> p: csv_points)
    {
        if (p.first == "A")
        {
            A.push_back(p.second);
        }
        else if (p.first == "B")
        {
            B.push_back(p.second);
        }
    }
}
