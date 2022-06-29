#include "Item_extend.h"
#include <fstream>
#include <regex>

using std::fstream;

bool ItemList::_loaded = 0;

string ItemList::getClass() {
	return "ItemList";
}

Itemptr ItemList::findItemByName(string name) {

	if (!_loaded) {
		throw "Load ItemList first before using it.";
		return nullptr;
	}

	for (Itemptr item : _allItem) {
		if (item->getName() == name) {
			return item;
		}
	}

	return nullptr;
}

bool ItemList::isLoaded() {
	return _loaded;
}
/*
bool ItemList::checkValidFile(string path) {
	fstream file;
	file.open(path, std::ios::in);
	int n;
	file >> n;
	if (file.bad())
		return 0;
	char c = file.get();

	std::regex reg("[a-zA-Z0-9_]+\\s+[a-zA-Z0-9_]+\\s+((Eq\\s+[0-9]+\\s+[0-9]+\\s+[0-9]+\\s+((HEAD)|(BODY)|(LEFT_HAND)|(RIGHT_HAND)))|(Po\\s+[0-9]+)|(Sp))");

	for (int i = 0; i < n; i++) {
		string buffer;
		std::getline(file, buffer);

		if (!std::regex_match(buffer, reg)) {
			return 0;
		}
	}

	file.close();
	return 1;
}*/

bool ItemList::load(string path) {
	/*
	if (!checkValidFile(path)) {
		return 0;
	}*/

	fstream file;
	file.open(path, std::ios::in);

	int siz;
	file >> siz;
	for (int i = 0; i < siz; i++) {
		string name, des;
		file >> name >> des;
		string type;
		file >> type;

		//Equipment
		if (type == "Eq") {
			int att, hp, def;
			string part;
			file >> att >> hp >> def >> part;

			Equipment::Part p;
			if (part == "HEAD") p = Equipment::Part::HEAD;
			else if (part == "BODY") p = Equipment::Part::BODY;
			else if (part == "LEFT_HAND") p = Equipment::Part::LEFT_HAND;
			else if (part == "RIGHT_HAND") p = Equipment::Part::RIGHT_HAND;

			Itemptr new_item(new Equipment(name, des, att, hp, def, p));
			_allItem.push_back(new_item);
		}
		else if (type == "Po") {//Potion
			int heal;
			file >> heal;
			Itemptr new_item(new Potion(name, des, heal));
			_allItem.push_back(new_item);
		}
		else if (type == "Sp") {//Special Item
			Itemptr new_item(new SpecialItem(name, des));
			_allItem.push_back(new_item);
		}
	}

	Itemptr head(new Unequip(Equipment::Part::HEAD,"__head__"));
	Itemptr body(new Unequip(Equipment::Part::BODY,"__body__"));
	Itemptr left(new Unequip(Equipment::Part::LEFT_HAND,"__lefthand__"));
	Itemptr right(new Unequip(Equipment::Part::RIGHT_HAND,"__righthand__"));

	_allItem.push_back(head);
	_allItem.push_back(body);
	_allItem.push_back(left);
	_allItem.push_back(right);

	_loaded = 1;
	file.close();

	return 1;
}

void ItemList::setMonsterDropItem(string path) {
	/*
	File format:
	(num_monster)
	(monster_id) (num_item) (item_1) (item_2) (...)
	...
	*/
	fstream file;
	file.open(path, std::ios::in);
	int n;
	file >> n;
	for (int i = 0; i < n; i++) {
		int mst_id, num_item;
		file >> mst_id >> num_item;

		for (int k = 0; k < num_item; k++) {
			string name;
			file >> name;
			Itemptr ptr = findItemByName(name);
			MonsterList::findMonsterById(mst_id).addItem(ptr);
		}
	}

	file.close();
	return;
}

vector<Itemptr> ItemList::_allItem = vector<Itemptr>();

Equipment::Equipment(string name, string des, int att, int health, int def, Part p) :Item(name, des) {
	_attack = att;
	_health = health;
	_defence = def;
	_part = p;
}

int Equipment::getAttack() const {
	return _attack;
}

int Equipment::getHealth() const {
	return _health;
}

int Equipment::getDefence() const {
	return _defence;
}

Equipment::Part Equipment::getPart() const {
	return _part;
}

string Equipment::getClass() const {
	return "Equipment";
}

string Equipment::to_string() const {
	string to;
	to += getName() + " {Equipment} ";
	if (getAttack() > 0) {
		to += " ATK: " + std::to_string(getAttack());
	}

	if (getDefence() > 0) {
		to += " DEF: " + std::to_string(getDefence());
	}

	if (getHealth() > 0) {
		to += " HP: " + std::to_string(getHealth());
	}
	return to;
}

