#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector: public SimpleAnomalyDetector {
protected:
    float thresholdLowValue = 0.5;
public:
	HybridAnomalyDetector() {};
	virtual ~HybridAnomalyDetector() {};

	virtual bool isDetection(const TimeSeries &ts, const correlatedFeatures &structCf, int numLine) override;
    virtual Line getLine(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson) override;
    virtual Point getPoint(const TimeSeries& ts, const string& feature1, const string& feature2, float maxPearson) override;
    virtual float getThreshold(Line l, vector<float> &featureOne, vector<float> &featureTwo, int numLines, float maxPearson) override;
    virtual bool isCorrelative(float maxPearson) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
