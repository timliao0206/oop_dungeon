#include "Character.h"
#include <fstream>


Character::Character(int h,int a,int d):_maxHealth(h),_attack(a),_defence(d),_currentHealth(h){}

string Character::getClass() const {
	return "Character";
}

bool Character::alive() const {
	return _currentHealth > 0;
}

int Character::getAttack() const {
	return _attack;
}

int Character::getMaxHealth() const {
	return _maxHealth;
}

int Character::getDefence() const {
	return _defence;
}

int Character::getCurrentHealth() const {
	return _currentHealth;
}

void Character::takeDamage(int dmg) {
	if(dmg >= 0)
		_currentHealth -= dmg;
}

void Character::setHealth(int h) {
	_currentHealth = h;
}

Player::Player(int h, int a, int d) :Character(h, a, d), _head(nullptr), 
	_body(nullptr), _left_hand(nullptr),_right_hand(nullptr)
{
	_inventory = Inventory();
}

string Player::getClass() const {
	return "Player";
}

Inventory Player::getInventory() const {
	return _inventory;
}

/*
Equipment Player::getEquippedItem(Equipment::Part p) {
	switch (p) {
	case Equipment::Part::HEAD:
		return _head;
		break;
	case Equipment::Part::BODY:
		return _body;
		break;
	case Equipment::Part::LEFT_HAND:
		return _left_hand;
		break;
	case Equipment::Part::RIGHT_HAND:
		return _right_hand;
		break;
	}

	return nullEquipment;
}
*/
/*
void Player::equip(Equipment eq) {
	Equipment* replaced;
	if (eq.getPart() == Equipment::Part::HEAD) {
		replaced = &_head;
	}else if (eq.getPart() == Equipment::Part::BODY) {
		replaced = &_body;
	}
	else if (eq.getPart() == Equipment::Part::LEFT_HAND) {
		replaced = &_left_hand;
	}
	else if (eq.getPart() == Equipment::Part::RIGHT_HAND) {
		replaced = &_right_hand;
	}
	else {
		return;
	}

	_maxHealth += eq.getHealth() - replaced->getHealth();
	_currentHealth += eq.getHealth() - replaced->getHealth();
	_attack += eq.getAttack() - replaced->getAttack();
	_defence += eq.getDefence() - replaced->getDefence();

	*replaced = eq;

	return;
}
*/

bool Player::attack_or_retreat() const {

	bool action = IOManager::chooseFromAction(vector<string>{"Attack", "Retreat"});

	return action;
}

bool Player::to_trade_or_to_decline() const {
	bool action = IOManager::chooseFromAction(vector<string>{"Trade", "Decline"});
	return action;
}

void Player::pick_up_item(Itemptr item) {
	_inventory += item;
}

void Player::pick_up_item(const Inventory& item) {
	_inventory += item;
}

void Player::drop_item(const Inventory& item) {
	_inventory = _inventory - item;
}

void Player::attack(vector<Monster>& monster_list) {
	if (!monster_list.empty()) {
		int idx = 0;
		for (; !monster_list[idx].alive(); idx++) {}
		int damage = _attack - monster_list[idx].getDefence();
		monster_list[idx].takeDamage(damage);
		IOManager::printText("Player attacked", monster_list[idx].getName(), ",", (damage >= 0 ? damage : 0), "damages dealt.");
	}
}

void Player::attack(Monster& mst) {
	int damage = _attack - mst.getDefence();
	mst.takeDamage(_attack - mst.getDefence());
	IOManager::printText("Player attacked", mst.getName(), ",", (damage >= 0 ? damage : 0), "damages dealt.");
}

void Player::show() const {
	IOManager::printText("--------------------------STATUS--------------------------");
	IOManager::printText("Health:", _currentHealth, "/", _maxHealth);
	IOManager::printText("Attack:",_attack);
	IOManager::printText("Defence:", _defence);
}


