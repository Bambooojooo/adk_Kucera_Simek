#include "algorithms.h"
#include <map>
#include "edge.h"

TPointLinePosition Algorithms:: getPointLinePosition(QPointFBO &a,QPointFBO &p1,QPointFBO &p2)
{
    //Analyze point and line position
    double eps = 1.0e-10;

    //Coordinate differences
    double ux=p2.x()-p1.x();
    double uy=p2.y()-p1.y();

    double vx=a.x()-p1.x();
    double vy=a.y()-p1.y();

    //Half plane test(cross product)
    double t = ux*vy-vx*uy;

    //Line length
    double s = sqrt(ux*ux + uy*uy);

    //Normalized half plane test
    double tn = t/s;

    //Point in the left halfplane
    if (tn > eps)
        return LeftHP;

    //Point in the right halfplane
    if (tn < -eps)
        return RightHP;

    //Point on the line
    return On;
}


double Algorithms::get2LinesAngle(QPointFBO &p1, QPointFBO &p2, QPointFBO &p3, QPointFBO &p4)
{
    //Compute angle formed by two lines

    //Coordinate differences
    double ux=p2.x()-p1.x();
    double uy=p2.y()-p1.y();

    double vx=p4.x()-p3.x();
    double vy=p4.y()-p3.y();

    //Dot product
    double dp=ux*vx+uy*vy;

    //Norms
    double nu = sqrt(ux*ux + uy*uy);
    double nv = sqrt(vx*vx + vy*vy);

    //Angle
    return fabs(acos(dp/(nu*nv)));
}


TPointPolygonPosition Algorithms::getPositionWinding(QPointFBO &q, TPolygon &pol)
{
    //Analyze position of point and polygon
    int n = pol.size();
    double omega_sum=0;

    //Process all segments of polygon
    for (int i = 0; i<n; i++)
    {
        // Angle between two line segments
        double omega = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

        // Point and line segment position
        TPointLinePosition pos = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point on the line
    if (pos == On)
    {
        //Coors if bbox
        double x_min = 0;
        double x_max = 0;
        double y_min = 0;
        double y_max = 0;

        //Difference tolerance
        double eps = 8;

        //Make bbox
        if (pol[i].x() <= pol[(i+1)%n].x())
        {
            x_min = pol[i].x();
            x_max = pol[(i+1)%n].x();
        }
        else
        {
            x_min = pol[(i+1)%n].x();
            x_max = pol[i].x();
        }

        if (pol[i].y() <= pol[(i+1)%n].y())
        {
            y_min = pol[i].y();
            y_max = pol[(i+1)%n].y();
        }
        else
        {
            y_min = pol[(i+1)%n].y();
            y_max = pol[i].y();
        }

        //Point on the line
        if ((q.x() >= (x_min-eps)) && (q.x() <= (x_max+eps)) && (q.y() >= (y_min-eps)) && (q.y() <= (y_max+eps)))
            return Boundary;
    }

        //Point in the left halfplane
        if (pos==LeftHP)
            omega_sum += omega;
        else
            omega_sum -= omega;
    }

    //Point inside polygon
    double eps = 1.0e-5;
    if (fabs(fabs(omega_sum) - 2*M_PI) < eps)
        return Inner;

    //Point on boundary
    else if (fabs(fabs(omega_sum) - M_PI) < eps)
        return Boundary;

    //Point outside polygon
    return Outer;
}


std::tuple<QPointFBO,T2LinesPosition> Algorithms::get2LinesIntersection(QPointFBO &p1, QPointFBO &p2, QPointFBO&p3, QPointFBO &p4)
{
    //Get intersect of two lines
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    //Computing coefs k1-3
    double k1=vx*wy-vy*wx;
    double k2=ux*wy-uy*wx;
    double k3=vy*ux-vx*uy;

    //Const
    double eps = 1.0e-15;

    // !!!!!!!!For colinearity works only sometimes (test k1, k2, k3 does not always works for following conditions)!!!!!!!
    //Colinear lines
    if ((fabs(k1) < eps) && (fabs(k2) < eps) && (fabs(k3) < eps))
    {
        return {QPointFBO(), Colinear};
    }

    //Parallel lines
    if ((fabs(k1) < eps) && (fabs(k2) < eps))
    {
        return {QPointFBO(), Parallel};
    }

    //Alpha, beta coeficients
    double alpha = k1/k3;
    double beta = k2/k3;

    //Intersection exists
    if ((fabs(alpha) > eps) && (fabs(alpha) < 1-eps) && (fabs(beta) > eps) && (fabs(beta) < 1-eps))
    {
        //Compute point of intersection
        double xq = p1.x() + alpha*ux;
        double yq = p1.y() + alpha*uy;

        //Intersection and its properties
        QPointFBO b (xq, yq);
        b.setAlpha(alpha);
        b.setBeta(beta);

        return {b, Intersect};
    }

    //Lines do not intersect
    return {QPointFBO(), NonIntersect};
}


void Algorithms::updatePolygons(TPolygon &A, TPolygon &B)
{
    //Update polygon vertices using mutual intersections

    //Go over each edge of the first polygon
    for (int i = 0; i < A.size(); i++)
    {
        //Map of intersections sorted by alpha
        std::map<double, QPointFBO> M;

        //Go over each edge of the second polygon
        for (int j = 0; j < B.size(); j++)
        {
            //Compute intersection
            auto[b, status] = get2LinesIntersection(A[i], A[(i+1)%A.size()], B[j], B[(j+1)%B.size()]);

            //Intersection exists
            if (status == Intersect)
            {
                //Get alpha, beta
                double alpha = b.getAlpha();
                double beta = b.getBeta();

                //Add to map
                M[alpha] = b;

                //Add point to B
                processIntersection(b, beta, j, B);
            }
        }

        //Is there an intersection
        if (M.size()>0)
        {
            //Process all intersections
            for (auto m:M)
            {
                double alpha = m.first;
                QPointFBO b = m.second;

                //Add point to A
                processIntersection(b, alpha, i, A);
            }
        }
    }
}


