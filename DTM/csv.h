#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>
#include <QtGui>

#include "qpoint3d.h"

class CSV
{
public:
	static std::vector<std::vector<std::string>> read_csv(std::string &filename);
	static std::vector<QPoint3D> getPoints3D(std::vector<std::vector<std::string>> &csv_content, double &x_min, double &x_max, double &y_min, double &y_max);
};

#endif // CSV_H
