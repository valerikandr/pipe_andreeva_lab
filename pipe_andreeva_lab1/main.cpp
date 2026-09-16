#include <iostream>
#include <string>

#include "windows.h"

using namespace std;

struct Pipe
{
	string name;
	double l;
	int d;
	bool vremonte;
};

struct CS
{
	string name;
	int cehvsego;
	int cehwork;
	int classstation;
};

void menu() {
	cout << "1. Добавить трубу\n"
		<< "2. Добавить КС \n"
		<< "3. Просмотр всех объектов\n"
		<< "4. Редактировать трубу\n"
		<< "5. Редактировать КС\n"
		<< "6. Сохранить\n"
		<< "7. Загрузить\n" 
		<< "0. Выход \n";
};

void Newpipe(Pipe & pipe) {
	cout << "Введите название трубы:";
	cin >> pipe.name;
	cout << "Введите количество цехов";
	cin >> pipe.l;
	cout << "Введите диаметр";
	cin >> pipe.d;
	cout << "В ремонте?";
	cin >> pipe.vremonte;
}

void Newcs(CS & station) {
	cout << "Введите название CS:";
	cin >> station.name;
	cout << "Введите количество цехов";
	cin >> station.cehvsego;
	cout << "Введите количество цехов в работе";
	cin >> station.cehwork;
	cout << "В ремонте? Введите 1 - да, 0 - нет"; 
	cin >> station.classstation;
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	menu();
	return 0;
}

