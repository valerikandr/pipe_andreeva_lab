#include <iostream>
#include <string>
#include <fstream>

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
}

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

int Diapozon(int min, int max) {
	int value = Tokachislo();

	while (value < min || value > max) {
		cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
		value = Tokachislo();
	}

	return value;
}

void Newpipe(Pipe& pipe) {
	cout << "Введите название трубы: ";
	getline(cin >> ws, pipe.name);
	cout << "Введите длину:  ";
	pipe.l = Diapozon(1,1000000);
	cout << "Введите диаметр: ";
	pipe.d = Diapozon(1,1000000);
	cout << "В ремонте? Введите 1 - да, 0 - нет ";
	pipe.vremonte = Diapozon(0,1);
}

void Newcs(CS& station) {
	cout << "Введите название КС: ";
	getline(cin >> ws, station.name);
	cout << "Введите количество цехов: ";
	station.cehvsego = Diapozon(1,1000000);
	cout << "Введите количество цехов в работе: ";
	station.cehwork = Diapozon(0,station.cehvsego);
	cout << "Какой класс станции? (Воздушные - 1, Газовые - 2)  ";
	station.classstation = Diapozon(1,2);
}

void PrintPipe(const Pipe& pipe) {
	cout << "Название трубы: " << pipe.name;
	cout << "\n Длина: " << pipe.l;
	cout << "\n Диаметр:  " << pipe.d;
	cout << "\n В ремонте (Да - 1, Нет - 0): " << pipe.vremonte << "\n";
}

void PrintCS(const CS& station) {
	cout << "Название КС: " << station.name;
	cout << "\n Всего цехов: " << station.cehvsego;
	cout << "\n Сколько в работе:  " << station.cehwork;
	cout << "\n Класс станции: (Воздушные - 1, Газовые - 2) " << station.classstation << "\n";
}

void Vseobjects(const Pipe& pipe, const CS& station,
	bool hasPipe, bool hasStation) {
	if (hasPipe) {
		PrintPipe(pipe);
	}
	else {
		cout << "Труба ещё не создана \n";
	}

	if (hasStation) {
		PrintCS(station);
	}
	else {
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
	pipe.vremonte = Diapozon(0,1);
}

void EditCS(CS& station, bool hasStation) {
	if (!hasStation) {
		cout << "КС ещё не задана\n";
		return;
	}

	cout << "1 - Запустить цех, 2 - Остановить цех: ";
	int action = Diapozon(1,2);

	if (action == 1) {
		if (station.cehwork >= station.cehvsego)
			cout << "Все цеха уже запущены \n";
		else {
			station.cehwork++;
			cout << "Цех запущен. В работе: " << station.cehwork << "\n";
		}
	}
	else {
		if (station.cehwork <= 0)
			cout << "Нет работающих цехов \n";
		else {
			station.cehwork--;
			cout << "Цех остановлен. В работе: " << station.cehwork << "\n";
		}
	}
}

void SavePipe(ofstream& fout, const Pipe& pipe) {
	fout << pipe.name << "\n"
		<< pipe.l << "\n"
		<< pipe.d << "\n"
		<< pipe.vremonte << "\n";
}

void SaveCS(ofstream& fout, const CS& station) {
	fout << station.name << "\n"
		<< station.cehvsego << "\n"
		<< station.cehwork << "\n"
		<< station.classstation << "\n";
}

void SaveFile(const Pipe& pipe, const CS& station,
	bool hasPipe, bool hasStation) {
	if (!hasPipe && !hasStation) {
		cout << "Нечего сохранять \n";
		return;
	}

	string filename;
	cout << "Введите имя файла: ";
	getline(cin >> ws, filename);

	ofstream fout(filename);
	if (!fout.is_open()) {
		cout << "Не удалось открыть файл для записи \n";
		return;
	}

	SavePipe(fout, pipe);
	SaveCS(fout, station);

	fout.close();
	cout << "Данные сохранены в " << filename << "\n";
}

void ZagruzkaPipe(ifstream& fin, Pipe& pipe) {
	fin >> pipe.name
		>> pipe.l
		>> pipe.d
		>> pipe.vremonte;
}

void ZagruzkaCS(ifstream& fin, CS& station) {
	fin >> station.name
		>> station.cehvsego
		>> station.cehwork
		>> station.classstation;
}

void Zagruzka(Pipe& pipe, CS& station,
	bool& hasPipe, bool& hasStation) {
	string filename;
	cout << "Введите имя файла: ";
	getline(cin >> ws, filename);

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Файл " << filename << " не найден \n";
		return;
	}

	pipe = Pipe{};
	station = CS{};

	ZagruzkaPipe(fin, pipe);
	ZagruzkaCS(fin, station);

	fin.close();

	hasPipe = true;
	hasStation = true;

	cout << "Данные загружены из " << filename << "\n";
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
		number = Diapozon(0, 7);

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

			//сохранение
		case 6:
			SaveFile(pipe, station, hasPipe, hasStation);
			break;

			//загрузка
		case 7:
			Zagruzka(pipe, station, hasPipe, hasStation);
			break;

		case 0:
			cout << "Выход \n";
			return 0;
		}
	}
}