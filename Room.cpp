#include <algorithm>
#include <string>
#include <random>
#include <fstream>
#include <windows.h>

using std::string;

#include "Room.h"

namespace CTW = GlobalVariable::RoomControlWords;

string Room::getClass() const {
	return "Room";
}

Room::Room(string id) :_id(id) {}

string Room::getId() const{
	return _id;
}

void Room::setAdjacentRoom(const vector<RoomPtr>& adj) {
	_roomList = vector<RoomPtr>(adj);
}

void Room::addAdjacentRoom(const RoomPtr& ptr) {
	_roomList.push_back(ptr);
}

RoomPtr Room::getAdjacentRoom(int idx) const {
	if (idx >= 0 && idx < _roomList.size())
		return _roomList[idx];

	return nullptr;
}

vector<RoomPtr> Room::getRoomList() const {
	return _roomList;
}

bool Room::isLocked() const {
	return 0;
}

Inventory Room::getRequiredItem() const {
	return Inventory();
}

bool Room::isTerminated() const {
	return _roomList.empty();
}

bool Map::_loaded = 0;
vector<RoomPtr> Map::_allRoom = vector<RoomPtr>();

void Map::load(string path) {

	if (!MonsterList::isLoaded()) {
		throw "Load Monsters Before Load Rooms.";
		return;
	}

	if (!ItemList::isLoaded()) {
		throw "Load Item Before Load Rooms";
		return;
	}

	std::fstream file;
	file.open(path, std::ios::in);

	if (file.fail()) {
		throw "Unable to open file" + path;
		return;
	}

	int room_num;
	file >> room_num;
	for (int i = 0; i < room_num; i++) {
		string id;
		string type;
		file >> id >> type;

		if (type == "MST") {
			//monster room
			// Format:
			// (num_monster)
			// (monster_id_1) (monster_id_2) ...
			int num_mst;
			file >> num_mst;
			vector<Monster> monst;
			for (int j = 0; j < num_mst; j++) {
				int mst_id;
				file >> mst_id;

				Monster mst(MonsterList::findMonsterById(mst_id));
				monst.push_back(mst);
			}

			RoomPtr roomptr(new BasicMonsterRoom(id, monst));
			_allRoom.push_back(roomptr);
		}
		else if (type == "NOR") {
			//normal room

			RoomPtr roomptr(new NormalRoom(id));
			_allRoom.push_back(roomptr);
		}
		else if (type == "TRS") {
			//treasure room
			//Format
			//(num_item) (percentage_of_mimic) (mimic_id)
			//(item_id_1) (item_id_2) ...

			int num_item, percent, mimic_id;
			file >> num_item >> percent >> mimic_id;
			vector<Itemptr> itemls;
			for (int j = 0; j < num_item; j++) {
				string item_id;
				file >> item_id;
				Itemptr it = ItemList::findItemByName(item_id);
				itemls.push_back(it);
			}

			Monster mimic = MonsterList::findMonsterById(mimic_id);
			RoomPtr roomptr(new TreasureRoom(id,itemls,percent,mimic));
			_allRoom.push_back(roomptr);
		}
		else if (type == "NPC") {
			//NPC room
			//Format
			//(num_required_items) (num_reward_items)
			//(required_item_id_1) (required_item_id_2) ...
			//(reward_item_id_1) (reward_item_id_2)
			
			int num_req, num_rew;
			file >> num_req >> num_rew;
			vector<Itemptr> require_item;
			vector<Itemptr> reward_item;
			for (int j = 0; j < num_req; j++) {
				string id;
				file >> id;
				Itemptr item = ItemList::findItemByName(id);

				require_item.push_back(item);
			}

			for (int j = 0; j < num_rew; j++) {
				string id;
				file >> id;
				Itemptr item = ItemList::findItemByName(id);

				reward_item.push_back(item);
			}

			RoomPtr roomptr(new NPCRoom(id, require_item, reward_item));
			_allRoom.push_back(roomptr);
		}
		else if(type == "LMS"){
			//Locked monster room
			//Format
			// (num_monster) (num_required_item)
			// (monster_id_1) (monster_id_2) ...
			// (required_item_1) (required_item_2) ...

			int num_monster, num_req;
			file >> num_monster >> num_req;
			vector<Monster> mstls;
			vector<Itemptr> itemls;
			for (int j = 0; j < num_monster; j++) {
				int id;
				file >> id;
				Monster mst = MonsterList::findMonsterById(id);
				mstls.push_back(mst);
			}

			for (int j = 0; j < num_req; j++) {
				string id;
				file >> id;
				Itemptr item = ItemList::findItemByName(id);
				itemls.push_back(item);
			}

			RoomPtr roomptr(new LockedMonsterRoom(id, mstls, itemls));
			_allRoom.push_back(roomptr);
		}
	}

	//Format
	//(edge_num)
	// (from_room_id_1) (to_room_id_1)
	// ...

	int edge_num;
	file >> edge_num;

	for (int i = 0; i < edge_num; i++) {
		string idfrom, idto;
		file >> idfrom >> idto;
		RoomPtr from;
		RoomPtr to;
		for (RoomPtr room : _allRoom) {
			if (room->getId() == idfrom) {
				from = room;
			}

			if (room->getId() == idto) {
				to = room;
			}
		}

		from->addAdjacentRoom(to);
	}

	file >> _start_room_id;

	_loaded = 1;
	return;
}

string Map::_start_room_id = "";

RoomPtr Map::getStartRoom() {
	return findRoomById(_start_room_id);
}

string Map::getClass(){
	return "Map";
}

