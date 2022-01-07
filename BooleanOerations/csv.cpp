#include "csv.h"

#include <algorithm>
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

std::vector<TPolygon> CSV::getCSVPolygons(std::vector<std::vector<std::string>> &csv_content, double &x_min, double &x_max, double &y_min, double &y_max)
{
	//Return vector of QPoints3D from vector of lines (csv lines) and update minmax box
    std::vector<std::pair<std::string, QPointFBO>> points;
    std::vector<TPolygon> result;

    //Go through each line (polygon)
    for(std::vector<std::string> polygon_points:  csv_content)
	{
        TPolygon polygon;

        //Remove first symbols of string wich are not digits (which is usually some WKT text)
        while ((!isdigit(polygon_points[0][0])) && (polygon_points[0][0] != '-'))
            polygon_points[0].erase(0,1);



        for (std::string point: polygon_points)
        {
            //Remove quotes, round brackets and first from line string
            point.erase(std::remove(point.begin(), point.end(), '"'), point.end());
            point.erase(std::remove(point.begin(), point.end(), '('), point.end());
            point.erase(std::remove(point.begin(), point.end(), ')'), point.end());

            // Help vars
            int pairIterator = 0;
            int x, y;
            std::string coordinate;
            std::stringstream ss2(point);

            //Go through every coordinate in point
            while(std::getline(ss2, coordinate, ' '))
            {
                if (coordinate == "")
                    continue;

                //If coordinate pair incomplete (must include both x and y)
                if (pairIterator < 2)
                {
                    //If there is x coor to read
                    if (pairIterator == 0)
                    {
                        //Convert string value to integer value
                        x = (std::stod(coordinate));

                        //Update minmax box coors
                        if (x < x_min)
                            x_min = x;

                        if (x > x_max)
                            x_max = x;

                        pairIterator++;
                    }
                    //If there is y coor to read
                    else if (pairIterator == 1)
                    {
                        //Convert string value to integer value
                        y = -(std::stod(coordinate));

                        //Update minmax box coors
                        if (y < y_min)
                            y_min = y;

                        if (y > y_max)
                            y_max = y;

                        pairIterator++;
                    }
                }
            }

            //Store the pair of coordinates and reset pair iterator
            if (pairIterator == 2)
            {
                polygon.push_back(QPointFBO(x,y));
                pairIterator = 0;
            }
        }
        //Store the object of a line
        result.push_back(polygon);
    }
    return result;
}
