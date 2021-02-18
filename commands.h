#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};

/*
 * StandardIO enables to use input and output in its basic form.
 */
class StandardIO: public DefaultIO {
public:
    StandardIO() {}
    virtual ~StandardIO() {}
    void write(string text) override { cout << text; }
    void write(float f) override { cout << f; }
    void read(float* f) override { cin >> *f; }
    string read() override
    {
        string str;
        cin >> str;
        return str;
    }

};


/*
 * CommandInfo class will hold all the variables and arguments that we will need to use via the class Command.
 * Instead of holding multiple variables, we will bundle everything in this class.
 * Each Command class will hold reference to this class - so the access will be immediate.
 */
class CommandInfo {
    HybridAnomalyDetector* anomaly;
    vector<AnomalyReport> vec;
    int numLines;
public:
    CommandInfo() { anomaly = new HybridAnomalyDetector(); }
    HybridAnomalyDetector* getHybrid() { return anomaly; }
    vector<AnomalyReport> getVector() { return vec; }
    void setVector(vector<AnomalyReport> vector) { vec = std::move(vector); }
    int getNumLines() { return numLines; }
    void setNumLines(int num) { numLines = num; }
};



class Command{
protected:
    DefaultIO* dio;
    CommandInfo* info;
    string description;
    string anomalyTrain = "anomalyTrain.csv";
    string anomalyTest = "anomalyTest.csv";
public:
	Command(DefaultIO* dio, CommandInfo* info, string description):dio(dio), info(info), description(description){ }
	virtual void execute()=0;
    virtual string getDescription() { return description; }
    virtual string getAnomalyTrain() { return anomalyTrain; }
    virtual string getAnomalyTest() { return anomalyTest; }
	virtual ~Command(){}
};

class FirstCommand: public Command {
public:
    FirstCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) { }
    ~FirstCommand(){}
    void execute() override
    {
        ofstream trainCSV("anomalyTrain.csv"); // create the first CSV file
        dio->write("Please upload your local train CSV file.\n");
        string line;
        // read all the lines from the file until "done"
        while(true)
        {
            line = dio->read();
            if(line == "done") break;
            trainCSV << line << endl;
        }
        dio->write("Upload complete.\n");
        trainCSV.close();

        ofstream testCSV("anomalyTest.csv"); // create the second CSV file
        dio->write("Please upload your local test CSV file.\n");
        // read all the lines from the file until "done"
        while(true)
        {
            line = dio->read();
            if(line == "done") break;
            testCSV << line << endl;
        }
        dio->write("Upload complete.\n");
        testCSV.close();
    }
};

class SecondCommand: public Command {
public:
    SecondCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) {}
    ~SecondCommand(){}
    void execute() override
    {
        dio->write("The current correlation threshold is ");
        dio->write(info->getHybrid()->getThreshValue());
        dio->write("\n");
        dio->write("Type a new threshold");
        dio->write("\n");
        string newThreshold;
        while(true)
        {
            newThreshold = dio->read();
            // check if the input from the user is legal
            if((std::stof(newThreshold) >= 0.0f) && (std::stof(newThreshold) <= 1.0f))
            {
                info->getHybrid()->setThreshValue(std::stof(newThreshold));
                break;
            }
            dio->write("please choose a value between 0 and 1");
            dio->write("\n");
        }
    }
};

class ThirdCommand: public Command {
public:
    ThirdCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) {}
    ~ThirdCommand(){}
    void execute() override
    {
        // create two TimeSeries objects for checking and manipulating 2 files
        TimeSeries tsTrain("anomalyTrain.csv");
        TimeSeries tsTest("anomalyTest.csv");
        // activate the function learnNormal for the anomalyTrain.csv file.
        // the access is from Hybrid object - because we used polymorphism
        info->getHybrid()->learnNormal(tsTrain);
        // activate the function detect for the anomalyTest.csv file.
        // initiate the vector with the output from detect function.
        info->setVector(info->getHybrid()->detect(tsTest));
        dio->write("anomaly detection complete.");
        dio->write("\n");

        info->setNumLines(tsTest.getNumOfLines());
    }
};

