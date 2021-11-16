#ifndef EDGE_H
#define EDGE_H

#include <QtGui>

class Edge
{
private:
    QPoint s,e;

public:
    Edge(QPoint &s_, QPoint &e_):s(s_), e(e_){}
    void changeOrientation(){QPoint temp=s; s=e; e=temp;}
    QPoint getStart() const {return s;}
    QPoint getEnd() const {return e;}

    bool operator == (const Edge &e2)
    {
        return (s==e2.getStart()) && (e == e2.getEnd());
    }

};

#endif // EDGE_H
