#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "Pipe.h"
#include "CS.h"
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

bool FilterPipeName(const Pipe& pipe, string name) {
	return pipe.CheckName(name);
}

bool FilterPipeRepair(const Pipe& pipe, bool repair) {
	return pipe.CheckRepair(repair);
}

bool FilterCSName(const CS& station, string name) {
	return station.CheckName(name);
}


bool FilterCSPercent(const CS& station, int percent) {
	return station.CheckPercent(percent);
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
	int id = GetCorrectNumber(1, 1000000);

	if (pipes.count(id)) {
		pipes.at(id).EditPipe();
	}
	else {
		cout << "Труба не найдена\n";
	}
}

void DeletePipe(unordered_map<int, Pipe>& pipes) {
	cout << "Введите ID трубы для удаления: ";
	int id = GetCorrectNumber(1, 1000000);

	if (pipes.erase(id)) {
		cout << "Труба удалена\n";
		
	}
	else {
		cout << "Труба не найдена\n";
		
	}
}

void DeleteCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС для удаления: ";
	int id = GetCorrectNumber(1, 1000000);

	if (stations.erase(id)) {
		cout << "КС удалена\n";
	}
	else {
		cout << "КС не найдена\n";
		
	}
}

void EditCS(unordered_map<int, CS>& stations) {
	cout << "Введите ID КС: ";
	int id = GetCorrectNumber(1, 1000000);

	if (stations.count(id)) {
		stations.at(id).EditCS();
		
	}
	else {
		cout << "КС не найдена\n";
		
	}
}

void SaveFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
	if (pipes.empty() && stations.empty()) {
		cout << "Нечего сохранять \n";
		return;
	}

	string filename;
	cout << "Введите имя файла: ";
	INPUT_LINE(cin, filename);

	ofstream fout(filename);
	if (!fout.is_open()) {
		cout << "Не удалось открыть файл для записи \n";
		return;
	}

	fout << pipes.size() << "\n";
	for (const auto& pipe : pipes)
		fout << pipe.second;

	fout << stations.size() << "\n";
	for (const auto& station : stations)
		fout << station.second;

	fout.close();
}

bool ZagruzkaPipes(ifstream& fin, unordered_map<int, Pipe>& pipes) {
	int count;
	if (!(fin >> count) || count < 0 || count > 1000000) return false;
	for (int i = 0; i < count; i++) {
		Pipe pipe(0);
		fin >> pipe;
		if (!fin || !pipe.CheckPipe() || pipes.count(pipe.getId())) return false;
		pipes.emplace(pipe.getId(), pipe);
	}
	return true;
}

bool ZagruzkaStations(ifstream& fin, unordered_map<int, CS>& stations) {
	int count;
	if (!(fin >> count) || count < 0 || count > 1000000) return false;
	for (int i = 0; i < count; i++) {
		CS station(0);
		fin >> station;
		if (!fin || !station.CheckCS() || stations.count(station.getId())) return false;
		stations.emplace(station.getId(), station);
	}
	return true;
}

void Zagruzka(unordered_map<int, Pipe>& pipes,
	unordered_map<int, CS>& stations) {
	string filename;
	cout << "Введите имя файла: ";
	INPUT_LINE(cin, filename);

	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Файл " << filename << " не найден\n";
		return;
	}

	unordered_map<int, Pipe> newPipes;
	unordered_map<int, CS> newStations;

	if (!ZagruzkaPipes(fin, newPipes) ||
		!ZagruzkaStations(fin, newStations)) {
		cout << "Ошибка файла\n";
		return;
	}

	pipes = move(newPipes);
	stations = move(newStations);

	Pipe::UpdateMaxID(pipes);
	CS::UpdateMaxID(stations);

	cout << "Данные загружены из " << filename << "\n";
}

void Poisk(const unordered_map<int, Pipe>& pipes) {
	string name;
	cout << "Введите название трубы: ";
	INPUT_LINE(cin, name);

	auto found = FindByFilter(pipes, FilterPipeName, name);

	if (found.empty()) {
		cout << "Трубы не найдены\n";
		return;
	}

	for (int id : found)
		cout << pipes.at(id);
}

void PoiskRepair(const unordered_map<int, Pipe>& pipes) {
	cout << "В ремонте? 1 - да, 0 - нет: ";
	bool value = GetCorrectNumber(0, 1);

	auto found = FindByFilter(pipes, FilterPipeRepair, value);

	if (found.empty()) {
		cout << "Трубы не найдены\n";
		return;
	}

	for (int id : found)
		cout << pipes.at(id);
}

