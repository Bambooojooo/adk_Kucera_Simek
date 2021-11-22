#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT
private:
    bool add_polygons;
    QPolygon ch, er;
    std::vector<QPoint> points;
    std::vector<QPolygon> polygons, chs, ers;



public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clearDrawing();
    void clearData();
    std::vector<QPoint> getPoints(){return points;}
    std::vector<QPolygon> getPolygons(){return polygons;}
    void addCh(QPolygon &ch_){chs.push_back(ch_);}
    void addEr(QPolygon &er_){ers.push_back(er_);}
    void clearChs(){chs.clear();}
    void clearErs(){ers.clear();}
    void drawPolygons(std::vector<QPolygon> &pols);
    void drawPolygons(std::vector<QPolygon> &pols, double &x_trans, double &y_trans, double &x_ratio, double &y_ratio);
    QPolygon transformPolygon(QPolygon &pol, double &x_trans, double &y_trans, double &x_ratio, double &y_ratio);


signals:

public slots:
};

#endif // DRAW_H
