#include "csv.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <QDir>

CSV::CSV()
{

}

std::vector<QPolygon> CSV::read_csv(std::string &filename)
{
    // Reads a CSV file into a vector of QPolygons

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<QPolygon> result;

    // Helper vars
    std::string line, colname;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Read the column names
    if(myFile.good())
    {
	// Extract the first line in the file
	std::getline(myFile, line);

	// Create a stringstream from line
	std::stringstream ss(line);

	// Extract each column name
	while(std::getline(ss, colname, ',')){
	}
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
	//Remove quotes, round brackets and first from line string
	line.erase(remove(line.begin(), line.end(), '"'), line.end());
	line.erase(remove(line.begin(), line.end(), '('), line.end());
	line.erase(remove(line.begin(), line.end(), ')'), line.end());

	//Remove first 12 symbols of string (which is "MULTIPOLYGON")
	line.erase(0,12);

	// Create a stringstream of the current line
	std::stringstream ss1(line);

	//Keep track of the current column id
	int colId = 0;

	// Help vars
	int pairIterator = 0;
	int x, y;
	std::string coordinate_pair, coordinate;
	QPolygon polygon;

	//Go through every point in polygon
	while(std::getline(ss1, coordinate_pair, ','))
	{
		std::stringstream ss2(coordinate_pair);

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
					x = (std::stod(coordinate)-668000)/2;
//					x = (std::stod(coordinate));
					pairIterator++;
				}
				//If there is y coor to read
				else if (pairIterator == 1)
				{
					//Convert string value to integer value
					y = (std::stod(coordinate)-1130000)/2 +200;
//					y = (std::stod(coordinate));
					pairIterator++;
				}
			}
		}

		//Store the pair of coordinates and reset pair iterator
		if (pairIterator == 2)
		{
			std::cout << "x: " << x << ", y: " << y << std::endl;
			QPoint p(x,y);
			polygon << p;
			pairIterator = 0;
		}
	    // Increment the column index
	    colId++;
	}
	//Store the object of a line
//	std:: cout << polygon.size() << std::endl;
	result.push_back(polygon);
    }

    // Close file
    myFile.close();

    return result;
}
