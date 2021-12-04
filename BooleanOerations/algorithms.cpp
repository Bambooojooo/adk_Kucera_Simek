#include "algorithms.h"


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

    //Point in the left halfplane
    if (t > eps)
        return LeftHP;

    //Point in the right halfplane
    if (t < -eps)
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
    double uy = p2.y() - p1.x();

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

        return {QPointFBO(xq, yq), Intersect};
    }

    //Lines do not intersect
    return {QPointFBO(), NonIntersect};
}
