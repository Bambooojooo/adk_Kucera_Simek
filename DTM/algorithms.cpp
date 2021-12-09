#include "algorithms.h"
#include "sortbyx.h"

#include <list>
#include <cmath>
#include <iostream>
#include <random>
#include <iterator>
#include <map>

Algorithms::Algorithms()
{
    srand (time(NULL));
}

int Algorithms::getPointLinePosition(QPoint3D &a,QPoint3D &p1,QPoint3D &p2)
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

std::tuple<QPoint3D,double>Algorithms::getCircleCenterAndRadius(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3)
{
    //Return center and radius of inscribed circle
    double k1 = p1.x() * p1.x() + p1.y() * p1.y();
    double k2 = p2.x() * p2.x() + p2.y() * p2.y();
    double k3 = p3.x() * p3.x() + p3.y() * p3.y();
    double k4 = p1.y() - p2.y();
    double k5 = p1.y() - p3.y();
    double k6 = p2.y() - p3.y();
    double k7 = p1.x() - p2.x();
    double k8 = p1.x() - p3.x();
    double k9 = p2.x() - p3.x();
    double k10 = p1.x() * p1.x();
    double k11 = p2.x() * p2.x();
    double k12 = p3.x() * p3.x();

    //Center of inscribed circle
    double m = 0.5 *(-k12 * k4 + k11 * k5 - (k10 + k4 * k5) * k6)/(-p3.x() * k4 + p2.x() * k5 - p1.x() * k6);
    double n = 0.5 * (-k1 * k9  + k2 * k8 - k3 * k7)/(-p1.y() * k9  + p2.y()  * k8 - p3.y()  * k7);
    double r = sqrt((p1.x() - m) * (p1.x() - m) + (p1.y() - n) * (p1.y() - n));

    //Create center
    QPoint3D c(m,n);

    return {c,r};
}

int Algorithms::getDelaunayPoint(QPoint3D &s,QPoint3D &e,std::vector<QPoint3D> &points)
{
    //Find and return Delaunay point
    int min_index = -1;
    double min_r = 1.e16;

    for (int i=0; i<points.size(); i++)
    {
        //Point is different from s,e
        if((points[i]!=s) && (points[i]!=e))
        {
            // Point in left halfplane
            if(getPointLinePosition(points[i],s,e)==1)
            {
                //Center and radius of inscribed circle
                auto[c,r] = getCircleCenterAndRadius(points[i],s,e);

                //Correct radius
                if(getPointLinePosition(c,s,e)==0)
                    r = -r;

                //Update minimum radius
                if (r < min_r)
                {
                   min_r = r;
                   min_index = i;
                }
            }
        }
    }

    return min_index;
}

int Algorithms::getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find nearest point
    double min_dist = 1.e16;
    int min_ind = -1;

    for (int i = 0; i < points.size(); i++)
    {
        //Point is different from p
        if((points[i]!=p))
        {
            double dist = sqrt((points[i].x()-p.x())*(points[i].x()-p.x())+(points[i].y()-p.y())*(points[i].y()-p.y()));

            //Update minimum
            if (dist < min_dist)
            {
                min_dist = dist;
                min_ind = i;
            }
        }
    }

    return min_ind;
}

