#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "windows.h"

using namespace std;

class Pipe
{
public:
	string name;
	int l = 0;
	int d = 0;
	bool vremonte = 0;
	int id = 0;
};

class CS
{
public:
	string name;
	int cehvsego = 0;
	int cehwork = 0;
	int classstation = 0;
	int id = 0;
};

void Menu() {
	cout << "1. Добавить трубу\n"
		<< "2. Добавить КС \n"
		<< "3. Просмотр всех объектов\n"
		<< "4. Редактировать трубу\n"
		<< "5. Редактировать КС\n"
		<< "6. Сохранить\n"
		<< "7. Загрузить\n"
		<< "8. Удалить трубу\n"
	    << "9. Удалить КС\n"
		<< "0. Выход \n";
}

int Diapozon(int min, int max) {
	int value;
	while (true) {
		while (!(cin >> value)){
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Ошибка! Напишите число: ";
		}
		cin.ignore(1000, '\n');
		if (value >= min && value <= max) {
			return value;
		}
		cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
	}
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
	cout << "ID трубы: " << pipe.id;
	cout << "\n Название трубы: " << pipe.name;
	cout << "\n Длина: " << pipe.l;
	cout << "\n Диаметр:  " << pipe.d;
	cout << "\n В ремонте (Да - 1, Нет - 0): " << pipe.vremonte << "\n";
}

void PrintCS(const CS& station) {
	cout << "ID КС: " << station.id;
	cout << "\n Название КС: " << station.name;
	cout << "\n Всего цехов: " << station.cehvsego;
	cout << "\n Сколько в работе:  " << station.cehwork;
	cout << "\n Класс станции: (Воздушные - 1, Газовые - 2) " << station.classstation << "\n";
}

void Vseobjects(const vector<Pipe>& pipes, const vector<CS>& stations) {
	cout << "\nТрубы:\n";
	if (pipes.empty())
		cout << "Труб нет\n";
	for (const Pipe& pipe : pipes)
		PrintPipe(pipe);

	cout << "\nКС:\n";
	if (stations.empty())
		cout << "КС нет\n";
	for (const CS& station : stations)
		PrintCS(station);
}

void EditPipe(vector<Pipe>& pipes) {
	cout << "Введите ID трубы: ";
	int id = Diapozon(1, 1000000);

	for (Pipe& pipe : pipes) {
		if (pipe.id == id) {
			cout << "Сейчас в ремонте: " << pipe.vremonte << "\n";
			cout << "Новое значение (Да - 1, Нет - 0): ";
			pipe.vremonte = Diapozon(0, 1);
			return;
		}
	}
	cout << "Труба не найдена\n";
}

void DeletePipe(vector<Pipe>& pipes) {
	cout << "Введите ID трубы для удаления: ";
	int id = Diapozon(1, 1000000);

	for (int i = 0; i < pipes.size(); i++) {
		if (pipes[i].id == id) {
			pipes.erase(pipes.begin() + i);
			cout << "Труба удалена\n";
			return;
		}
	}
	cout << "Труба не найдена\n";
}

void DeleteCS(vector<CS>& stations) {
	cout << "Введите ID КС для удаления: ";
	int id = Diapozon(1, 1000000);

	for (int i = 0; i < stations.size(); i++) {
		if (stations[i].id == id) {
			stations.erase(stations.begin() + i);
			cout << "КС удалена\n";
			return;
		}
	}
	cout << "КС не найдена\n";
}

void EditCS(vector<CS>& stations) {
	cout << "Введите ID КС: ";
	int id = Diapozon(1, 1000000);

	for (CS& station : stations) {
		if (station.id != id)
			continue;

		cout << "1 - Запустить цех, 2 - Остановить цех: ";
		int action = Diapozon(1, 2);

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
		return;
	}
	cout << "КС не найдена\n";
}

void SavePipe(ofstream& fout, const Pipe& pipe) {
	fout << pipe.id << "\n"
		<< pipe.name << "\n"
		<< pipe.l << "\n"
		<< pipe.d << "\n"
		<< pipe.vremonte << "\n";
}

void SaveCS(ofstream& fout, const CS& station) {
	fout << station.id << "\n"
		<< station.name << "\n"
		<< station.cehvsego << "\n"
		<< station.cehwork << "\n"
		<< station.classstation << "\n";
}

void SaveFile(const vector<Pipe>& pipes, const vector<CS>& stations) {
	if (pipes.empty() && stations.empty()) {
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
	fout << pipes.size() << "\n";
	for (const Pipe& pipe : pipes)
		SavePipe(fout, pipe);

	fout << stations.size() << "\n";
	for (const CS& station : stations)
		SaveCS(fout, station);

	fout.close();
	cout << "Данные сохранены в " << filename << "\n";
}

void ZagruzkaPipe(ifstream& fin, Pipe& pipe) {
	fin >> pipe.id;
	fin.ignore();
	getline(fin, pipe.name);
	fin >> pipe.l
		>> pipe.d
		>> pipe.vremonte;
	fin.ignore();
}

void ZagruzkaCS(ifstream& fin, CS& station) {
	fin >> station.id;
	fin.ignore();
	getline(fin, station.name);
	fin >> station.cehvsego
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

	hasPipe = !pipe.name.empty();
	hasStation = !station.name.empty();

	cout << "Данные загружены из " << filename << "\n";
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	vector<Pipe> pipes;
	vector<CS> stations;

	int nextPipeId = 1;
	int nextCSId = 1;

	int  number;

	while (true) {
		Menu();
		cout << "Введите команду: ";
		number = Diapozon(0, 9);

		switch (number) {

		case 1: {
			Pipe pipe;
			Newpipe(pipe);
			pipe.id = nextPipeId++;
			pipes.push_back(pipe);
			break;
		}
		case 2: {
			CS station;
			Newcs(station);
			station.id = nextCSId++;
			stations.push_back(station);
			break;
		}
		case 3:
			Vseobjects(pipes, stations);
			break;

		case 4:
			EditPipe(pipes);
			break;

		case 5:
			EditCS(stations);
			break;

		case 6:
			SaveFile(pipes, stations);
			break;

		case 7:
			Zagruzka(pipe, station, hasPipe, hasStation);
			break;

		case 8:
			DeletePipe(pipes);
			break;

		case 9:
			DeleteCS(stations);
			break;

		case 0:
			cout << "Выход \n";
			return 0;
		}
	}
}