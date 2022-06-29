#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using std::vector;
using std::string;
using std::cout;
using std::wcout;

class Equipment;
class Potion;

std::ostream& operator<<(std::ostream& out, Equipment*& eq);
std::ostream& operator<<(std::ostream& out, Potion*& eq);

enum class Color {
	RED,GREEN,WHITE,BLACK,YELLOW,BLUE
};

class IOManager {
private:
	static vector< std::pair<string,int> > _remember;
	static int color_code;
public:

	template <typename Arg, typename... Args>
	static void printText(Arg&& arg, Args&&... args) {
		cout << arg;
		((cout << ' ' << args), ...);
		cout << std::endl;

		std::stringstream ss;
		ss << arg;
		((ss << ' ' << args), ...);
		ss << std::endl;
		_remember.push_back(make_pair(ss.str(), color_code));
	}

	template <typename Arg, typename... Args>
	static void printTextw(Arg&& arg, Args&&... args) {
		wcout << arg;
		((wcout << ' ' << args), ...);
		wcout << std::endl;
	}
	static int chooseFromAction(const vector<string>&,int=0);
	static void clear();
	static void pause();
	static void setColor(Color);
	static void retrieve();
};


#endif