void Algorithms::processIntersection(QPointFBO &b, double t, int &index, TPolygon &P)
{
    //If inner point add to polygon
    double epsilon = 1.e-5;

    //Inner point
    if ((t > epsilon) && (t < (1 - epsilon)))
    {
        //Increment index and add point to polygon
        index++;
        P.insert(P.begin()+index, b);
    }
}


void Algorithms::setEdgePositions(TPolygon &A, TPolygon &B)
{
    //Set positions of the polygon edges A to B
    int n=A.size();

    //Browse polygon vertices one by one
    for (int i = 0; i < n; i++)
    {
        //Mid point
        double xm = (A[i].x() + A[(i+1)%n].x())/2;
        double ym = (A[i].y() + A[(i+1)%n].y())/2;

        //Get position of point and polygon
        QPointFBO pm (xm, ym);
        TPointPolygonPosition pos = getPositionWinding(pm, B);

        //Update edge position
        A[i].setPosition(pos);
    }
}


void Algorithms::selectEdges(TPolygon &P, TPointPolygonPosition pos, TEdges &edges)
{
    //Selected edges according to boolean operations
    int n = P.size();

    //Go through polygon by edge
    for (int i = 0; i < n; i++)
    {
        //Same position
        if (P[i].getPosition() == pos)
        {
            //Create new Edge
            Edge e(P[i], P[(i+1)%n]);
            edges.push_back(e);
        }
    }
}


TEdges Algorithms:: createOverlay(TPolygon &A, TPolygon &B, TBooleanOperation &op)
{
    //Create overlay from polygons
    TEdges result;

    //Compute intersections and update polygons
    updatePolygons(A, B);

    //Find out positions of (A to B) & (B to A)
    setEdgePositions(A, B);
    setEdgePositions(B, A);

    //Union
    if (op == Union)
    {
        selectEdges(A, Outer, result);
        selectEdges(B, Outer, result);
        selectEdges(A, Boundary, result);
        selectEdges(B, Boundary, result);
    }

    //Intersection
    else if(op == Intersection)
    {
        selectEdges(A, Inner, result);
        selectEdges(B, Inner, result);
//        selectEdges(A, Boundary, result);
        selectEdges(B, Boundary, result);
    }

    //DifferenceA_B
    else if(op == DifferenceA_B)
    {
        selectEdges(A, Outer, result);
        selectEdges(B, Inner, result);
        selectEdges(A, Boundary, result);
    }

    //DifferenceB_A
    else if (op == DifferenceB_A)
    {
        selectEdges(A, Inner, result);
        selectEdges(B, Outer, result);
        selectEdges(B, Boundary, result);
    }

    return result;
}

std::vector<TPolygon> Algorithms::transformPolygons(std::vector<TPolygon> &polygons, double &trans_x, double &trans_y, double &scale, int &offset_x, int &offset_y)
{
    //Transform polygon coorinates by basic transformation based on minmax box of dataset
    //x_min, x_max, y_min, y_max represent boundaries of dataset minmax box
    std::vector<TPolygon> polygons_transformed;

    //Process all polygons
    for (TPolygon polygon: polygons)
    {
        //Declare polygon wich will be transformed
        TPolygon polygon_transformed;

        for (QPointFBO p: polygon)
        {
            //Translation with slight offset due to canvas origin set on [11,11] coors
            double dx = p.x()-trans_x-offset_x;
            double dy = p.y()-trans_y-offset_y;

            //Data scaling
            double ddx = dx*(scale/1.1);
            double ddy = dy*(scale/1.1);

            //Translate data back to visible part of Canvas
            double x0 = ddx + offset_x;
            double y0 = ddy + offset_y;

            polygon_transformed.push_back(QPointFBO(x0, y0));
        }
        polygons_transformed.push_back(polygon_transformed);
    }

    //Compute transformation key
    return polygons_transformed;
}

bool Algorithms::BBoxIntersection(TPolygon &A, TPolygon &B)
{
    //Return true if there is a mmb intersection
    double xmin_a = -10.e10, xmax_a = 10.e10, ymin_a = -10.e10, ymax_a = 10.e10;
    double xmin_b = -10.e10, xmax_b = 10.e10, ymin_b = -10.e10, ymax_b = 10.e10;

    //Get mmb of A
    for (QPointFBO p: A)
    {
        //Update minmax box coors
        if (p.x() < xmin_a)
            xmin_a = p.x();

        if (p.x() > xmax_a)
            xmax_a = p.x();

        if (p.y() < ymin_a)
            ymin_a = p.y();

        if (p.y() > ymax_a)
            ymax_a = p.y();
    }

    //Get mmb of A
    for (QPointFBO p: B)
    {
        //Update minmax box coors
        if (p.x() < xmin_b)
            xmin_b = p.x();

        if (p.x() > xmax_b)
            xmax_b = p.x();

        if (p.y() < ymin_b)
            ymin_b = p.y();

        if (p.y() > ymax_b)
            ymax_b = p.y();
    }

    return ((xmax_a >= xmin_b) && (xmax_b >= xmin_a) && (ymax_a >= ymin_b) && (ymax_b >= ymin_a));
}
