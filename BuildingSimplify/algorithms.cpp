#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"

#include <QtGui>
#include <iostream>

Algorithms::Algorithms()
{

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


QPolygon Algorithms::cHull (std::vector <QPoint> &points)
{
    //Create convex hull, Jarvis scan
    QPolygon ch;

    //Find pivot
    QPoint q=*std::min_element(points.begin(), points.end(), sortByY());

    //Add pivot to convex hull
    ch.append(q);

    //Create pj, pjj
    QPoint pj = q;
    QPoint pjj(0,q.y());

    do
    {
        //Find next convex hull point
        int i_max = -1;
        double om_max = 0;
        for (int i = 0; i<points.size(); i++)
        {
            double om = get2LinesAngle(pj, pjj, pj, points[i]);

            //Update maximum
            if (om > om_max)
            {
                om_max = om;
                i_max = i;
            }
        }

        //Add point to the convex hull
        ch.append(points[i_max]);

        //Update last two points of the convex hull
        pjj = pj;
        pj = points[i_max];

    } while (pj != q);

    return ch;
}


std::vector <QPoint> Algorithms::rotate(std::vector <QPoint> &points, double sigma)
{
    //Rotate dataset by angle
    std::vector <QPoint> r_points;
    for (int i = 0; i < points.size(); i++)
    {
        //Rotate point
        double x_r = points[i].x()*cos(sigma) - points[i].y()*sin(sigma);
        double y_r = points[i].x()*sin(sigma) + points[i].y()*cos(sigma);

        // Create point
        QPoint rp(x_r, y_r);

        //Add point to the list
        r_points.push_back(rp);
    }

    return r_points;
}


std::tuple<std::vector<QPoint>, double> Algorithms::minMaxBox(std::vector <QPoint> &points)
{
    //Return vertices of min-max box and its area
    double area = 0;

    //Return vertices with extreme coordinates
    QPoint pxmin = *std::min_element(points.begin(), points.end(), sortByX());
    QPoint pxmax = *std::max_element(points.begin(), points.end(), sortByX());
    QPoint pymin = *std::min_element(points.begin(), points.end(), sortByY());
    QPoint pymax = *std::max_element(points.begin(), points.end(), sortByY());

    //Create min-max box vertices
    QPoint v1(pxmin.x(),pymin.y());
    QPoint v2(pxmax.x(),pymin.y());
    QPoint v3(pxmax.x(),pymax.y());
    QPoint v4(pxmin.x(),pymax.y());

    //Create min-max box polygon
    std::vector<QPoint> mmb{v1,v2,v3,v4};

    //Calculate min-max box area
    area = (pxmax.x() - pxmin.x())*(pymax.y() - pymin.y());

    return {mmb, area};
}


QPolygon Algorithms::minAreaEnclosingRectangle(std::vector <QPoint> &points)
{
    //Create minimum area enclosing rectangle
    QPolygon ch = cHull(points);

    //Searching for min max box with min area
    int n = ch.size();
    double sigma_min=0;
    std::vector<QPoint> mmb_min;

    //Initializing area_min
    auto [mmb, area_min] = minMaxBox(points);

    for (int i=0; i<n; i++)
    {
         //Coordinate differences
         double dx = ch[(i+1)%n].x() - ch[i].x();
         double dy = ch[(i+1)%n].y() - ch[i].y();

         double sigma = atan2(dy, dx);

         //Rotate by -sigma
         std::vector<QPoint> r_points = rotate(points, -sigma);

         //Create min-max box
         auto [mmb, area] = minMaxBox(r_points);

         //Update minimum
	 if (area <= area_min)
         {
             area_min = area;
             sigma_min = sigma;
             mmb_min = mmb;
         }
     }
    //Create enclosing rectangle
    std::vector<QPoint> er = rotate(mmb_min, sigma_min);

    //Resize rectangle, preserve area of the building
    std::vector<QPoint> err = resizeRectangle(points,er);
    //Create QPolygon
    QPolygon er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);
    return er_pol;
}


