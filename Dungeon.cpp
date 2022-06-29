#include "Dungeon.h"

namespace Stats = GlobalVariable::Stats;
namespace Path = GlobalVariable::Path;
namespace CTW = GlobalVariable::RoomControlWords;

string Dungeon::getClass() const {
	return "Dungeon";
}

Dungeon::Dungeon():_player(Player(Stats::playerBasicHealth, Stats::playerBasicAtt, Stats::playerBasicDefence)){
}

void Dungeon::init() {
	MonsterList::load(Path::monsterListPath);
	ItemList::load(Path::itemListPath);
	ItemList::setMonsterDropItem(Path::monsterItemPath);
	Map::load(Path::roomListPath);
	
	_player.pick_up_item(ItemList::findItemByName("__head__"));
	_player.pick_up_item(ItemList::findItemByName("__body__"));
	_player.pick_up_item(ItemList::findItemByName("__lefthand__"));
	_player.pick_up_item(ItemList::findItemByName("__righthand__"));
	current = Map::getStartRoom();
	_history.push_back(current);
}

void Dungeon::runDungeon() {
	while (!current->isTerminated()) {

		_player.show();

		vector<string> room_name;
		auto roomList = current->getRoomList();
		for (RoomPtr ptr : roomList) {
			if(ptr)
				room_name.push_back(ptr->getId());
		}
		room_name.push_back("Show Map");

		int forwardRoom = IOManager::chooseFromAction(room_name,1);

		if (forwardRoom == room_name.size() - 1) {
			IOManager::clear();
			Map::showMap(current->getId());
			IOManager::pause();
			continue;
		}

		if (forwardRoom == room_name.size()) {
			//show backpack
			IOManager::clear();
			_player.show_backpack();
			continue;
		}

		if (roomList[forwardRoom]->isLocked()) {
			auto require_item = roomList[forwardRoom]->getRequiredItem();

			auto inventory = _player.getInventory();

			Inventory need = require_item - inventory;

			if (!need.empty()) {
				IOManager::printText("Room is locked. You need:");
				need.print();
				continue;
			}
		}

		current = roomList[forwardRoom];
		_history.push_back(current);

		IOManager::printText("Enter",current->getId());
		IOManager::pause();

		string result = current->roomEvent(_player);

		if (result == CTW::success) {

		}
		else if (result == CTW::retreat) {
			current = _history[_history.size() - 2];
			continue;
		}
		else if (result == CTW::died) {
			IOManager::setColor(Color::RED);
			IOManager::printText("YOU DIED.");
			IOManager::setColor(Color::WHITE);
			break;
		}
		else if (result == CTW::fail) {

		}
	}
	
	if (_player.alive()) {
		IOManager::setColor(Color::YELLOW);
		IOManager::printText("Success!");
		IOManager::setColor(Color::WHITE);
	}
}