void Player::show_backpack(){
	IOManager::printText("--------------------------EQUIPMENT--------------------------");
	IOManager::setColor(Color::YELLOW);
	if (_head == nullptr)
		IOManager::printText("Head: [Empty]");
	else
		IOManager::printText("Head:",_head);
	
	if (_body == nullptr)
		IOManager::printText("Body: [Empty]");
	else
		IOManager::printText("Body:",_body);

	if (_left_hand == nullptr)
		IOManager::printText("Left Hand: [Empty]");
	else
		IOManager::printText("Left Hand:", _left_hand);
	
	if(_right_hand == nullptr)
		IOManager::printText("Right Hand: [Empty]");
	else
		IOManager::printText("Right Hand:",_right_hand);

	IOManager::setColor(Color::WHITE);

	IOManager::printText("--------------------------INVENTORY--------------------------");
	_inventory.print(1);

	vector<string> action{"Use Potion","Equip Item","Unequip Item"};
	int act = IOManager::chooseFromAction(action);
	if (act == 0) {
		//Use potion
		Inventory potions = _inventory.filter([](Itemptr it) {return it->getClass() == "Potion"; });
		IOManager::printText("Potion list:");
		potions.print(1);
		vector<string> act;
		act.resize(potions.size(), "");
		act.push_back("Exit");
		int action = IOManager::chooseFromAction(act);
		if (action != potions.size()) {
			potions[action]->invoke(*this);
		}
	}
	else if(act == 1){
		//Equip item
		Inventory eqs = _inventory.filter([](Itemptr it) {return it->getClass() == "Equipment"; });
		IOManager::printText("Equipment list:");
		eqs.print(1);

		vector<string> act;
		act.resize(eqs.size(), "");
		act.push_back("Exit");
		int action = IOManager::chooseFromAction(act);
		if (action != eqs.size()) {
			eqs[action]->invoke(*this);
		}
	}
	else {
		//Unequip Item
		vector<string> actions;

		if (_head != nullptr)
			actions.push_back("head");
		if (_body != nullptr)
			actions.push_back("body");
		if (_left_hand != nullptr)
			actions.push_back("left hand");
		if (_right_hand != nullptr)
			actions.push_back("right hand");

		actions.push_back("Exit");
		int act = IOManager::chooseFromAction(actions);
		
		if (act != actions.size() -1) {
			int idx = 0;
			if (actions[act] == "head") idx = 0;
			else if (actions[act] == "body") idx = 1;
			else if (actions[act] == "left hand") idx = 2;
			else if (actions[act] == "right hand") idx = 3;
			Inventory uneq = _inventory.filter([](Itemptr p) {return p->getClass() == "Unequip"; });
			uneq[idx]->invoke(*this);
		}
	}

	return;
}


Monster::Monster(int h, int a, int d,string name,int id) :Character(h, a, d) {
	_dropItem = Inventory();
	_name = name;
	_id = id;
}

string Monster::getClass() const {
	return "Monster";
}

int Monster::getId()const {
	return _id;
}

string Monster::getName() const {
	return _name;
}

void Monster::attack(Player& player) {
	int damage = _attack - player.getDefence();
	player.takeDamage(damage);
	IOManager::printText(_name, " attack player,", (damage >= 0 ? damage : 0), "damages dealt.");
}

Inventory Monster::getDropItem() const {
	return _dropItem;
}

void Monster::show() const {
	IOManager::printText("----------------------------------------------------------");
	IOManager::setColor(Color::RED);
	IOManager::printText(_name);
	IOManager::setColor(Color::WHITE);
	IOManager::printText("Health:", _currentHealth, "/", _maxHealth);
	IOManager::printText("Attack:", _attack);
	IOManager::printText("Defence:", _defence);
}

void Monster::addItem(Itemptr ptr) {
	_dropItem += ptr;
}
/*
MonsterList::MonsterList(){
	_loaded = 0;
}
*/

bool MonsterList::_loaded = 0;
vector<Monster> MonsterList::_monsterList = vector<Monster>();

Monster& MonsterList::findMonsterById(int id) {
	for (Monster& mst : _monsterList) {
		if (mst.getId() == id) {
			return mst;
		}
	}
}

bool MonsterList::isLoaded() {
	return _loaded;
}

void MonsterList::load(string path) {
	// Format
	// (num_monster)
	// (mst_1_id) (mst_1_health) (mst_1_attack) (mst_1_defence) (mst_1_name)
	// ...

	std::fstream file;
	file.open(path, std::ios::in);

	int num_mst;
	file >> num_mst;

	for (int i = 0; i < num_mst; i++) {
		int id, h, a, d;
		string name;
		file >> id >> h >> a >> d >> name;

		_monsterList.push_back(Monster(h, a, d, name, id));
	}

	_loaded = 1;
	file.close();
	return;
}