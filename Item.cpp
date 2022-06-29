#include "Item.h"
#include <fstream>
#include <regex>
using std::fstream;


Item::Item(string name, string des):_name(name),_descri(des){}

string Item::getClass() const {
	return "Item";
}

string Item::getName() const {
	return _name;
}

string Item::getDescription() const {
	return _descri;
}

bool Item::operator==(const Item& item) const{
	return this->getName() == item.getName();
}

string Item::to_string() const {
	return _name + " | " + _descri;
}

std::ostream& operator<<(std::ostream& out, const Item& itm) {
	out << itm.to_string();
	return out;
}

std::ostream& operator<<(std::ostream& out, Item&& itm) {
	out << itm.to_string();
	return out;
}

std::ostream& operator<<(std::ostream& out, Itemptr itm) {
	out << itm->to_string();
	return out;
}
/*
void Item::invoke(Player& p) {

}
*/