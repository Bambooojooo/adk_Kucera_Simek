#include "algorithms.h"
#include <cmath>
#include <vector>
#include <iostream>

Algorithms::Algorithms()
{

}

bool Algorithms::ifCloseToPoint (QPoint &q, std::vector<QPoint> &pol)
{
	// Decide whether the point q is close to any polygon point
	int n = pol.size();
	double eps = 4.1;
	double dist;

	//Process all segments of polygon
	for (int i = 0; i<n; i++)
	{
		// Calculate distance of each point of polygon to point q
		dist = sqrt((pol[i].x()-q.x()) * (pol[i].x()-q.x()) + (pol[i].y()-q.y()) * (pol[i].y()-q.y()));

		//Point q is near of a polygon point
		if (dist < eps)
			return 1;
	}

	//Point q is not near of any polygon point
	return 0;
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

std::vector<QPoint> Algorithms::getLocalCoords(QPoint &q, std::vector<QPoint> &pol)
{
	int n = pol.size();
	int x_, y_;
	std::vector<QPoint> p_;

	//Process all segments of polygon
	for (int i = 0; i<n; i++)
	{
		// Compute local coordinates
		x_ = pol[i].x() - q.x();
		y_ = pol[i].y() - q.y();

//		std::cout << "p_[" << i << "].y - " << "q.y()" << std::endl;
//		std::cout << pol[i].y() <<" - " << q.y() << std::endl;
//		std::cout << y_ << std::endl;

		// Making QPoint object from local coords
		QPoint p(x_,y_);

		// Adding QPoint of local coords to vector p_
		p_.push_back(p);
	}
	return p_;
}

int Algorithms::getPositionRay(QPoint &q, std::vector<QPoint> &pol)
{
	// Analyze position of a point and polygon via Ray crossing algorithm
	int n = pol.size();
	int k = 0;
	double x_m, y_m;
	double eps = 4.1;

	std::vector<QPoint> p_ = Algorithms::getLocalCoords(q, pol);

	for (int i = 0; i<n; i++)
	{

		//Bools if there are intersections of axis and polygon edges
		bool ifAxisXIntersection = ((p_[i].y() > 0 && p_[(i-1+n)%n].y() < eps) || (p_[(i-1+n)%n].y() > 0 && p_[i].y() < eps));
		bool ifAxisYIntersection = ((p_[i].x() > 0 && p_[(i-1+n)%n].x() < eps) || (p_[(i-1+n)%n].x() > 0 && p_[i].x() < eps));

		//std::cout << "Oy : " << ifAxisYIntersection << std::endl;
		//std::cout << "p_[" << i << "].y : " << p_[i].y() << std::endl;
		//std::cout << "p_[" << (i-1+n)%n << "].y : " << p_[(i-1+n)%n].y() << std::endl;

		// Counting amount of points intersecting axis x
		if (ifAxisXIntersection || ifAxisYIntersection)
		{
			// X coord of axis x intersection
			x_m = (-1) * (p_[i].x() * p_[(i-1+n)%n].y() - p_[(i-1+n)%n].x() * p_[i].y())/(p_[i].y() - p_[(i-1+n)%n].y());
			std::cout << "hrana " << (i-1+n)%n << " " << i << std::endl;
			std::cout << "x_m : " << x_m << ", so: " << (fabs(x_m) < eps) << std::endl;

			// Y coord of axis y intersection
			y_m = (-1) * (p_[(i-1+n)%n].x() * (p_[(i-1+n)%n].y() - p_[i].y()) / (p_[i].x() - p_[(i-1+n)%n].x()) + p_[(i-1+n)%n].y());
			//std::cout << "y_m : " << y_m << ", so: " << (fabs(y_m) < eps) << std::endl;


			// Return "on the border" if intersection is too close to any axis
			if (fabs(x_m) < eps || fabs(y_m) < eps)
				return -1;
			// Increment k number (number of intersections of a polygon edges with axis x on its first and fourth quadrant
			else if (x_m > eps && ifAxisXIntersection)
				k++;

		}

	}
	//std::cout << "k : " << k << std::endl;
	// Point outside polygon
	std::cout << "k: " << k << std::endl;
	if (k%2 == 0)
		return 0;
	// Point inside polygon
	else
		return 1;
}
