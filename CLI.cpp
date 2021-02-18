#include "CLI.h"

CLI::CLI(DefaultIO* dio)
{
    this->dio = dio;
    this->info = new CommandInfo();

    Command* first_Command = new FirstCommand(dio, info, "1.upload a time series csv file\n");
    Command* second_Command = new SecondCommand(dio, info, "2.algorithm settings\n");
    Command* third_Command = new ThirdCommand(dio, info, "3.detect anomalies\n");
    Command* fourth_Command = new FourthCommand(dio, info, "4.display results\n");
    Command* fifth_Command = new FifthCommand(dio, info, "5.upload anomalies and analyze results\n");
    Command* sixth_Command = new SixthCommand(dio, info, "6.exit\n");

    menu.insert(pair<int, Command*>(1, first_Command));
    menu.insert(pair<int, Command*>(2, second_Command));
    menu.insert(pair<int, Command*>(3, third_Command));
    menu.insert(pair<int, Command*>(4, fourth_Command));
    menu.insert(pair<int, Command*>(5, fifth_Command));
    menu.insert(pair<int, Command*>(6, sixth_Command));
}

void CLI::start()
{
    while(true)
    {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for(itr = menu.begin(); itr != menu.end(); ++itr)
        {
            dio->write(itr->second->getDescription());
        }
        string userChoice = dio->read();
        bool valid = true;
        for(int i = 0; i < userChoice.size(); i++)
        {
            if(!isdigit(userChoice[i])) valid = false;
        }
        if(!valid) continue;
        menu.at(std::stoi(userChoice))->execute();
        if(std::stoi(userChoice) == 6) break;
    }
}

CLI::~CLI()
{
    for(itr = menu.begin(); itr != menu.end(); ++itr)
    {
        delete itr->second;
    }
}

