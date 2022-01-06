#include "csv.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <QDir>

std::vector<std::vector<std::string>> CSV::read_csv(std::string &filename)
{
    //Reads a CSV file into a vector of string vectors
    std::vector<std::string> lines;
    std::vector<std::vector<std::string>> result;

    //Help vars
    std::string csv_line, csv_colnames;

    //Create an input filestream
    std::ifstream myFile(filename);

    //Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    //Read the column names
    if(myFile.good())
    {
	//Extract the first line in the file
	std::getline(myFile, csv_line);

	//Create a stringstream from line
	std::stringstream ss(csv_line);

	//Extract each column name
	while(std::getline(ss, csv_colnames, ','))
		continue;
    }

    //Read data, line by line
    while(std::getline(myFile, csv_line))
    {
	    //Create a stringstream of the current line
	    std::stringstream ss1(csv_line);
	    std::string field;

	    //Go through every field in line
	    while(std::getline(ss1, field, ','))
		    lines.push_back(field);

	    result.push_back(lines);
	    lines.clear();

    }

    // Close file
    myFile.close();

    return result;
}

std::vector<std::pair<std::string, QPointFBO>> CSV::getPoints(std::vector<std::vector<std::string>> &csv_content, double &x_min, double &x_max, double &y_min, double &y_max)
{
	//Return vector of QPoints3D from vector of lines (csv lines) and update minmax box
    std::vector<std::pair<std::string, QPointFBO>> points;

	//Point variables declaration
	int id;
    double x, y;
    std::string polygon;

	//Go through each line (point)
	for(std::vector<std::string> line:  csv_content)
	{
		//Convert strings to integer and double values
        polygon = line[0];
        id = std::stoi(line[1]);
        x  = std::stod(line[2]);
        y  = std::stod(line[3]);

		//Update minmax box
		if(x < x_min)
			x_min = x;

		if(x > x_max)
			x_max = x;

		if(y < y_min)
			y_min = y;

		if(y > y_max)
			y_max = y;

        points.push_back(std::make_pair(polygon, QPointFBO(x, y)));
	}

    return points;
}
