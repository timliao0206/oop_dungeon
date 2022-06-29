#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Object.h"
#include "Item.h"
#include "Backpack.h"

class Equipment;
class Potion;
class SpecialItem;
class Unequip;

typedef Equipment* Eqptr;
typedef Potion* Poptr;
typedef SpecialItem* Spptr;

typedef Item* Itemptr;

using std::vector;
typedef Backpack Inventory;

class Character : public Object {
protected:
	int _maxHealth;
	int _currentHealth;
	int _attack;
	int _defence;
public:
	Character(int,int,int);
	virtual string getClass() const;

	bool alive() const;

	int getMaxHealth() const;
	int getCurrentHealth() const;
	int getAttack() const;
	int getDefence() const;

	void setHealth(int);

	void takeDamage(int);

	virtual void show() const = 0;

};

class Monster;

class Player : public Character {
private:
	Inventory _inventory;

	Eqptr _head;
	Eqptr _body;
	Eqptr _left_hand;
	Eqptr _right_hand;

public:

	Player(int, int, int);
	string getClass() const;

	Inventory getInventory() const;

	void pick_up_item(Itemptr);
	void pick_up_item(const Inventory&);
	void drop_item(const Inventory&);

	bool attack_or_retreat() const;
	bool to_trade_or_to_decline() const;

	void attack(vector<Monster>&);
	void attack(Monster&);

	
	void show_backpack();
	void show() const;

	friend Equipment;
	friend Unequip;
	friend Potion;
};

class Monster : public Character {
private:
	Inventory _dropItem;
	string _name;
	int _id;
public:
	Monster(int, int, int,string,int);
	string getClass() const;
	string getName() const;
	
	Inventory getDropItem() const;

	void attack(Player&);

	int getId()const;

	void show() const;

	void addItem(Itemptr);
};

class MonsterList : public Object{
private:
	static vector<Monster> _monsterList;
	static bool _loaded;
public:
	//MonsterList();
	static void load(string);
	static Monster& findMonsterById(int id);
	static bool isLoaded();
};
#endif // !CHARACTER_H

