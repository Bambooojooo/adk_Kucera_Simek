#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPolygon> polygons;
    std::vector<QPoint> vertices;
    QPoint q;
    bool add_vertex;
    bool add_polygons;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    void changeStatus(){add_vertex = !add_vertex;}
    void polygonStatus(){add_polygons = !add_polygons;}
    QPoint getPoint(){return q;}
    std::vector<QPoint> getVertices(){return vertices;}
    std::vector<QPolygon> getPolygons(){return polygons;}
    void drawPolygons(std::vector<QPolygon> &pols);
signals:

public slots:
};

#endif // DRAW_H