std::vector<Edge> Algorithms::dT(std::vector<QPoint3D> &points)
{
    //Create Delaunay triangulation using incremental method
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Find pivot (minimum x)
    QPoint3D q = *min_element(points.begin(), points.end(), sortByX());

    //Point nearest to pivot q
    int i_nearest = getNearestPoint(q, points);
    QPoint3D qn = points[i_nearest];

    //Create new edge
    Edge e(q, qn);

    //Find optimal Delaunay point
    int i_point = getDelaunayPoint(q, qn, points);

    //Point has not been found, change orientation, search again
    if (i_point == -1)
    {
        e.changeOrientation();
        i_point = getDelaunayPoint(qn, q, points);
    }

    //Delaunay point + 3rd vertex
    QPoint3D v3 = points[i_point];

    //Create edges
    QPoint3D es = e.getStart();
    QPoint3D ee = e.getEnd();
    Edge e2(ee, v3);
    Edge e3(v3, es);

    //Adding 3 edges to DT
    dt.push_back(e);
    dt.push_back(e2);
    dt.push_back(e3);

    //Adding 3 edges to AEL
    ael.push_back(e);
    ael.push_back(e2);
    ael.push_back(e3);

    //Proces edges until AEL is empty
    while(!ael.empty())
    {
        //get last edge
        e = ael.back();
        ael.pop_back();

        //Change orientation
        e.changeOrientation();

        //Find optimal Delaunay point
        QPoint3D qs = e.getStart();
        QPoint3D qe = e.getEnd();
        i_point = getDelaunayPoint(qs, qe, points);

        //Point has been found
        if (i_point != -1)
        {
            //Delaunay point + 3rd vertex
            v3 = points[i_point];

            //Create edges
            es = e.getStart();
            ee = e.getEnd();
            Edge e2(ee, v3);
            Edge e3(v3, es);

            //Add 3 edges to DT
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);

            //Update AEL
            updateAEL(e2, ael);
            updateAEL(e3, ael);
        }
    }
    std::cout<< dt.size() << std::endl;
    return dt;
}

void Algorithms::updateAEL(Edge &e, std::list<Edge> &ael)
{
    //Update AEL

    //Switch orientation
    e.changeOrientation();

    //Look for e3 in AEL
    auto ie = std::find(ael.begin(),ael.end(),e);

    //E2 has not been found, add to the list
    if(ie == ael.end())
    {
        e.changeOrientation();
        ael.push_back(e);
    }

    //Erase from ael
    else
        ael.erase(ie);
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Get countour point intersection of triangle and horizontal countour
    double xb = (p2.x()-p1.x())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.x();
    double yb = (p2.y()-p1.y())/(p2.getZ()-p1.getZ())*(z-p1.getZ())+p1.y();

    return QPoint3D(xb, yb, z);
}

