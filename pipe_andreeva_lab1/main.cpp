#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Pipe.h"
#include "CS.h"

#include <windows.h>

using namespace std;

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

void Vseobjects(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
	cout << "\nТрубы:\n";
	if (pipes.empty())
		cout << "Труб нет\n";
	for (const auto& pipe : pipes)
		cout << pipe.second;

	cout << "\nКС:\n";
	if (stations.empty())
		cout << "КС нет\n";
	for (const auto& station : stations)
		cout << station.second;
}

void EditPipe(unordered_map<int, Pipe>& pipes) {
	cout << "Введите ID трубы: ";
	int id = Diapozon(1, 1000000);

	if (pipes.count(id))
		pipes.at(id).EditPipe();
	else
		cout << "Труба не найдена\n";
}

void DeletePipe(unordered_map<int, Pipe>& pipes) {
	cout << "Введите ID трубы для удаления: ";
	int id = Diapozon(1, 1000000);

	if (pipes.erase(id))
		cout << "Труба удалена\n";
	else
		cout << "Труба не найдена\n";
}

void DeleteCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС для удаления: ";
	int id = Diapozon(1, 1000000);

	if (stations.erase(id))
		cout << "КС удалена\n";
	else
		cout << "КС не найдена\n";
}

void EditCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС: ";
	int id = Diapozon(1, 1000000);

	if (stations.count(id))
		stations.at(id).EditCS();
	else
		cout << "КС не найдена\n";
}

void SaveFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
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
	for (const auto& pipe : pipes)
		pipe.second.SavePipe(fout);

	fout << stations.size() << "\n";
	for (const auto& station : stations)
		station.second.SaveCS(fout);

	fout.close();
	cout << "Данные сохранены в " << filename << "\n";
}

bool ZagruzkaPipes(ifstream& fin, unordered_map<int, Pipe>& pipes, int& nextPipeId) {
	int count;
	if (!(fin >> count) || count < 0 || count > 1000000) return false;
	for (int i = 0; i < count; i++) {
		Pipe pipe;
		pipe.ZagruzkaPipe(fin);
		if (!fin || !pipe.CheckPipe() || pipes.count(pipe.getId())) return false;
		pipes.emplace(pipe.getId(), pipe);
		if (pipe.getId() >= nextPipeId) nextPipeId = pipe.getId() + 1;
	}
	return true;
}

bool ZagruzkaStations(ifstream& fin, unordered_map<int, CS>& stations, int& nextCSId) {
	int count;
	if (!(fin >> count) || count < 0 || count > 1000000) return false;
	for (int i = 0; i < count; i++) {
		CS station;
		station.ZagruzkaCS(fin);
		if (!fin || !station.CheckCS() || stations.count(station.getId())) return false;
		stations.emplace(station.getId(), station);
		if (station.getId() >= nextCSId) nextCSId = station.getId() + 1;
	}
	return true;
}

void Zagruzka(unordered_map<int, Pipe>& pipes, unordered_map<int, CS>& stations,
	int& nextPipeId, int& nextCSId) {
	string filename;
	cout << "Введите имя файла: ";
	getline(cin >> ws, filename);

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Файл " << filename << " не найден\n";
		return;
	}

	unordered_map<int, Pipe> newPipes;
	unordered_map<int, CS> newStations;
	int pipeId = nextPipeId, csId = nextCSId;

	if (!ZagruzkaPipes(fin, newPipes, pipeId) ||
		!ZagruzkaStations(fin, newStations, csId)) {
		cout << "Ошибка файла\n";
		return;
	}

	pipes = move(newPipes);
	stations = move(newStations);
	nextPipeId = pipeId;
	nextCSId = csId;

	cout << "Данные загружены из " << filename << "\n";
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	unordered_map<int, Pipe> pipes;
	unordered_map<int, CS> stations;

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
			pipe.Newpipe();
			pipe.setId(nextPipeId++);
			pipes.emplace(pipe.getId(), pipe);
			break;
		}
		case 2: {
			CS station;
			station.Newcs();
			station.setId(nextCSId++);
			stations.emplace(station.getId(), station);
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
			Zagruzka(pipes, stations, nextPipeId, nextCSId);
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