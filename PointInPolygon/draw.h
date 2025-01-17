#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <iostream>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPolygon> polygons;
    std::vector<QPoint> vertices;
    std::vector<int> polygonResults;
    QPoint q;
    bool add_vertex;
    bool painting;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    void changeStatus(){add_vertex = !add_vertex;}
    QPoint getPoint(){return q;}
    std::vector<QPoint> getVertices(){return vertices;}
    std::vector<QPolygon> getPolygons(){return polygons;}
    void pushPolygon(QPolygon &pol){polygons.push_back(pol);}
    void drawVertices();
    void drawPolygons(std::vector<QPolygon> &pols);
    void addResults(std::vector<int> &results);
    void paintPolygon();
signals:

public slots:
};

#endif // DRAW_H
