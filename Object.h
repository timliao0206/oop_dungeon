#ifndef OBJECT_H
#define OBJECT_H

#include "Random.h"
#include "GlobalVariable.h"
#include "IOManager.h"

/*
Abstract class for every object defined in games
*/

#include <string>
using std::string;

//ABSTRACT CLASS
class Object {
public:
	//get class name
	virtual string getClass()const = 0;
};

#endif // !OBJECT_H