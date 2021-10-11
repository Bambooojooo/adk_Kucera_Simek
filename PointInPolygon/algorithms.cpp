#include "algorithms.h"
#include <cmath>
#include <vector>
#include <iostream>

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint &a,QPoint &p1,QPoint &p2)
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
        return 1;

    //Point in the right halfplane
    if (t < -eps)
        return 0;

    //Point on the line
    return -1;
}

double Algorithms::get2LinesAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
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


int Algorithms::getPositionWinding(QPoint &q, std::vector<QPoint> &pol)
{
    //Analyze position of point and polygon
    int n = pol.size();
    double omega_sum=0;
    double eps_border = M_PI/36;

    //Process all segments of polygon
    for (int i = 0; i<n; i++)
    {
        // Angle between two line segments
        double omega = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

	//
	if (fabs(omega - M_PI) < eps_border || fabs(omega) < eps_border)
		return -1;

        // Point and line segment position
        int pos = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left halfplane
        if (pos==1)
            omega_sum += omega;
        else
            omega_sum -= omega;
    }

    //Point inside polygon
    double eps = 1.0e-5;
    if (fabs(fabs(omega_sum) - 2*M_PI) < eps)
        return 1;

    //Point outside polygon
    return 0;
}

int Algorithms::getPositionRay(QPoint &q, std::vector<QPoint> &pol)
{
	// Analyze position of a point and polygon via Ray crossing algorithm
	int n = pol.size();
	int k = 0;
	double x_, y_;
	double x_m;
	double eps = 1.0e-5;
	std::vector<QPoint> p_;

	//Process all segments of polygon
	for (int i = 0; i<n; i++)
	{
		// Compute local coordinates
		x_ = pol[i].x() - q.x();
		y_ = pol[i].y() - q.y();

		// Making QPoint object from local coords
		QPoint p(x_,y_);

		// Adding QPoint of local coords to vector p_
		p_.push_back(p);

		// Counting amount of points intersecting axis x
		if ((p_[i].y() > 0 && p_[(i-1)%n].y() < eps) || (p_[(i-1)%n].y() > 0 && p_[i].y() < eps))
		{
			// Evaluate x coord of a point intersecting axis x
			x_m = (p_[i].x() * p_[(i-1)%n].y() - p_[(i-1)%n].x() * p_[i].y())/(p_[i].y() - p_[(i-1)%n].y());
			std::cout << x_m << std::endl;
			// Increments k if x coord is in first or fourth quadrant
			if (x_m > 0)
				k++;
		}
	}

	// Point outside polygon
	if (k%2 == 0)
		return 0;
	// Point inside polygon
	else
		return 1;
}
