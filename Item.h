#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <iostream>
#include <memory>

#include "Object.h"

using std::vector;
/*
Format of the item file(.imlt)
a line of with a single number (the num of items)
n lines of
(name) (description) ((Eq (attack) (health) (defence) (HEAD/BODY/LEFT_HAND/RIGHT_HAND)) / Po (heal) / Sp)
*/
class Player;
class Item : public Object{
private:
	string _name;
	string _descri;

public:
	Item(string, string);

	string getName() const;
	string getDescription() const;
	virtual string getClass() const;

	bool operator==(const Item&) const;
	virtual void invoke(Player&) = 0;
	virtual string to_string() const;
	
};

typedef Item* Itemptr;


std::ostream& operator<<(std::ostream&,const Item&);
std::ostream& operator<<(std::ostream& out, Item&& itm);
std::ostream& operator<<(std::ostream& out, Itemptr itm);


#endif // !ITEM_H