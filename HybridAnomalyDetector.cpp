#include "HybridAnomalyDetector.h"

bool HybridAnomalyDetector::isDetection(const TimeSeries &ts, const correlatedFeatures &structCf, int numLine)
{
    if(fabs(structCf.corrlation) > thresholdHighValue)
        return SimpleAnomalyDetector::isDetection(ts, structCf, numLine);
    Point p(ts.getMap().at(structCf.feature1)[numLine], ts.getMap().at(structCf.feature2)[numLine]);
    Circle circle(structCf.p, structCf.threshold);
    return !insideCircle(p, circle);
}


Point HybridAnomalyDetector::getPoint(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson)
{
    if(fabs(maxPearson) > thresholdHighValue)
        return SimpleAnomalyDetector::getPoint(ts, feature1, feature2, maxPearson);
    Circle c = findMinCircle(ts.getMap()[feature1], ts.getMap()[feature2], ts.getNumOfLines());
    return c.center;
}

Line HybridAnomalyDetector::getLine(const TimeSeries &ts, const string &feature1, const string &feature2, float maxPearson)
{
    if(fabs(maxPearson) > thresholdHighValue)
        return SimpleAnomalyDetector::getLine(ts, feature1, feature2, maxPearson);
    return {0,0};
}

bool HybridAnomalyDetector::isCorrelative(float maxPearson)
{
    if((fabs(maxPearson) > thresholdLowValue) || (fabs(maxPearson) > thresholdHighValue)) return true;
    return false;
}

float HybridAnomalyDetector::getThreshold(Line l, vector<float> &featureOne, vector<float> &featureTwo, int numLines, float maxPearson)
{
    if(fabs(maxPearson) > thresholdHighValue)
        return SimpleAnomalyDetector::getThreshold(l, featureOne, featureTwo, numLines, maxPearson);

    Circle c = findMinCircle(featureOne, featureTwo, numLines);
    return c.radius * 1.1f;
}
