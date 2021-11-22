#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>
#include <QtGui>

class CSV
{
public:
	CSV();

	static std::vector<QPolygon> read_csv(std::string &filename, double &x_min, double &x_max, double &y_min, double &y_max);
};

#endif // CSV_H
