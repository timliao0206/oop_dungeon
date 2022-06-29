#ifndef ROOM_H
#define ROOM_H

#include <memory>
#include <vector>

#include "Object.h"
#include "loader.h"
#include "Character.h"
#include "Item_extend.h"

using std::vector;

class Room;

typedef std::shared_ptr<Room> RoomPtr;

class Room : public Object {
protected:
	vector<RoomPtr> _roomList;
	string _id;
public:
	virtual string getClass() const;

	Room(string);

	string getId() const;

	void setAdjacentRoom(const vector<RoomPtr>&);
	void addAdjacentRoom(const RoomPtr&);
	RoomPtr getAdjacentRoom(int idx) const;
	vector<RoomPtr> getRoomList() const;

	virtual string roomEvent(Player&) = 0;

	virtual bool isLocked() const;
	virtual Inventory getRequiredItem() const;

	virtual bool isTerminated() const;
};

class Map : public Object {
private:
	static vector<RoomPtr> _allRoom;
	static bool _loaded;
	static string _start_room_id;
public:
	static string getClass();
	//Map();

	static void load(string path);

	static RoomPtr findRoomById(string id);
	static RoomPtr getStartRoom();

	static void showMap(string id);
};

class BasicMonsterRoom : public Room {
private:
	vector<Monster> _monster_list;
	bool cleared;

	bool monsterAlive() const;

public:

	BasicMonsterRoom(string,const vector<Monster>&);

	virtual string getClass() const;
	string roomEvent(Player&);

	bool isCleared() const;
};

class NormalRoom : public Room {
private:
public:

	NormalRoom(string);

	string getClass() const;
	string roomEvent(Player&);
};

class TreasureRoom : public Room {
private:
	Inventory _treasure;

	Monster _monster_mimic;
	bool entered;

	int _percent;

public:
	TreasureRoom(string, const vector<Itemptr>&,int,Monster);

	string getClass() const;
	string roomEvent(Player&);
};

class NPCRoom : public Room {
private:
	Inventory _require_item;
	Inventory _reward_item;
public:

	NPCRoom(string,const vector<Itemptr>&,const vector<Itemptr>&);

	string getClass() const;
	string roomEvent(Player&);
};


class LockedMonsterRoom : public BasicMonsterRoom {
private:
	Inventory _required_keys;
public:

	LockedMonsterRoom(string, const vector<Monster>&, const vector<Itemptr>&);

	bool isLocked() const;
	string getClass() const;
	Inventory getRequiredItem() const;
};
#endif // !ROOM_H