std::vector<Edge> Algorithms::getContourLines(std::vector<Edge> &dt, double zmin, double zmax, int dz)
{
    //Get countour lines from delaunay triangulation
    std::vector<Edge> contours;

    //Process all triangles of dt
    for (int i = 0; i < dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Get height of points
        double z1 = p1.getZ();
        double z2 = p2.getZ();
        double z3 = p3.getZ();

        //Check all horizontal planes
        for (double z = zmin-((int)zmin%dz); z <= zmax; z+=dz)
        {
            //Height differences
            double dz1 = z1 - z;
            double dz2 = z2 - z;
            double dz3 = z3 - z;

            //Edge intersected by plane?
            double dz12 = dz1*dz2;
            double dz23 = dz2*dz3;
            double dz31 = dz3*dz1;

            //Triangle is complanar
            if ((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge p1-p2 is collinear
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge p2-p3 is collinear
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge p3-p1 is collinear
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Plane intersects edges p1-p2 and p2-p3
            else if (((dz12 <= 0) && (dz23 < 0)) || ((dz12 < 0) && (dz23 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p1, p2, z);
                QPoint3D B = getContourPoint(p2, p3, z);

                Edge ab(A, B);
                //Create edge and add it to the list
                if (p2.getZ() > p1.getZ())
                    Edge ab(B, A);

                contours.push_back(ab);
            }

            //Plane intersects edges p2-p3 and p3-p1
            else if (((dz23 <= 0) && (dz31 < 0)) || ((dz23 < 0) && (dz31 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p2, p3, z);
                QPoint3D B = getContourPoint(p3, p1, z);

                //Create edge and add it to the list
                Edge ab(A, B);
                if (p3.getZ() > p2.getZ())
                    Edge ab(B, A);
                contours.push_back(ab);
            }

            //Plane intersects edges p3-p1 and p1-p2
            else if (((dz31 <= 0) && (dz12 < 0)) || ((dz31 < 0) && (dz12 <= 0)))
            {
                //Compute intersections
                QPoint3D A = getContourPoint(p3, p1, z);
                QPoint3D B = getContourPoint(p1, p2, z);

                //Create edge and add it to the list
                Edge ab(A, B);
                if (p1.getZ() > p3.getZ())
                    Edge ab(B, A);

                contours.push_back(ab);
            }
        }
    }

    return contours;
}

double Algorithms::getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute slope of triangle
    double ux = p1.x() - p2.x();
    double uy = p1.y() - p2.y();
    double uz = p1.getZ() - p2.getZ();

    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Normal vector
    double nx = uy*vz - vy*uz;
    double ny = -ux*vz + vx*uz;
    double nz = ux*vy - vx*uy;

    //Norm
    double n = sqrt(nx*nx + ny*ny + nz*nz);

    return acos(nz/n);
}

double Algorithms:: getExposition(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute exposition (direction of the projected normal vector)
    double ux = p1.x() - p2.x();
    double uy = p1.y() - p2.y();
    double uz = p1.getZ() - p2.getZ();

    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Normal vector
    double nx = uy*vz - vy*uz;
    double ny = -ux*vz + vx*uz;
    double nz = ux*vy - vx*uy;

    //Direction of the vector
    return atan2(nx, ny);
}

std::vector<Triangle> Algorithms::analyzeDTM(std::vector<Edge> &dt)
{
    //Computing slope and exposition for each triangle
    std::vector<Triangle> triangles;
    for (int i = 0; i < dt.size(); i+=3)
    {
        //Get triangle edges
        Edge e1 = dt[i];
        Edge e2 = dt[i+1];

        //Get triangle vertices
        QPoint3D p1 = e1.getStart();
        QPoint3D p2 = e1.getEnd();
        QPoint3D p3 = e2.getEnd();

        //Compute slope and exposition
        double slope = getSlope(p1, p2, p3);
        double exposition = getExposition(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, slope, exposition);

        //Add triangle to the list
        triangles.push_back(t);
    }

    return triangles;
}

std::vector<QPoint3D> Algorithms::generatePile(std::vector<QPoint3D> &points)
{
    //Generate pile by z = x^2 + y^2 equation

    int n= points.size();

    //Compute z coordinates
    double z;
    for (int i = 0; i<n; i+=1)
    {
        double dh = points[i].y() - points[n-1].y();
        double dw = points[i].x() - points[n-1].x();

        z = - dh*dh - dw*dw + rand() % 10;
        points[i].setZ(z/200+1000);

    }

    return points;
}

double Algorithms::pointDist(QPoint3D &p1, QPoint3D &p2)
{
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();

    return sqrt(dx*dx + dy*dy);
}

std::vector<QPoint3D> Algorithms::generateRandomPoints(QSize &size_canvas, int n)
{
    //Generate vector of random points, last point in vector is centre of points
    std::vector<QPoint3D> points;

    double height, width;
    double tw=0, th=0;
    for (int i = 0; i<n; i+=1)
    {
        //Generate random coordinates
        height = rand() % size_canvas.height();
        width = rand() % size_canvas.width();

        points.push_back(QPoint3D(width, height));

        tw+=width;
        th+=height;
    }

    tw=tw/n;
    th=th/n;

    points.push_back(QPoint3D(tw, th));

    return points;
}

std::vector<QPoint3D> Algorithms::generateSaddle(std::vector<QPoint3D> &points)
{
    //Generate saddle by z=x*y equation

    int n= points.size();

    for (int i = 0; i<n; i+=1)
    {
        double y = points[i].y() - points[n-1].y();
        double x = points[i].x() - points[n-1].x();

        double z = (x*y)/250 + 500 + rand() % 5;

        points[i].setZ(z);
    }
    return points;
}

int Algorithms::findMaxZ(std::vector<QPoint3D> &points)
{
    //Find maximum height from points
    int max=-1e6;

    for (QPoint3D p : points)
    {
        if (p.getZ() > max)
            max=p.getZ();
    }
    return max;
}
int Algorithms::findMinZ(std::vector<QPoint3D> &points)
{
    //Find minimum height from points
    int min=1e6;

    for (QPoint3D p : points)
    {
        if (p.getZ() < min)
            min=p.getZ();
    }
    return min;
}

std::vector<QPoint3D> Algorithms::generateRidge(std::vector<QPoint3D> &points)
{
    //Generate cylinder and pick up him by x coordinate
    int n= points.size();


    for (int i = 0; i<n; i+=1)
    {
        double y = points[i].y() - points[n-1].y();
        //Height of cylinder
        double z = sqrt((points[n-1].y()+40)*(points[n-1].y()+40) - y*y) + points[i].x() + rand() % 10;

        points[i].setZ(z);
    }
    return points;
}

std::vector<QPoint3D> Algorithms::transformPoints(std::vector<QPoint3D> &points_3d, double &trans_x, double &trans_y, double &scale, int &offset_x, int &offset_y)
{
    //Transform polygon coorinates by basic transformation based on minmax box of dataset
    //x_min, x_max, y_min, y_max represent boundaries of dataset minmax box
    std::vector<QPoint3D> points_transformed;

    for (QPoint3D p : points_3d)
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

        points_transformed.push_back(QPoint3D(x0, y0, p.getZ()));
    }

    //Compute transformation key
    return points_transformed;
}

std::map<double, std::vector<Edge>> Algorithms::getMainContourLines(std::vector<Edge> &contours, int contour_interval, double dz)
{
    //Return main contours divided into groups by height
    std::map<double, std::vector<Edge>> contours_main;

    //Height interval for main contour lines
    int interval_z = dz*contour_interval;

    for (Edge c:contours)
    {
        //Height of contour line
        int contour_z = c.getStart().getZ();

        //Main contour lines
        if ((contour_z)%interval_z == 0)
        {
            if (contours_main.find(contour_z) == contours_main.end())
                contours_main.insert(std::pair<double, std::vector<Edge>>(contour_z, {c}));
            else
                contours_main[contour_z].push_back(c);
        }
    }
    return contours_main;
}

std::vector<Edge> Algorithms::getLabeledContours(std::vector<Edge> &contours, std::vector<Edge> &contours_main, int contour_interval, double dz, double &distance_threshold, double &length_threshold, double &offset)
{
    //Get contour lines where height labels needs to be labeled
    std::vector<Edge> distanced_edges;
    std::vector<Edge> contours_labeled;

    std::map<double, std::vector<Edge>> main_contours = getMainContourLines(contours, contour_interval, dz);

    for (std::pair<double, std::vector<Edge>> element : main_contours)
    {
        //Append main conture lines for drawing
        contours_main.insert(contours_main.begin(), element.second.begin(), element.second.end());

        //Separate multiple contour lines of the same height to single continuous contour lines
        std::vector<std::vector<Edge>> contours_separated = chainEdges(element.second);

        //Process single contour lines to get labels
        for (std::vector<Edge> contour_line:contours_separated)
        {
            if (!contour_line.empty())
            {
                //Get distanced edges for labeling
                distanced_edges = getDistancedEdges(contour_line, distance_threshold, length_threshold, offset);

                //Append those label edges
                contours_labeled.insert(contours_labeled.end(), distanced_edges.begin(), distanced_edges.end());
            }
        }
    }
    return contours_labeled;
}

std::vector<Edge> Algorithms::getDistancedEdges(std::vector<Edge> &edges, double &distance_threshold, double &length_threshold, double &offset)
{
    //Get vector of edges where labels will be distances by given threshold
    std::vector<Edge> distanced_edges;
    int n = edges.size();

    distanced_edges.push_back(edges[rand()%n]);

    for (Edge e : edges)
    {
        //Get start point, get end point
        QPoint3D s_point = e.getStart();
        QPoint3D e_point = e.getEnd();

        //If edge length is longer than given offset
        if (Algorithms::pointDist(s_point, e_point) > length_threshold)
        {
            //Center of contour line
            QPoint3D center_e((e.getStart().x()+e.getEnd().x())/2, (e.getStart().y()+e.getEnd().y())/2);

            if (rand()%100 < 10)
            {
                bool noCloseEdge = true;
                //Comparing distance of label positions
                for (Edge de : distanced_edges)
                {
                        //Center of contour line
                        QPoint3D center_de((de.getStart().x()+de.getEnd().x())/2, (de.getStart().y()+de.getEnd().y())/2);

                        //Get distance of centers
                        double distance = Algorithms::pointDist(center_e, center_de);

                        //Push back point if it is far enough
                        if (distance < distance_threshold+offset)
                            noCloseEdge = false;
                }
                if (noCloseEdge)
                    distanced_edges.push_back(e);
            }
        }
    }
    return distanced_edges;
}

double Algorithms::getMinSlope(std::vector<Triangle> &triangles)
{
    double min = 10e16;

    for (Triangle t:triangles)
    {
        if (t.getSlope() < min)
            min = t.getSlope();
    }

    return min;
}
double Algorithms::getMaxSlope(std::vector<Triangle> &triangles)
{
    double max = 10e-16;

    for (Triangle t:triangles)
    {
        if (t.getSlope() > max)
            max = t.getSlope();
    }

    return max;
}

std::vector<std::vector<Edge>> Algorithms::chainEdges(std::vector<Edge> &edges)
{
    //Groups coresponding contour lines
    bool connection_segment_exists;
    std::list<int> add_ids;
    std::vector<std::vector<Edge>> edges_chained;
    std::list<std::vector<Edge>> edge_list;
    std::vector<std::vector<Edge>> edge_vector;

    for (Edge e:edges)
    {
        //Vector is empty yet
        if (edge_vector.empty())
            edge_vector.push_back({e});

        else
        {
            //Iterate trough all potential separated contours (chains)
            for (int i=0; i<edge_vector.size(); i++)
            {
                //Bool determining wether we want add edge to newly created separate contour line (chain) in the list or add it to already existing chain.
                //True => create new chain and add edge to it; False => add edge to existing chain
                connection_segment_exists = false;

                //Go through every edge (segment) of contour line (chain)
                for (Edge segment:edge_vector[i])
                {
                    //If edge connects to another edge save iterator of this particular contour (chain)
                    if (e.isEdgeConnection(segment) && !connection_segment_exists)
                    {
                        connection_segment_exists = true;
                        add_ids.push_back(i);
                        break;
                    }
                }
            }
            //No iterator stored, creating new contour
            if (add_ids.size() == 0)
                edge_vector.push_back({e});

            //Only one iterator stored, appending to a contour line
            else if (add_ids.size() == 1)
            {
                edge_vector[add_ids.front()].push_back(e);
                add_ids.pop_front();
            }
            //More than one iterator saved, appending to a contour line and merging with other contour lines
            else
            {
                //Append edge to first found contour and remove saved iterator of this contour
                int main_merge_contour = add_ids.front();
                add_ids.pop_front();
                edge_vector[main_merge_contour].push_back(e);


                //Process the rest of iterators to join contours with the frist one
                while (!add_ids.empty())
                {
                    edge_vector[main_merge_contour].insert(edge_vector[main_merge_contour].end(), edge_vector[add_ids.front()].begin(), edge_vector[add_ids.front()].end());
                    edge_vector[add_ids.front()].clear();
                    add_ids.pop_front();
                }
            }
        }
    }
    return edge_vector;
}
