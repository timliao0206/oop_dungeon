#ifndef ITEM_EXTEND_H
#define ITEM_EXTEND_H

#include "Item.h"
#include "Character.h"

#include <memory>

class Equipment : public Item {
public:
	enum class Part
	{
		HEAD, BODY, LEFT_HAND, RIGHT_HAND, ALL
	};
private:
	int _attack;
	int _health;
	int _defence;
	Part _part;
public:
	Equipment(string, string, int, int, int, Part);

	int getAttack() const;
	int getHealth() const;
	int getDefence() const;
	Part getPart() const;

	void invoke(Player&);

	string getClass() const;

	string to_string() const;
};

std::ostream& operator<<(std::ostream& out, Equipment eq);
std::ostream& operator<<(std::ostream&, Eqptr);
//std::ostream& operator<<(std::ostream& out, const Equipment*& eq);

typedef Equipment* Eqptr;

class Unequip : public Item {
private:
	Equipment::Part _part;
public:
	Unequip(Equipment::Part,string);
	string getClass() const;
	void invoke(Player&);
};

extern const Equipment nullEquipment;

class Potion : public Item {
private:
	int _heal;
public:
	Potion(string, string, int);
	int getHeal() const;

	void invoke(Player&);

	string getClass() const;
	string to_string() const;
};
typedef Potion* Potionptr;

std::ostream& operator<<(std::ostream& out, const Potion& potion);

class SpecialItem : public Item {
private:
public:
	SpecialItem(string, string);

	string getClass() const;
	string to_string() const;

	void invoke(Player&);

};
typedef SpecialItem* Spptr;

class ItemList : public Object {
private:
	static vector<Itemptr> _allItem;
	static bool _loaded;

	//static bool checkValidFile(string);
public:
	static string getClass();
	//ItemList();

	static Itemptr findItemByName(string name);
	static void setMonsterDropItem(string path);

	static bool load(string);
	static bool isLoaded();
};


#endif
