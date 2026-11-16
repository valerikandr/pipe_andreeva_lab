#include "CS.h"
#include <iostream>
#include "Utils.h"
#include <climits>

using namespace std;

int CS::MaxID = 1;

CS::CS() {
    id = MaxID++;
}

CS::CS(int loadedId) {
    id = loadedId;
}

void CS::UpdateMaxID(const unordered_map<int, CS>& CSs) {
    MaxID = 1;
    for (const auto& [id, station] : CSs)
        if (id >= MaxID)
            MaxID = id + 1;
}

ostream& operator<<(ostream& out, const CS& station) {
    out << "ID КС: " << station.id;
    out << "\n Название КС: " << station.name;
    out << "\n Всего цехов: " << station.cehvsego;
    out << "\n Работающих цехов: " << station.cehwork;
    out << "\n Класс станции: " << station.classstation << "\n";
    return out;
}

ofstream& operator<<(ofstream& out, const CS& station) {
    out << station.id << "\n"
        << station.name << "\n"
        << station.cehvsego << "\n"
        << station.cehwork << "\n"
        << station.classstation << "\n";
    return out;
}

ifstream& operator>>(ifstream& in, CS& station) {
    in >> station.id;
    getline(in >> ws, station.name);
    in >> station.cehvsego
        >> station.cehwork
        >> station.classstation;

    return in;
}

istream& operator>>(istream& in, CS& station) {
    cout << "Введите название КС: ";
    INPUT_LINE(in, station.name);

    cout << "Введите общее количество цехов: ";
    station.cehvsego = GetCorrectNumber(1, 1000000);

    cout << "Введите количество работающих цехов: ";
    station.cehwork = GetCorrectNumber(0, station.cehvsego);

    cout << "Введите класс станции (1 или 2): ";
    station.classstation = GetCorrectNumber(1, 2);
    return in;
}

int CS::getId() const {
    return id;
}

bool CS::CheckCS() const {

    return id > 0 && id < INT_MAX && !name.empty() &&
        cehvsego > 0 && cehwork >= 0 &&
        cehwork <= cehvsego &&
        (classstation == 1 || classstation == 2);
}

void CS::EditCS() {
    cout << "1 - Запустить цех\n2 - Остановить цех\n";
    int action = GetCorrectNumber(1, 2);

    if (action == 1 && cehwork < cehvsego) {
        cehwork++;
        cout << "Цех запущен\n";
    }
    else if (action == 2 && cehwork > 0) {
        cehwork--;
        cout << "Цех остановлен\n";
    }
    else {
        cout << "Невозможно выполнить действие\n";
    }
}

bool CS::CheckName(const string& value) const {
    return name.find(value) != string::npos;
}

bool CS::CheckPercent(int percent) const {
    return (cehvsego - cehwork) * 100.0 / cehvsego >= percent;
}