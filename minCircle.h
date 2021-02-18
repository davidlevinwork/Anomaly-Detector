#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};

float findDistance(const Point& g, const Point& h);
float findDistance(const float x, const float x1, const float y, const float y1);
float calcDet(int flag, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3);
bool insideCircle(const Point& point, const Circle& circle);
bool isValid(const Circle& circle, const std::vector<Point>& points);
Circle createCircle(const Point& p1, const Point& p2);
Circle createCircle(const Point& p1, const Point& p2, const Point& p3);
Circle calcCircle(std::vector<Point>& points);
Circle theAlgorithm(std::vector<Point>& originalVec, std::vector<Point> newVec, int size);
Circle findMinCircle(Point** points, size_t size);
Circle findMinCircle(std::vector<float> &featureOne, std::vector<float> &featureTwo, size_t size);
#endif /* MINCIRCLE_H_ */