QPolygon Algorithms::wallAverage(std::vector <QPoint> &points)
{
    //Create enclosing rectangle using wall average method
    double sigma = 0, si_sum = 0;
    QPolygon pol;

    //Compute initial direction
    double dx = points[1].x() - points[0].x();
    double dy = points[1].y() - points[0].y();
    double sigma_ = atan2(dy, dx);

    //Compute directions for segments
    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        //Compute direction and length
        double dxi = points[(i+1)%n].x() - points[i].x();
        double dyi = points[(i+1)%n].y() - points[i].y();
        double sigmai = atan2(dyi, dxi);
        double lengthi = sqrt(dxi*dxi + dyi*dyi);

        //Compute direction differences
        double dsigmai = sigmai - sigma_;
        if (dsigmai < 0)
            dsigmai += 2*M_PI;

        //Compute fraction
        double ki=round(dsigmai/(M_PI/2));

        //Compute reminder
        double ri=dsigmai-ki*(M_PI/2);

        //Weighted average sums
        sigma += ri*lengthi;
        si_sum += lengthi;
    }

    //Weighted average
    sigma = sigma_ + sigma/si_sum;

    //Rotate by -sigma
    std::vector<QPoint> r_points = rotate(points, -sigma);

    //Create min-max box
    auto [mmb, area] = minMaxBox(r_points);

    //Create enclosing rectangle
    std::vector<QPoint> er = rotate(mmb, sigma);

    //Resize rectangle, preserve area of the building
    std::vector<QPoint> err = resizeRectangle(points,er);

    //Create QPolygon
    QPolygon er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}


double Algorithms::LH(std::vector <QPoint> &points)
{
    //Get area of building by L' Huillier formula
    int n = points.size();
    double area = 0;

    //Proces all vertices of the building
    for (int i = 0; i < n; i++)
    {
        area += points[i].x() * (points[(i+1)%n].y() - points[(i-1+n)%n].y());
    }

    //Return unsigned area
    return 0.5 * fabs(area);
}


std::vector <QPoint> Algorithms::resizeRectangle(std::vector <QPoint> &points, std::vector <QPoint> &er)
{
        //Resize rectangle to given area

        //Building area
        double AB = LH(points);

	if (AB == 0)
		AB = 1;

        //Rectangle area
        double AR = LH(er);

	//Fraction of areas
	if (AR == 0)
		AR = 1;

	double k = AB/AR;

        //Center of mass
	double xc = (er[0].x() + er[1].x() + er[2].x() + er[3].x())/4;
	double yc = (er[0].y() + er[1].y() + er[2].y() + er[3].y())/4;

        //Compute vector components
        double u1x = er[0].x() - xc;
        double u1y = er[0].y() - yc;
        double u2x = er[1].x() - xc;
        double u2y = er[1].y() - yc;
        double u3x = er[2].x() - xc;
        double u3y = er[2].y() - yc;
        double u4x = er[3].x() - xc;
        double u4y = er[3].y() - yc;

        //Create new rectangle vertices
        QPoint v1_(xc + sqrt(k) * u1x, yc + sqrt(k) * u1y);
        QPoint v2_(xc + sqrt(k) * u2x, yc + sqrt(k) * u2y);
        QPoint v3_(xc + sqrt(k) * u3x, yc + sqrt(k) * u3y);
        QPoint v4_(xc + sqrt(k) * u4x, yc + sqrt(k) * u4y);

        //Add vertices to vector
        std::vector<QPoint> er_res = {v1_, v2_, v3_, v4_};

        return er_res;
}

QPolygon Algorithms::weightedBisector(std::vector <QPoint> &points)
{
	//Create enclosing rectangle using weighted bisector method
	int n = points.size();
	std::vector<std::pair<double, double>> lengths_angles;

	//Compute lengths of every diagonal and its direction and store as a pair
	for(int i=0; i<n; i++)
	{
		for (int j=i; j<n; j++)
		{
			//Coordinate differences
			double dx = points[i].x()-points[j].x();
			double dy = points[i].y()-points[j].y();

			//Compute length of diagonal
			double s = sqrt(dx*dx + dy*dy);

			//Compute direction of diagonal
			double sigma = atan2(dy,dx);
			if (sigma < 0)
			    sigma += 2*M_PI;

			lengths_angles.push_back(std::make_pair(s, sigma));
		}
	}

	//Sort lengths of diagonals in ascending order
	std::sort(lengths_angles.begin(), lengths_angles.end());

	//Store diagonal maximum length and its direction
	double s1 = lengths_angles.back().first;
	double sigma1 = lengths_angles.back().second;

	lengths_angles.pop_back();

	//Store diagonal second maximum length and its direction
	double s2 = lengths_angles.back().first;
	double sigma2 = lengths_angles.back().second;

	//Compute dominant building direction
	double sigma_main = (s1*sigma1 + s2*sigma2) / (s1+s2);

	//Rotate by -sigma
	std::vector<QPoint> r_points = rotate(points, -sigma_main);

	//Create min-max box
	auto [mmb, area] = minMaxBox(r_points);

	//Create enclosing rectangle
	std::vector<QPoint> er = rotate(mmb, sigma_main);

	//Resize rectangle, preserve area of the building
	std::vector<QPoint> err = resizeRectangle(points,er);

	//Create QPolygon
	QPolygon er_pol;
	er_pol.append(err[0]);
	er_pol.append(err[1]);
	er_pol.append(err[2]);
	er_pol.append(err[3]);

	return er_pol;
}

