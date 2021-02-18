cmake_minimum_required(VERSION 3.17)
project(Step1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Step1 main.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.h CLI.cpp CLI.h)