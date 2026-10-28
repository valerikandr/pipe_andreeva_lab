#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Pipe.h"
#include "CS.h"
#include <algorithm>
#include "Utils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

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
		<< "10. Поиск труб\n"
		<< "11. Поиск КС\n"
		<< "12. Пакетное редактирование\n"
		<< "0. Выход \n";
}

int Diapozon(int min, int max) {
	return LogCin(min, max);
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

	if (pipes.count(id)) {
		pipes.at(id).EditPipe();
		cerr << "Изменена труба ID: " << id << "\n";
	}
	else {
		cout << "Труба не найдена\n";
		cerr << "Труба не найдена ID: " << id << "\n";
	}
}

void DeletePipe(unordered_map<int, Pipe>& pipes) {
	cout << "Введите ID трубы для удаления: ";
	int id = Diapozon(1, 1000000);

	if (pipes.erase(id)) {
		cout << "Труба удалена\n";
		cerr << "Удалена труба ID: " << id << "\n";
	}
	else {
		cout << "Труба не найдена\n";
		cerr << "Труба не найдена ID: " << id << "\n";
	}
}

void DeleteCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС для удаления: ";
	int id = Diapozon(1, 1000000);

	if (stations.erase(id)) {
		cout << "КС удалена\n";
		cerr << "Удалена КС ID: " << id << "\n";
	}
	else {
		cout << "КС не найдена\n";
		cerr << "КС не найдена ID: " << id << "\n";
	}
}

void EditCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС: ";
	int id = Diapozon(1, 1000000);

	if (stations.count(id)) {
		stations.at(id).EditCS();
		cerr << "Редактирование КС ID: " << id << "\n";
	}
	else {
		cout << "КС не найдена\n";
		cerr << "КС не найдена ID: " << id << "\n";
	}
}

void SaveFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
	if (pipes.empty() && stations.empty()) {
		cout << "Нечего сохранять \n";
		cerr << "Ошибка сохранения: нет объектов\n";
		return;
	}

	string filename;
	cout << "Введите имя файла: ";
	INPUT_LINE(cin, filename);

	ofstream fout(filename);
	if (!fout.is_open()) {
		cout << "Не удалось открыть файл для записи \n";
		cerr << "Ошибка открытия файла: " << filename << "\n";
		return;
	}

	fout << pipes.size() << "\n";
	for (const auto& pipe : pipes)
		pipe.second.SavePipe(fout);

	fout << stations.size() << "\n";
	for (const auto& station : stations)
		station.second.SaveCS(fout);

	fout.close();
	cerr << "Данные сохранены в " << filename << "\n";
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
	INPUT_LINE(cin, filename);

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Файл " << filename << " не найден\n";
		cerr << "Файл не найден: " << filename << "\n";
		return;
	}

	unordered_map<int, Pipe> newPipes;
	unordered_map<int, CS> newStations;
	int pipeId = nextPipeId, csId = nextCSId;

	if (!ZagruzkaPipes(fin, newPipes, pipeId) ||
		!ZagruzkaStations(fin, newStations, csId)) {
		cout << "Ошибка файла\n";
		cerr << "Ошибка загрузки файла: " << filename << "\n";
		return;
	}

	pipes = move(newPipes);
	stations = move(newStations);
	nextPipeId = pipeId;
	nextCSId = csId;

	cout << "Данные загружены из " << filename << "\n";
	cerr << "Данные загружены из " << filename << "\n";
}

void Poisk(const unordered_map<int, Pipe>& pipes) {
	string name;
	cout << "Введите название трубы: ";
	INPUT_LINE(cin, name);

	bool found = false;
	for (const auto& pipe : pipes) {
		if (pipe.second.CheckName(name)) {
			cout << pipe.second;
			found = true;
		}
	}
	cerr << "Результат поиска: "
		<< (found ? "Объекты найдены" : "Объекты не найдены") << "\n";
	if (!found) cout << "Трубы не найдены\n";
}

void PoiskRepair(const unordered_map<int, Pipe>& pipes) {
	cout << "В ремонте? 1 - да, 0 - нет: ";
	int value = Diapozon(0, 1);

	bool found = false;
	for (const auto& pipe : pipes) {
		if (pipe.second.CheckRepair(value)) {
			cout << pipe.second;
			found = true;
		}
	}
	cerr << "Результат поиска: "
		<< (found ? "Объекты найдены" : "Объекты не найдены") << "\n";
	if (!found) cout << "Трубы не найдены\n";
}

void PoiskCS(const unordered_map<int, CS>& stations) {
	string name;
	cout << "Введите название КС: ";
	INPUT_LINE(cin, name);

	bool found = false;
	for (const auto& station : stations) {
		if (station.second.CheckName(name)) {
			cout << station.second;
			found = true;
		}
	}
	cerr << "Результат поиска: "
		<< (found ? "Объекты найдены" : "Объекты не найдены") << "\n";
	if (!found) cout << "КС не найдены\n";
}

void PoiskPercent(const unordered_map<int, CS>& stations) {
	cout << "Минимальный процент незадействованных цехов: ";
	int percent = Diapozon(0, 100);

	bool found = false;
	for (const auto& station : stations) {
		if (station.second.CheckPercent(percent)) {
			cout << station.second;
			found = true;
		}
	}
	cerr << "Результат поиска: "
		<< (found ? "Объекты найдены" : "Объекты не найдены") << "\n";
	if (!found) cout << "КС не найдены\n";
}