unordered_set<int> PoiskCS(const unordered_map<int, CS>& stations)
{
	string name;
	cout << "Введите название КС: ";
	INPUT_LINE(cin, name);

	return FindByFilter(stations, FilterCSName, name);

}

unordered_set<int> PoiskPercent(const unordered_map<int, CS>& stations) {
	cout << "Минимальный процент незадействованных цехов: ";
	int percent = GetCorrectNumber(0, 100);

	return FindByFilter(stations, FilterCSPercent, percent);
}

void PoiskPipe(const unordered_map<int, Pipe>& pipes) {
	cout << "1. По названию\n2. По ремонту\n";
	int choice = GetCorrectNumber(1, 2);

	if (choice == 1)
		Poisk(pipes);
	else
		PoiskRepair(pipes);
}

void PoiskStation(const unordered_map<int, CS>& stations) {
	cout << "1. По названию\n2. По проценту цехов\n";
	int choice = GetCorrectNumber(1, 2);

	unordered_set<int> found = (choice == 1) ?
		PoiskCS(stations) : PoiskPercent(stations);

	if (found.empty()) {
		cout << "КС не найдены\n";
		return;
	}

	for (int id : found)
		cout << stations.at(id);
}

unordered_set<int> ViborPipe(const unordered_set<int>& found) {
	unordered_set<int> ids;

	cout << "1. Все найденные\n2. Выбрать по ID\n";
	int choice = GetCorrectNumber(1, 2);

	if (choice == 1) return found;

	cout << "Введите ID (0 - закончить): ";
	while (true) {
		int id = GetCorrectNumber(0, 1000000);
		if (id == 0) break;

		if (found.count(id) && !ids.count(id)) {
			ids.insert(id);
		}
		else {
			cout << "ID не найден или уже выбран\n";
		}
	}
	return ids;
}

unordered_set<int> FindPipes(const unordered_map<int, Pipe>& pipes) {
	cout << "1. По названию\n2. По ремонту\n";
	int choice = GetCorrectNumber(1, 2);

	if (choice == 1) {
		string name;
		cout << "Введите название: ";
		INPUT_LINE(cin, name);
		return FindByFilter(pipes, FilterPipeName, name);
	}

	cout << "В ремонте? 1 - да, 0 - нет: ";
	bool repair = GetCorrectNumber(0, 1);
	return FindByFilter(pipes, FilterPipeRepair, repair);
}

void ChangePipes(unordered_map<int, Pipe>& pipes,
	const unordered_set<int>& ids) {

	cout << "Новое состояние ремонта (0 или 1): ";
	int value = GetCorrectNumber(0, 1);

	for (int id : ids)
		pipes[id].SetRepair(value);

	cout << "Трубы изменены\n";
}

void RemovePipes(unordered_map<int, Pipe>& pipes,
	const unordered_set<int>& ids) {

	for (int id : ids)
		pipes.erase(id);

	cout << "Трубы удалены\n";
}


void PaketPipe(unordered_map<int, Pipe>& pipes) {
	unordered_set<int> found = FindPipes(pipes);

	if (found.empty()) {
		cout << "Трубы не найдены\n";
		return;
	}

	for (int id : found)
		cout << pipes.at(id);

	unordered_set<int> ids = ViborPipe(found);
	if (ids.empty()) return;

	cout << "1. Изменить ремонт\n2. Удалить\n";
	int action = GetCorrectNumber(1, 2);

	if (action == 1)
		ChangePipes(pipes, ids);
	else
		RemovePipes(pipes, ids);
}

void AddPipe(unordered_map<int, Pipe>& pipes) {
	Pipe pipe;
	cin >> pipe;
	pipes.emplace(pipe.getId(), pipe);
}

void AddCS(unordered_map<int, CS>& stations) {
	CS station;
	cin >> station;
	stations.emplace(station.getId(), station);
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

	redirect_output_wrapper log(cerr);
	log.redirect(logfile);

	unordered_map<int, Pipe> pipes;
	unordered_map<int, CS> stations;

	int  number;

	while (true) {
		Menu();
		cout << "Введите команду: ";
		number = GetCorrectNumber(0, 12);

		switch (number) {
		case 1:
			AddPipe(pipes);
			break;

		case 2:
			AddCS(stations);
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
			Zagruzka(pipes, stations);
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
			cout << "Выход\n";
			return 0;
		}
	} 
}