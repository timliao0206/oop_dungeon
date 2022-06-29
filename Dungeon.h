#ifndef DUNGEON_H
#define DUNGEON_H

#include "Object.h"
#include "Character.h"
#include "Room.h"

class Dungeon : public Object {
private:

	Player _player;

	vector<RoomPtr> _history;
	RoomPtr current;

public:
	Dungeon();
	string getClass()const;
	void init();

	void runDungeon();


};


#endif