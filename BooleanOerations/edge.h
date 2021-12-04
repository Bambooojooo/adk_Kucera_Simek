#ifndef EDGE_H
#define EDGE_H

#include "qpointfbo.h"

class Edge
{
private:
    QPointFBO start, end;
public:
    Edge():start(),end(){}
    Edge(QPointFBO &start_, QPointFBO &end_):start(start_), end(end_){}

    void setStart(QPointFBO &start_){start=start_;}
    void setEnd(QPointFBO &end_){end=end_;}
    QPointFBO getStart(){return start;}
    QPointFBO getEnd(){return end;}

};

#endif // EDGE_H
