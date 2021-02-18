#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <map>
using namespace std;

class TimeSeries{
    int numOfLines = 0;
    int numOfFeatures = 0;
    vector<string> features;
    map<string, vector<float>> mapData;
    void loadFile(const char* CSVfileName);
public:
    explicit TimeSeries(const char* CSVfileName) { loadFile(CSVfileName); }
    virtual ~TimeSeries(){} // virtual destructor

    map<string, vector<float>> getMap() const; // return the map
    vector<string> getFeatures() const; // return the features
    int getNumOfLines() const; // return the number of lines (without the first line of features)
    int getNumOfFeatures() const; // return the number of features
    void copyVector(vector<float> vec); // helper function to load the data from CSV file
};



#endif /* TIMESERIES_H_ */
