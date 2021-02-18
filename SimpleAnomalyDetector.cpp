#include <iostream>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() { }

SimpleAnomalyDetector::~SimpleAnomalyDetector() { }

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts)
{
    vector<string> vec = ts.getFeatures();

	for(int i = 0; i < ts.getNumOfFeatures(); i++)
    {
        float temp, max = 0;
        string h_i, h_j; // save the features that correlated
	    for(int j = i+1; j < ts.getNumOfFeatures(); j++)
        {
	        temp = pearson(ts.getMap().at(vec[i]).data(), ts.getMap().at(vec[j]).data(), ts.getNumOfLines());
	        if(fabs(temp) > fabs(max))
	        {
	            h_i = vec[i]; // update the first feature
	            h_j = vec[j]; // update the second feature
	            max = temp; // update the correlation
	        }
        }
	    if(!isCorrelative(max)) continue;
	    // create the line regression from the two features
        Line line = getLine(ts, h_i, h_j, max);
        // calculate the max threshold of the given features
        float threshold = getThreshold(line, ts.getMap()[h_i], ts.getMap()[h_j], ts.getNumOfLines(), max);
        Point p = getPoint(ts, h_i, h_j, max);
        correlatedFeatures c{h_i, h_j, max, line, threshold, p}; // creating the new struct
        cf.push_back(c);
    }
}


Line SimpleAnomalyDetector::getLine(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson)
{
    Line line = linear_reg(ts.getMap()[feature1].data(), ts.getMap()[feature2].data(), ts.getNumOfLines());
    return line;
}

Point SimpleAnomalyDetector::getPoint(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson)
{
    return {0, 0};
}


/*
 * Function role: the function get as arguments 4 args: 2 float vectors that represents the values of each feature.
 * The 2 other arguments, are line (represents the lin_reg of the 2 features) and number of lines in each feature.
 * We will create a point (x = value[i] of feature 1, y = value[i] of feature 2) and will iterate all over the
 * values of features, and find the max dev that we can hold. Multiply it by 1.1 and return the value.
 */
float SimpleAnomalyDetector::getThreshold(Line l, vector<float> &featureOne, vector<float> &featureTwo, int numLines, float maxPearson)
{
    float maxDev = 0;
    for(int i = 0; i < numLines; i++) // iterate over all the values in both of the vectors
    {
        Point p(featureOne[i], featureTwo[i]); // creating the point
        float temp = dev(p, l); // calculating the dev of the specific point
        if(temp > maxDev) maxDev = temp; // update the biggest value
    }
    return maxDev * 1.1f;
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts)
{
    vector<string> vec = ts.getFeatures();
    vector<AnomalyReport> result;
    for (int k = 0; k < cf.size(); k++)
    {
        for (int i = 0; i < ts.getNumOfLines(); i++) // iterating over the vector of correlated features
        {
            if(isDetection(ts, cf[k], i)) // if we have detection
            {
                AnomalyReport report(cf[k].feature1 + "-" + cf[k].feature2, i + 1);
                result.push_back(report);
            }
        }
    }
    return result;
}


bool SimpleAnomalyDetector:: isDetection(const TimeSeries& ts, const correlatedFeatures& structCf, int numLine)
{
    float x = ts.getMap().at(structCf.feature1)[numLine]; // creating the x value
    float y = ts.getMap().at(structCf.feature2)[numLine]; // creating the y value
    Point p(x, y);

    if(structCf.threshold < dev(p, structCf.lin_reg)) return true;
    return false;
}


bool SimpleAnomalyDetector::isCorrelative(float maxPearson)
{
    if(fabs(maxPearson) > thresholdHighValue) return true;
    return false;
}