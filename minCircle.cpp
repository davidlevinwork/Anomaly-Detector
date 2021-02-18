#include "minCircle.h"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <vector>

using namespace std;

/*
 * Function role is to return the distance between two given points (using the known formula).
 */
float findDistance(const Point& g, const Point& h)
{
    return sqrtf(pow(g.x - h.x, 2) + pow(g.y - h.y, 2));
}


/*
 * Function role is to calculate the distance (not exactly for 2 points..).
 */
float findDistance(const float x, const float x1, const float y, const float y1)
{
    return sqrtf(powf(x - x1, 2) + powf(y - y1, 2));
}


/*
 * Function role is to define if a given point is inside the circle.
 */
bool insideCircle(const Point& point, const Circle& circle)
{
    if (findDistance(circle.center, point) <= circle.radius) return true;
    return false;
}


/*
 * Function role is to create a circle by given 2 points .
 */
Circle createCircle(const Point& p1, const Point& p2)
{
    float centerX = (p1.x + p2.x) / 2; // find the x value
    float centerY = (p1.y + p2.y) / 2; // find the y value
    Point center = {centerX, centerY}; // create the point
    auto radius = float(findDistance(p1, p2) / 2); // create the distance

    return {center, radius};
}


/*
 * Function role is determined if the given circle contain the points.
 */
bool isValid(const Circle& circle, const vector<Point>& points)
{
    for(int i = 0; i < points.size(); i++) // iterating the points
    {
        if(!insideCircle(points[i], circle)) return false;
    }
    return true;
}


/*
 * Function role is to solve the determinants of the 3 matrix (3X3):
 * case 0 - calc the determinant of matrix A, built from the 3 points.
 * case 1 - calc the determinant of matrix B, built from multiplying the points and the y's values.
 * case 2 - calc the determinant of matrix B, built from multiplying the points and the x's values.
 */
float calcDet(int flag, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
    float res;
    switch (flag)
    {
        case 0: // calc the determinant of matrix A, built from the 3 points
            res = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
            break;
        case 1: // calc the determinant of matrix B, built from multiplying the points and the y's values
            res = (powf(x1, 2) + powf(y1, 2)) * (y3 - y2) + (powf(x2, 2) + powf(y2, 2)) * (y1 - y3) +
                  (powf(x3, 2) + powf(y3, 2)) * (y2 - y1);
            break;
        case 2: // calc the determinant of matrix B, built from multiplying the points and the x's values
            res = (powf(x1, 2) + powf(y1, 2)) * (x2 - x3) + (powf(x2, 2) + powf(y2, 2)) * (x3 - x1) +
                  (powf(x3, 2) + powf(y3, 2)) * (x1 - x2);
            break;
        default: // default case
            res = 0;
    }
    return res;
}


/*
 * Function role: create a circle when giving 3 points, using the formula: (x-x')^2 + (y-y')^2 -(r^2) = 0.
 * After multiplied out, we will need to calc 4 elements:
 * the determinants of 3 matrix (3X3) with another function, and create the circle.
 */
Circle createCircle(const Point& p1, const Point& p2, const Point& p3)
{
    float p1_x = p1.x, p1_y = p1.y; // data of the first point
    float p2_x = p2.x, p2_y = p2.y; // data of the second point
    float p3_x = p3.x, p3_y = p3.y; // data of the third point

    float a = calcDet(0, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y);
    float b = calcDet(1, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y);
    float c = calcDet(2, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y);

    float x = -b / (2 * a); // x value of the point
    float y = -c / (2 * a); // y value of the point
    float r = findDistance(x, p1_x, y, p1_y); // calc the radius of the circle

    return {Point(x, y), r};
}

/*
 * Function role is to find minimum circle of num of point less or equal to 3.
 * if there is 0 points = return circle equal to none.
 * if there is 1 point - return circle equal only to that point.
 * if there is 2 points - create the minimum circle that included those points (both will be on the circle border).
 * if there is 3 points - create the minimum circle that included those points (2 of them will be on the border of
 * the circle and 1 inside the border).
 */
Circle calcCircle(vector<Point>& points)
{
    if (points.empty()) return Circle(Point(0, 0), 0);
    else if (points.size() == 1) return Circle(points[0], 0);
    else if (points.size() == 2) return createCircle(points[0], points[1]);
    else if (points.size() == 3) {
        for (int i = 0; i <= 2; i++) {
            for (int j = i + 1; j <= 2; j++) {
                Circle tempCircle = createCircle(points[i], points[j]);
                if (isValid(tempCircle, points)) return tempCircle;
            }
        }
        return createCircle(points[0], points[1], points[2]);
    }
}


/*
 * Function role: Choose 3 points and create the circle where they are on the circle boundary. After that the algorithm
 * Get the next point and check if it is inside or on the circle:
 * If it is enclosed - the circle is good for those points, so he keep checking for the other points.
 * If it is not enclosed - the alg' will create new circle contain the checked points.
 */
Circle theAlgorithm(vector<Point>& originalVec, vector<Point> newVec, int size)
{
    // Base case for the recursion
    if (newVec.size() == 3 || size == 0) return calcCircle(newVec);
    // variable declaration:
    // choose random point:
    int random = rand() % (size);
    Point tempPoint = originalVec[random]; // swap between the two points - more efficient
    originalVec[random] = originalVec[size - 1];
    originalVec[size - 1] = tempPoint;
    Circle tempCircle = theAlgorithm(originalVec, newVec, size - 1);
    // end of the recursion, check if the circle suitable for all the points.
    if (insideCircle(tempPoint, tempCircle)) return tempCircle;
    // Otherwise, must be on the boundary of the MEC
    newVec.push_back(tempPoint);
    return theAlgorithm(originalVec, newVec, size - 1);
}

Circle findMinCircle(Point** points,size_t size)
{
    vector<Point> vec;
    for(int i = 0; i < size; i++)
    {
        Point p(points[i]->x, points[i]->y);
        vec.push_back(p);
    }
    vector<Point> R;
    return theAlgorithm(vec, R, size);
}

Circle findMinCircle(vector<float> &featureOne, vector<float> &featureTwo, size_t size)
{
    vector<Point> vec;
    for(int i = 0; i < size; i++)
    {
        Point p(featureOne[i], featureTwo[i]);
        vec.push_back(p);
    }
    vector<Point> R;
    return theAlgorithm(vec, R, size);
}