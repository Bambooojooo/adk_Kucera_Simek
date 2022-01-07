#ifndef CSV_H
#define CSV_H

#include "qpointfbo.h"
#include "types.h"

#include <string>
#include <vector>
#include <QtGui>

class CSV
{
public:
	static std::vector<std::vector<std::string>> read_csv(std::string &filename);
    static std::vector<TPolygon> getCSVPolygons(std::vector<std::vector<std::string>> &csv_content, double &x_min, double &x_max, double &y_min, double &y_max);
};

#endif // CSV_H
