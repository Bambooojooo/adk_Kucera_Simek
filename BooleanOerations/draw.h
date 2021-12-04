#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "types.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    TPolygon A, B;
    std::vector<Edge> res;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);


signals:

public slots:
};

#endif // DRAW_H