QPolygon Algorithms::longestEdge(std::vector <QPoint> &points)
{
	//Create enclosing rectangle using Longest Edge method
	int n = points.size();
	std::vector<std::pair<double, double>> lengths_angles;

	//Compute lengths of every edge and its direction and store as a pair
	for(int i=0; i<n; i++)
	{
		//Coordinate differences
		double dx = points[i].x()-points[(i+1)%n].x();
		double dy = points[i].y()-points[(i+1)%n].y();

		//Compute length of edge
		double s = sqrt(dx*dx + dy*dy);

		//Compute direction of edge
		double sigma = atan2(dy,dx);
		if (sigma < 0)
		    sigma += 2*M_PI;

		lengths_angles.push_back(std::make_pair(s, sigma));
	}

	//Sort lengths of edges in ascending order
	std::sort(lengths_angles.begin(), lengths_angles.end());

	//Store direction of longest edge
	double sigma_main = lengths_angles.back().second;

	//Rotate by -sigma main
	std::vector<QPoint> r_points = rotate(points, -sigma_main);

	//Create min-max box
	auto [mmb, area] = minMaxBox(r_points);

	//Create enclosing rectangle
	std::vector<QPoint> er = rotate(mmb, sigma_main);

	//Resize rectangle, preserve area of the building
	std::vector<QPoint> err = resizeRectangle(points,er);

	//Create QPolygon
	QPolygon er_pol;
	er_pol.append(err[0]);
	er_pol.append(err[1]);
	er_pol.append(err[2]);
	er_pol.append(err[3]);

	return er_pol;
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

QPolygon Algorithms::cHullGraham(std::vector <QPoint> &points)
{
    //Create convex hull, Graham scan

    //Numeric precision for comparision of angles
    double eps = 1e-9;
    //Convex hull
    QPolygon ch;

    //Find pivot
    QPoint q=*std::min_element(points.begin(), points.end(), sortByY());

    //Add pivot to the convex hull
    ch.append(q);

    //x point is on X axis
    QPoint x(q.x() -100, q.y());

    //Create vector of pairs - sigma/index
    std::vector<std::pair<double, int>> angle_index;

    //Compute sigma - angle measured from the x parallel
    for (int i =0; i < points.size(); i++)
    {
        double sigma = get2LinesAngle(q, x, q, points[i]);

        //Insert pairs into vector
        if (std::isnan(sigma))
            angle_index.push_back(std::make_pair(0, i));
        else
            angle_index.push_back(std::make_pair(sigma, i));
    }

    //Sort points by sigma
    std::sort(angle_index.begin(), angle_index.end());

    //Delete closer points with the same angle
    for (int i = 0; i < angle_index.size()-1; i++)
    {
        if (fabs(angle_index[i].first - angle_index[i+1].first) < eps)
        {
            double l1 = distance(points[angle_index[i].second], q);
            double l2 = distance(points[angle_index[i+1].second], q);

            if (l1 < l2)
                points.erase(points.begin() + angle_index[i].second);
            else
                points.erase(points.begin() + angle_index[i+1].second);
        }

    }

    //Add point with smallest sigma into the convex hull
    ch.append(points[angle_index[1].second]);

    //Create the convex hull
    for (int j =2; j < angle_index.size(); j++)
    {
        while (getPointLinePosition(points[angle_index[j].second],ch[ch.size() -1],ch[ch.size() -2]) != 1)
            ch.pop_back();
        ch.push_back(points[angle_index[j].second]);
    }

    return ch;
}

double Algorithms::distance(QPoint p1, QPoint p2)
{
    //Compute 2D distance of 2 inserted points
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();

    return std::sqrt(dx*dx + dy*dy);
}

