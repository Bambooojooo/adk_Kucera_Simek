#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>
#include <QtGui>


class CSV
{
private:

public:
	CSV();

	std::vector<QPolygon> read_csv(std::string &filename);
};

#endif // CSV_H
