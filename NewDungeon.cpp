#include <iostream>

#include "Character.h"
#include "Item.h"
#include "loader.h"
#include "Object.h"
#include "Room.h"
#include "Dungeon.h"

int main()
{
	Dungeon dungeon;
	try {
		dungeon.init();
		dungeon.runDungeon();
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
}
