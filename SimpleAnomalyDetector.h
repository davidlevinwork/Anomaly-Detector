#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation; // value of correlation
	Line lin_reg; // make the line with them
	float threshold; // 1.1
	Point p;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    float thresholdHighValue = 0.9;
    vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	virtual bool isCorrelative(float maxPearson);
	virtual float getThreshold(Line l, vector<float> &featureOne, vector<float> &featureTwo, int numLines, float maxPearson);
    virtual Line getLine(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson);
    virtual Point getPoint(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson);
    virtual bool isDetection(const TimeSeries& ts, const correlatedFeatures& structCf, int numLine);

    virtual void setThreshValue(float newThreshold) { thresholdHighValue = newThreshold; }
    virtual float getThreshValue() const { return thresholdHighValue; }
	vector<correlatedFeatures> getNormalModel(){ return cf; }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