RoomPtr Map::findRoomById(string id){

	if (!_loaded) {
		throw "Load Map Object before using it.";
		return nullptr;
	}

	for (RoomPtr ptr : _allRoom) {
		if (ptr->getId() == id) {
			return ptr;
		}
	}

	return nullptr;
}

void Map::showMap(string id) {
	std::fstream file;
	file.open(GlobalVariable::Path::mapPath + id + ".smp", std::ios::in);
	std::string line;
	while (std::getline(file,line)) {
		IOManager::printText(line);
	}

	file.close();
	return;
}

BasicMonsterRoom::BasicMonsterRoom(string id,const vector<Monster>& mst):Room(id) {
	_monster_list = vector<Monster>(mst);
	cleared = 0;
}

string BasicMonsterRoom::getClass() const {
	return "BasicMonsterRoom";
}

string BasicMonsterRoom::roomEvent(Player& player) {
	if (cleared) {

		IOManager::printText("You back to the room where you just killed all the enemies. Feeling that the monster may appear at any time, you decide to leave soon.");

		return CTW::success;
	}

	IOManager::printText("Monsters appear! Be ready for the bloody battle...");

	while (player.alive() && monsterAlive()) {
		IOManager::clear();
		player.show();
		for (Monster mst : _monster_list) {
			if(mst.alive())
				mst.show();
		}

		bool to_retreat = player.attack_or_retreat();

		if (!to_retreat) {
			player.attack(_monster_list);
			Sleep(500);

			for (Monster m : _monster_list) {
				if (m.alive()) {
					m.attack(player);
					Sleep(500);
				}
			}
			IOManager::pause();
		}
		else {
			return CTW::retreat;
			break;
		}
	}

	if (player.alive()) {
		for (Monster m : _monster_list) {
			player.pick_up_item(m.getDropItem());
		}
		cleared = 1;
		return CTW::success;
	}
	else {
		return CTW::died;
	}
}

bool BasicMonsterRoom::monsterAlive() const {
	bool some_alive = 0;
	for (Monster m : _monster_list)
		some_alive |= m.alive();

	return some_alive;
}

bool BasicMonsterRoom::isCleared()const {
	return cleared;
}

NormalRoom::NormalRoom(string id):Room(id){}

string NormalRoom::getClass() const {
	return "NormalRoom";
}

string NormalRoom::roomEvent(Player& p) {
	//Nothing Happen
	return CTW::success;
}

TreasureRoom::TreasureRoom(string id, const vector<Itemptr>& items,int mimic_percent,Monster mst) :Room(id),_monster_mimic(mst){
	_treasure = Inventory(items);
	_percent = mimic_percent;
	entered = 0;
}

string TreasureRoom::getClass() const {
	return "TreasureRoom";
}

string TreasureRoom::roomEvent(Player& player) {
	if (entered) {
		IOManager::printText("You've been here and taken away all the items.");
		return CTW::success;
	}
	entered = 1;
	IOManager::printText("You found a treasure box!", "Trying to open it...");
	if (RandomGenerator::getRandomNumberFromRange(0, 99) < _percent) {
		//mimic appear

		IOManager::printText("Mimic appear! Get ready for the battle!");

		while (player.alive() && _monster_mimic.alive()) {
			bool to_attack = player.attack_or_retreat();
			if (to_attack) {
				player.attack(_monster_mimic);

				if (_monster_mimic.alive())
					_monster_mimic.attack(player);
			}
			else {
				return CTW::retreat;
			}
		}

		if (player.alive()) {
			player.pick_up_item(_monster_mimic.getDropItem());
			player.pick_up_item(_treasure);
			
			return CTW::success;
		}
		else {
			return CTW::died;
		}
	}
	else {
		IOManager::printText("You get:");
		_treasure.print();
		player.pick_up_item(_treasure);
	}

	return CTW::success;
}

NPCRoom::NPCRoom(string name, const vector<Itemptr>& req, const vector<Itemptr>& rew) :Room(name) {
	_require_item = Inventory(req);
	_reward_item = Inventory(rew);
}

string NPCRoom::getClass() const {
	return "NPCRoom";
}

string NPCRoom::roomEvent(Player& player) {
	//print some greeting words

	IOManager::printText("Welcome, traveler! Do you want to trade something?");
	IOManager::setColor(Color::RED);
	IOManager::printText("REQUIRE:");
	IOManager::setColor(Color::BLUE);
	_require_item.print();
	IOManager::setColor(Color::GREEN);
	IOManager::printText("TO GET:");
	IOManager::setColor(Color::YELLOW);
	_reward_item.print();
	IOManager::setColor(Color::WHITE);

	bool not_trade = player.to_trade_or_to_decline();

	if (!not_trade) {
		auto inventory = player.getInventory();
		bool trade_succ = inventory.contains(_require_item);

		if (trade_succ) {
			//print some happy words
			player.drop_item(_require_item);
			player.pick_up_item(_reward_item);

			return CTW::success;
		}
		else {
			//print disappointment
			IOManager::printText("You disappointed me. You did not have things I want. Now go.");

			return CTW::fail;
		}
	}
	else {
		return CTW::success;
	}
}

LockedMonsterRoom::LockedMonsterRoom(string name, const vector<Monster>& mst, const vector<Itemptr>& req) :BasicMonsterRoom(name, mst) {
	_required_keys = Inventory(req);
}

string LockedMonsterRoom::getClass() const {
	return "LockedMonsterRoom";
}

bool LockedMonsterRoom::isLocked() const {
	return 1;
}

Inventory LockedMonsterRoom::getRequiredItem() const {
	return _required_keys;
}