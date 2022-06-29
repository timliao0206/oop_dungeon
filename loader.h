#ifndef LOADER_H
#define LOADER_H

#include "Object.h"
//loader for game object
//every class should define its own loader

class Loader {
public:

	static void load(string,Object*);

};

#endif