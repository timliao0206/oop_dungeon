#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>

#include "Item.h"

using std::vector;
using std::pair;
using std::make_pair;

typedef Item* T;

class Backpack {
private:
	vector< pair<T,int> > _list;

	int find_idx(const T& t) {
		for (int i = 0; i < _list.size(); i++) {
			if (_list[i].first == t) {
				return i;
			}
		}
		return _list.size();
	}
public:
	Backpack() { _list = vector< pair<T, int> >(); };
	Backpack(const vector<T>& vec) {
		_list = vector< pair<T, int> >();
		for (T t : vec) {
			bool found = 0;
			for (pair<T, int> item : _list) {
				if (item.first == t) {
					item.second++;
					found = 1;
					break;
				}
			}
			if (!found)
				_list.push_back(make_pair(t, 1));
			
		}
	}

	const int& operator[](const T& item) const{
		for (pair<T, int> p : _list) {
			if (p.first == item) {
				return p.second;
			}
		}
		return 0;
	}

	T operator[](const int& x) const {
		return _list[x].first;
	}

	Backpack& operator+=(const T& item) {
		for (pair<T, int>& p : _list) {
			if (p.first == item) {
				p.second++;
				return *this;
			}
		}
		_list.push_back(make_pair(item, 1));
		return *this;
	}

	Backpack& operator+=(const pair<T,int>& item) {
		for (pair<T, int>& p : _list) {
			if (p.first == item.first) {
				p.second += item.second;
				return *this;
			}
		}
		_list.push_back(make_pair(item.first, item.second));
		return *this;
	}

	Backpack& operator+=(const Backpack& bp) {
		for (pair<T, int> p1 : bp._list) {
			bool found = 0;
			for (pair<T, int>& p2 : _list) {
				if (p1.first == p2.first) {
					p2.second += p1.second;
					found = 1;
					break;
				}
			}
			if (!found) {
				_list.push_back(make_pair(p1.first, p1.second));
			}
			
		}

		return *this;
	}

	Backpack operator-(const Backpack& sub) {
		Backpack b;
		for (pair<T, int> p : _list) {
			if (p.second - sub[p.first] > 0) {
				b += make_pair(p.first, p.second - sub[p.first]);
			}
		}
		return b;
	}

	void operator-=(const T& ptr) {
		for (pair<T, int>& p : _list) {
			if (p.first == ptr) {
				p.second--;
				return;
			}
		}
	}

	bool contains(const Backpack& check) {
		for (pair<T, int> p : check._list) {
			int idx = find_idx(p.first);
			if (idx == _list.size() || _list[idx].second < p.second)
				return 0;
		}
		return 1;
	}

	bool empty()const {
		for (pair<T, int> p : _list) {
			if (p.second > 0) {
				return 0;
			}
		}
		return 1;
	}

	void print(bool with_index = 0)const {
		if (with_index == 0) {
			for (pair<T, int> p : _list) {
				if (p.first->getClass() == "Unequip")
					continue;
				if (p.second > 1)
					IOManager::printText(p.second, "x", *p.first);
				else if(p.second == 1)
					IOManager::printText(*p.first);
			}
		}
		else {
			int cnt = 0;
			for (pair<T, int> p : _list) {
				if (p.first->getClass() == "Unequip")
					continue;
				if (p.second > 1)
					IOManager::printText("[", cnt, "]", p.second, "x", *p.first);
				else if (p.second == 1)
					IOManager::printText("[", cnt, "]", *p.first);
				else
					cnt--;
				cnt++;
			}
		}
	}

	
	typedef bool b_func(T);
	Backpack filter(b_func f) const{
		Backpack b;
		for (pair<T, int> p : _list) {
			if (f(p.first) && p.second)
				b += p;
		}

		return b;
	}

	int size() const {
		return _list.size();
	}
};

#endif
