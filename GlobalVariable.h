#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include <string>

using std::string;

namespace GlobalVariable {
	namespace Path {
		const string roomListPath = "map/game.mp";
		const string itemListPath = "item/game.imlt";
		const string monsterListPath = "monster/game.mst";
		const string monsterItemPath = "item/game.mstim";
		const string mapPath = "map/show_map/";
	}

	namespace Stats {
		const int playerBasicAtt = 10;
		const int playerBasicHealth = 100;
		const int playerBasicDefence = 0;
	}

	namespace RoomControlWords {
		const string success = "Success";
		const string retreat = "Retreat";
		const string died = "Died";
		const string fail = "Fail";
	}
}


#endif