void Equipment::invoke(Player& p) {
	switch (_part) {
	case Part::HEAD:
		if (p._head != nullptr) {
			p._inventory += p._head;
			p._attack -= p._head->getAttack();
			p._defence -= p._head->getDefence();
			p._maxHealth -= p._head->getHealth();
		}
		p._head = Eqptr(this);
		p._inventory -= Itemptr(this);
		break;
	case Part::BODY:
		if (p._body != nullptr) {
			p._inventory += p._body;
			p._attack -= p._body->getAttack();
			p._defence -= p._body->getDefence();
			p._maxHealth -= p._body->getHealth();
		}
		p._body = Eqptr(this);
		p._inventory -= Itemptr(this);
		break;
	case Part::LEFT_HAND:
		if (p._left_hand != nullptr) {
			p._inventory += p._left_hand;
			p._attack -= p._left_hand->getAttack();
			p._defence -= p._left_hand->getDefence();
			p._maxHealth -= p._left_hand->getHealth();
		}
		p._left_hand = Eqptr(this);
		p._inventory -= Itemptr(this);
		break;
	case Part::RIGHT_HAND:
		if (p._right_hand != nullptr) {
			p._inventory += p._right_hand;
			p._attack -= p._right_hand->getAttack();
			p._defence -= p._right_hand->getDefence();
			p._maxHealth -= p._right_hand->getHealth();
		}
		p._right_hand = Eqptr(this);
		p._inventory -= Itemptr(this);
		break;
	}

	p._attack += _attack;
	p._defence += _defence;
	p._maxHealth += _health;
	p._currentHealth += _health;
}

std::ostream& operator<<(std::ostream& out, Equipment eq) {
	out << eq.to_string();
	return out;
}

std::ostream& operator<<(std::ostream& out, Eqptr eq) {
	out << *eq;
	return out;
}

std::ostream& operator<<(std::ostream& out, Equipment*& eq) {
	out << *eq;
	return out;
}

extern const Equipment nullEquipment = Equipment("", "", 0, 0, 0, Equipment::Part::ALL);

Unequip::Unequip(Equipment::Part p,string name):Item(name,"used to unequip") {
	_part = p;
}

string Unequip::getClass() const {
	return "Unequip";
}

void Unequip::invoke(Player& p) {
	switch (_part) {
	case Equipment::Part::HEAD:
		if (p._head != nullptr) {
			p._inventory += p._head;
			p._attack -= p._head->getAttack();
			p._defence -= p._head->getDefence();
			p._maxHealth -= p._head->getHealth();
			p._head = nullptr;
		}
		break;
	case Equipment::Part::BODY:
		if (p._body != nullptr) {
			p._inventory += p._body;
			p._attack -= p._body->getAttack();
			p._defence -= p._body->getDefence();
			p._maxHealth -= p._body->getHealth();
			p._body = nullptr;
		}
		break;
	case Equipment::Part::LEFT_HAND:
		if (p._left_hand != nullptr) {
			p._inventory += p._left_hand;
			p._attack -= p._left_hand->getAttack();
			p._defence -= p._left_hand->getDefence();
			p._maxHealth -= p._left_hand->getHealth();
			p._left_hand = nullptr;
		}
		break;
	case Equipment::Part::RIGHT_HAND:
		if (p._right_hand != nullptr) {
			p._inventory += p._right_hand;
			p._attack -= p._right_hand->getAttack();
			p._defence -= p._right_hand->getDefence();
			p._maxHealth -= p._right_hand->getHealth();
			p._right_hand = nullptr;
		}
		break;
	}
}

Potion::Potion(string name, string des, int h) :Item(name, des), _heal(h) {

}

int Potion::getHeal() const {
	return _heal;
}

string Potion::getClass() const {
	return "Potion";
}

string Potion::to_string() const {
	return getName() + " {Potion} heal:" + std::to_string(getHeal());
}

void Potion::invoke(Player& p) {
	if (p.getCurrentHealth() + _heal >= p.getMaxHealth()) {
		p.setHealth(p.getMaxHealth());
	}
	else {
		p.setHealth(p.getCurrentHealth() + _heal);
	}
	p._inventory -= this;
}

std::ostream& operator<<(std::ostream& out, Potion*& potion) {
	out << potion->to_string();
	return out;
}

std::ostream& operator<<(std::ostream& out, const Potion& potion) {
	out << potion.to_string();
	return out;
}

SpecialItem::SpecialItem(string name, string des) : Item(name, des) {

}

string SpecialItem::getClass()const {
	return "SpecialItem";
}

void SpecialItem::invoke(Player&) {

}

string SpecialItem::to_string() const {
	return getName() + " {Unknown} " + getDescription();
}