void PoiskPipe(const unordered_map<int, Pipe>& pipes) {
	cout << "1. По названию\n2. По ремонту\n";
	int choice = Diapozon(1, 2);

	if (choice == 1)
		Poisk(pipes);
	else
		PoiskRepair(pipes);
}

void PoiskStation(const unordered_map<int, CS>& stations) {
	cout << "1. По названию\n2. По проценту цехов\n";
	int choice = Diapozon(1, 2);

	if (choice == 1)
		PoiskCS(stations);
	else
		PoiskPercent(stations);
}

vector<int> ViborPipe(const vector<int>& found) {
	vector<int> ids;
	cout << "1. Все найденные\n2. Выбрать по ID\n";
	int choice = Diapozon(1, 2);

	if (choice == 1) return found;

	cout << "Введите ID (введите 0, чтобы закончить:) ";
	while (true) {
		int id = Diapozon(0, 1000000);
		if (id == 0) break;

		if (find(found.begin(), found.end(), id) != found.end()
			&& find(ids.begin(), ids.end(), id) == ids.end()) {
			ids.push_back(id);
			cerr << "Выбрана труба ID: " << id << "\n";
		}
		else {
			cout << "ID не найден или уже выбран\n";
			cerr << "Неверный или повторный ID: " << id << "\n";
		}
	}
	return ids;
}

vector<int> FindPipes(const unordered_map<int, Pipe>& pipes) {
	vector<int> found;
	cout << "1. По названию\n2. По ремонту\n";
	int choice = Diapozon(1, 2);

	string name;
	int value = 0;

	if (choice == 1) {
		cout << "Введите название: ";
		INPUT_LINE(cin, name);
	}
	else {
		cout << "В ремонте? 1 - да, 0 - нет: ";
		value = Diapozon(0, 1);
	}

	for (const auto& pipe : pipes)
		if (choice == 1 ? pipe.second.CheckName(name) :
			pipe.second.CheckRepair(value))
			found.push_back(pipe.first);

	return found;
}

void ChangePipes(unordered_map<int, Pipe>& pipes, const vector<int>& ids) {
	cout << "Новое состояние ремонта (0 или 1): ";
	int value = Diapozon(0, 1);

	for (int id : ids)
		pipes.at(id).SetRepair(value);

	cerr << "Изменено труб: " << ids.size()
		<< ", состояние ремонта: " << value << "\n";

	cout << "Трубы изменены\n";
}

void RemovePipes(unordered_map<int, Pipe>& pipes, const vector<int>& ids) {
	for (int id : ids)
		pipes.erase(id);

	cerr << "Удалено труб: " << ids.size() << "\n";
	cout << "Трубы удалены\n";
}


void PaketPipe(unordered_map<int, Pipe>& pipes) {
	vector<int> found = FindPipes(pipes);

	if (found.empty()) {
		cout << "Трубы не найдены\n";
		return;
	}

	for (int id : found)
		cout << pipes.at(id);

	vector<int> ids = ViborPipe(found);
	if (ids.empty()) return;

	cout << "1. Изменить ремонт\n2. Удалить\n";
	int action = Diapozon(1, 2);

	if (action == 1)
		ChangePipes(pipes, ids);
	else
		RemovePipes(pipes, ids);
}

void AddPipe(unordered_map<int, Pipe>& pipes, int& nextPipeId) {
	Pipe pipe;
	pipe.Newpipe();
	pipe.setId(nextPipeId++);
	pipes.emplace(pipe.getId(), pipe);
	cerr << "Добавлена труба ID: " << pipe.getId() << "\n";
}

void AddCS(unordered_map<int, CS>& stations, int& nextCSId) {
	CS station;
	station.Newcs();
	station.setId(nextCSId++);
	stations.emplace(station.getId(), station);
	cerr << "Добавлена КС ID: " << station.getId() << "\n";
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	time_t now = time(nullptr);
	tm t{};
	localtime_s(&t, &now);

	ostringstream filename;
	filename << "log_" << put_time(&t, "%d_%m_%Y_%H_%M_%S") << ".txt";

	ofstream logfile(filename.str());
	if (!logfile) {
		cout << "Не удалось создать журнал\n";
		return 1;
	}

	streambuf* oldCerr = cerr.rdbuf(logfile.rdbuf());

	unordered_map<int, Pipe> pipes;
	unordered_map<int, CS> stations;

	int nextPipeId = 1;
	int nextCSId = 1;

	int  number;

	while (true) {
		Menu();
		cout << "Введите команду: ";
		number = Diapozon(0, 12);
		cerr << "Выбрана команда меню: " << number << "\n";

		switch (number) {
		case 1:
			AddPipe(pipes, nextPipeId);
			break;

		case 2:
			AddCS(stations, nextCSId);
			break;

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

		case 10:
			PoiskPipe(pipes);
			break;

		case 11:
			PoiskStation(stations);
			break;

		case 12:
			PaketPipe(pipes);
			break;

		case 0:
			cout << "Выход \n";
			cerr << "Выход из программы\n";
			cerr.rdbuf(oldCerr);
			return 0;
		}
	}
}