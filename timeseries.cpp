#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "timeseries.h"

using namespace std;

/*
 * Function role: load all the data from the given file. It will iterate every line in the file, and will add
 * each value to the right feature.
 */
void TimeSeries::loadFile(const char *CSVfileName)
{
    ifstream infile(CSVfileName);
    string sLine;
    vector<float> temp; // will hold the values of each features

    if (infile.good())
    {
        getline(infile, sLine);
    }
    std::stringstream firstLine(sLine);

    while(firstLine.good()) // create the features vector from the first line in the file
    {
        string substr;
        getline(firstLine, substr, ','); // ignore commas
        features.push_back(substr); // check - remove!
        mapData.insert(pair<string, vector<float>>(substr, temp)); // add every feature to the map with empty vector
    }

    string line;

    while(getline(infile, line)) // iterating the lines
    {
        numOfLines++;
        float temp;
        vector<float> row; // temporary vector that holds the data from each row
        stringstream lineStream(line);

        while(lineStream >> temp) // adding the values in the specific row to the vector
        {
            if(lineStream.peek() == ',') lineStream.ignore(); // ignore commas
            row.push_back(temp);
        }
        copyVector(row);
    }
    infile.close();
}

/*
 * Function role: with a given vector, the function role is to add any of the element in the vector,
 * to the current key (=feature) in the map.
 */
void TimeSeries::copyVector(vector<float> vec)
{
    auto it = mapData.begin(); // declaring an iterator for the map
    int helper = 0; // will help us to identify if we are in the correct key

    for(it = mapData.begin(); it != mapData.end(); ++it) // iterating over the map
    {
        mapData[it->first].push_back(vec[helper]); // adding each piece of data to the current key
        helper++;
    }
    numOfFeatures = helper;
}

/*
 * Function role: return the number of features in the map.
 */
int TimeSeries::getNumOfFeatures() const { return numOfFeatures; }

/*
 * Function role: return the number of lines in the file.
 */
int TimeSeries::getNumOfLines() const { return numOfLines; }

/*
 * Function role: get the vector object that holds the features names.
 */
vector<string> TimeSeries::getFeatures() const { return features; }

/*
 * Function role: get the map object that holds the CSV data.
 */
map<string, vector<float>> TimeSeries::getMap() const { return mapData; }