#include "anomaly_detection_util.h"
#include <cmath>
#include <iostream>
using namespace std;

float calcAverage(float* x, int size)
{
    if(size == 0)
    {
        return 0.0f;
    }

    float expectation = 0.0f;
    for(int i = 0; i < size; i++)
    {
        expectation += x[i];
    }
    return ((float) 1 / size) * expectation;
}

float var(float *x, int size)
{
    if(size == 0)
    {
        return 0.0f;
    }

    float sum_x = 0.0f;
    float average_x = calcAverage(x, size);

    for(int i = 0; i < size; i++)
    {
        sum_x += pow(x[i], 2);
    }
    sum_x *= ((float) 1 / size);

    return sum_x - pow(average_x, 2);
}

float cov(float *x, float *y, int size)
{
    if(size == 0)
    {
        return 0.0f;
    }

    float sum_xy = 0.0f;
    float average_x = calcAverage(x, size);
    float average_y = calcAverage(y, size);

    for(int i = 0; i < size; i++)
    {
        sum_xy += x[i] * y[i];
    }

    return (((float) 1 / size) * sum_xy) - (average_x * average_y);
}

float pearson(float *x, float *y, int size)
{
    float cov_xy = cov(x, y, size);
    float sum_x = var(x, size);
    float sum_y = var(y, size);

    if(size == 0 || sum_y == 0 || sum_x == 0)
    {
        return 0.0f;
    }

    return (cov_xy / (sqrt(sum_x) * sqrt(sum_y)));
}

Line linear_reg(float* x, float* y, int size)
{
    if(size == 0)
    {
        Line* line = nullptr;
        return *line;
    }
    float average_x = calcAverage(x, size);
    float average_y = calcAverage(y, size);

    float a = cov(x, y, size) / var(x, size);
    float b = average_y - (a * average_x);

    Line line = Line(a, b);
    return line;
}

Line linear_reg(Point** points, int size)
{
    if(size == 0)
    {
        Line* line = nullptr;
        return *line;
    }
    float x_points[size], y_points[size];

    for(int i = 0; i < size; i++)
    {
        x_points[i] = points[i]->x;
        y_points[i] = points[i]->y;
    }
    return linear_reg(x_points, y_points, size);
}

float dev(Point p, Point** points, int size)
{
    if(size == 0)
    {
        return 0.0f;
    }

    Line line = linear_reg(points, size);
    return dev(p, line);
}

float dev(Point p, Line l)
{
    float line_y = l.f(p.x);
    return abs(p.y - line_y);
}
