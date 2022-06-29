#include "IOManager.h"
#include <Windows.h>
using std::cout;
using std::cin;

int IOManager::chooseFromAction(const vector<string>& actions,int show_back_pack) {
	printText("You have following options:");
	for (int i = 0; i < actions.size(); i++) {
		cout << "[" << i << "] " << actions[i];
		if (i != actions.size()-1)
			cout << " / ";
	}

	if (show_back_pack) {
		cout << " / [" << actions.size() << "] Inventory";
	}

	cout << std::endl;
	int action = -1;
	while (true) {
		cout << ">>";
		cin >> action;
		if (action >= 0 && action < actions.size()+show_back_pack) {
			break;
		}

		cout << std::endl << "Invalid input.\n";
	}

	return action;
}

void IOManager::clear() {
	system("cls");
	_remember.clear();
}

void IOManager::pause() {
	cin.ignore(100, '\n');
	cout << "\nPress Enter to continue...";
	char c = getchar();
	clear();
}

void IOManager::setColor(Color color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int k;
	switch (color) {
	case Color::BLACK:
		k = 0;
		break;
	case Color::BLUE:
		k = 9;
		break;
	case Color::GREEN:
		k = 10;
		break;
	case Color::RED:
		k = 12;
		break;
	case Color::WHITE:
		k = 15; 
		break;
	case Color::YELLOW:
		k = 14;
		break;
	}
	color_code = k;
	SetConsoleTextAttribute(hConsole, k);
}

vector< std::pair<string, int> > IOManager::_remember = vector< std::pair<string, int> >();
int IOManager::color_code = 15;

void IOManager::retrieve() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < _remember.size(); i++) {
		SetConsoleTextAttribute(hConsole, _remember[i].second);
		cout << _remember[i].first;
	}
	SetConsoleTextAttribute(hConsole, color_code);
}