#ifndef CLI_H_
#define CLI_H_

#include <cstring>
#include <iterator>
#include <map>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	CommandInfo* info;
	map<int, Command*> menu;
    map<int, Command*>::iterator itr;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
