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

void Menu() {
	cout << "1. Добавить трубу\n"
		<< "2. Добавить КС \n"
		<< "3. Просмотр всех объектов\n"
		<< "4. Редактировать трубу\n"
		<< "5. Редактировать КС\n"
		<< "6. Сохранить\n"
		<< "7. Загрузить\n" 
		<< "0. Выход \n";
};

//защита от ввода букв
int Tokachislo() {
	int value;

	cin >> value;

	while (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');

		cout << "Ошибка! Напишите число: ";
		cin >> value;
	}

	return value;
}

void Newpipe(Pipe & pipe) {
	cout << "Введите название трубы: ";
	getline(cin >> ws, pipe.name);
	cout << "Введите длину:  ";
	pipe.l = Tokachislo();
	cout << "Введите диаметр: ";
	pipe.d = Tokachislo();
	cout << "В ремонте? Введите 1 - да, 0 - нет ";
	pipe.vremonte = Tokachislo();
}

void Newcs(CS & station) {
	cout << "Введите название КС: ";
	getline(cin >> ws, station.name);
	cout << "Введите количество цехов: ";
	station.cehvsego = Tokachislo();
	cout << "Введите количество цехов в работе: ";
	station.cehwork = Tokachislo();
	cout << "Какой класс станции? (Воздушные - 1, Газовые - 2)  ";
	station.classstation = Tokachislo();
}

void Vseobjects(const Pipe& pipe, const CS& station,
	bool hasPipe, bool hasStation) {
	if (hasPipe) {
		cout << "Название трубы: " << pipe.name;
		cout << "\n Длина: " << pipe.l;
		cout << "\n Диаметр:  " << pipe.d;
		cout << "\n В ремонте (Да - 1, Нет - 0): " << pipe.vremonte << "\n";
	}

	else
	{
		cout << "Труба ещё не создана \n";
	}
	if (hasStation)
	{
		cout << "Название КС: " << station.name;
		cout << "Всего цехов: " << station.cehvsego;
		cout << "Сколько в работе:  " << station.cehwork;
		cout << "Класс станции: (Воздушные - 1, Газовые - 2) " << station.classstation << "\n";
	}
	else
	{
		cout << "КС ещё не создана \n";
	}
}

void EditPipe(Pipe& pipe, bool hasPipe) {
	if (!hasPipe) {
		cout << "Труба ещё не задана \n";
		return;
	}
	cout << "Сейчас в ремонте: " << pipe.vremonte << "\n";
	cout << "Новое значение (Да - 1, Нет - 0): ";
	pipe.vremonte = Tokachislo();
}

void EditCS(CS& station, bool hasStation) {
	if (!hasStation) {
		cout << "КС ещё не задана\n";
		return;
	}

	cout << "1 - Запустить цех, 2 - Остановить цех: ";
	int action = Tokachislo();

	if (action == 1) {
		if (station.cehwork >= station.cehvsego)
			cout << "Все цеха уже запущены \n";
		else {
			station.cehwork++;
			cout << "Цех запущен. В работе: " << station.cehwork << "\n";
		}
	}
	else if (action == 2) 
	{
		if (station.cehwork <= 0)
			cout << "Нет работающих цехов \n";
		else {
			station.cehwork--;
			cout << "Цех остановлен. В работе: " << station.cehwork << "\n";
		}
	}
	else {
		cout << "Нет такого действия  \n";
	}
}

 
int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	Pipe pipe;
	CS station;

	bool hasPipe = false;
	bool hasStation = false;

	int  number;

	while (true) {
		Menu();
		cout << "Введите команду: ";
		cin >> number;

		switch (number) {

			//добавить трубу
		case 1:
			Newpipe(pipe);
			hasPipe = true;
			break;

			//добавить кс
		case 2:
			Newcs(station);
			hasStation = true;
			break;

			//просмотр всех объектов
		case 3:
			Vseobjects(pipe, station, hasPipe, hasStation);
			break;

			//редактировать трубу
		case 4:
			EditPipe(pipe, hasPipe);
			break;

			//редактировать кс
		case 5:
			EditCS(station, hasStation);
			break;
		}
	}
}