class FourthCommand: public Command {
public:
    FourthCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) {}
    ~FourthCommand(){}
    void execute() override
    {
        // print to the screen all the outputs we calculate in previous commands
        for(int i = 0; i < info->getVector().size(); ++i)
        {
            dio->write(info->getVector()[i].timeStep);
            dio->write("         ");
            dio->write(info->getVector()[i].description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

class FifthCommand: public Command {
    float TP;
    float FP;
public:
    FifthCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) {}
    ~FifthCommand(){}
    // function role is to arrange the values in the vector that we calculated in detect function and combine them.
    map<int, int> arrangeReport()
    {
        map<int, int> reportedValues;
        for(int i = 0; i < info->getVector().size(); ++i)
        {
            bool helper = false;
            int minValue = INT_MAX, maxValue = INT_MIN;
            // while we find exception with the same description - combine them
            while((i+1 <= info->getVector().size()) &&
            (info->getVector()[i].description == info->getVector()[i+1].description))
            {
                if(minValue > info->getVector()[i].timeStep) minValue = info->getVector()[i].timeStep;
                if(maxValue < info->getVector()[i+1].timeStep) maxValue = info->getVector()[i+1].timeStep;
                i++;
                helper = true;
            }
            // insert to the map the "lonely" value - we didnt combine it with any value
            if(!helper)
                reportedValues.insert(pair<int, int>(info->getVector()[i].timeStep, info->getVector()[i].timeStep));
            // insert to the map the combine value
            else
                reportedValues.insert(pair<int, int>(minValue, maxValue));
        }
        return reportedValues;
    }
    // function role is to calculate the TP value, considering the values in the map that we create earlier.
    float initializeTP(map<string, string> exceptionValues)
    {
        float sum = 0.0;
        map<int, int>::iterator firstItr;
        map<string, string>::iterator secondItr;

        for(secondItr = exceptionValues.begin(); secondItr != exceptionValues.end(); ++secondItr)
        {
            for(int i = 0; i < info->getVector().size(); i++)
            {
                // check if we find match between the values
                if(info->getVector()[i].timeStep >= stoi(secondItr->first) &&
                (info->getVector()[i].timeStep <= stoi(secondItr->second)))
                {
                    sum++;
                    break;
                }
            }
        }
        return sum;
    }
    void execute() override
    {
        map<string, string> exceptionValues; // map to hold the pairs of exceptions values

        dio->write("Please upload your local anomalies file.\n");
        string exceptionFileName;
        string line;
        // read all the lines from the file until "done"
        while(line != "done") // iterating over the txt file
        {
            line = dio->read();
            if(line == "done") break;
            stringstream lineStream(line);
            string s1, s2;
            int comma = line.find(',');

            s1 = line.substr(0, comma);
            s2 = line.substr(comma + 1, line.length() - comma);

            exceptionValues.insert(pair<string, string>(s1, s2));
        }
        // collecting data from the values in the map
        float positiveException = 0.0, negativeException = static_cast<float>(info->getNumLines());
        map<string , string>::iterator itr;

        for(itr = exceptionValues.begin(); itr != exceptionValues.end(); ++itr)
        {
            positiveException++; // each iteration is positive exception
            negativeException -= ((stof(itr->second) - stof(itr->first)) + 1.0f);
        }
        // exceptionReported map holds the combined pairs of the anomaly report
        map<int, int> exceptionReported = arrangeReport();
        // initialize True Positive rate = TP
        TP = initializeTP(exceptionValues);
        FP = exceptionReported.size() - TP;

        auto TPrate = TP / positiveException;
        auto FPrate = FP / negativeException;
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(floorf(1000 * TPrate) / 1000);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(floorf(1000 * FPrate) / 1000);
        dio->write("\n");
    }
};

class SixthCommand: public Command {
public:
    SixthCommand(DefaultIO *dio, CommandInfo *info, string description) : Command(dio, info, description) {}
    ~SixthCommand(){}
    void execute() override { return; }
};



#endif /* COMMANDS_H_ */
