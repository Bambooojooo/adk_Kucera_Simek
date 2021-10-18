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
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

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
	//Remove quotes from line string
	line.erase(remove(line.begin(), line.end(), '"'), line.end());

	std::cout << "Line: " << line << std::endl;

	// Create a stringstream of the current line
	std::stringstream ss(line);

	//Keep track of the current column id
	int colId = 0;

	// Help vars
	int pairIterator = 0;
	int x, y;
	std::string value;
	QPolygon polygon;


	//Go through every object in csv line
	while(std::getline(ss, value, ','))
	{
		int pointsAmount = 0;

		//Column cursor is on coords column
		if (colId > 1)
		{
			//Skip blanks made by getline method
			if (value != "")
			{
				//If coordinate pair incomplete (must include both x and y)
				if (pairIterator != 2)
				{
					//If there is x coor to read
					if (pairIterator%2 == 0)
					{
						x = std::stoi(value);
						pairIterator++;
					}
					//If there is y coor to read
					else
					{
						y = std::stoi(value);
						pairIterator++;
					}
				}

				if (pairIterator == 2)
				{
					QPoint p(x,y);
					polygon << p;

					pairIterator = 0;
					pointsAmount++;
				}
			}
		}

	    // Increment the column index
	    colId++;
	}
	result.push_back(polygon);
    }

    // Close file
    myFile.close();

    return result;
}
