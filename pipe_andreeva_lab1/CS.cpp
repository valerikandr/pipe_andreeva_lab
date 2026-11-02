#include "CS.h"
#include <iostream>
#include "Utils.h"

using namespace std;

int Diapozon(int min, int max);

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
    station.cehvsego = Diapozon(1, 1000000);

    cout << "Введите количество работающих цехов: ";
    station.cehwork = Diapozon(0, station.cehvsego);

    cout << "Введите класс станции (1 или 2): ";
    station.classstation = Diapozon(1, 2);
    return in;
}

int CS::getId() const {
    return id;
}

void CS::setId(int newId) {
    id = newId;
}

bool CS::CheckCS() const {
    return id > 0 && !name.empty() &&
        cehvsego > 0 && cehwork >= 0 &&
        cehwork <= cehvsego &&
        (classstation == 1 || classstation == 2);
}

void CS::EditCS() {
    cout << "1 - Запустить цех\n2 - Остановить цех\n";
    int action = Diapozon(1, 2);

    if (action == 1 && cehwork < cehvsego) {
        cehwork++;
        cerr << "Запущен цех КС ID: " << id << "\n";
        cout << "Цех запущен\n";
    }
    else if (action == 2 && cehwork > 0) {
        cehwork--;
        cerr << "Остановлен цех КС ID: " << id << "\n";
        cout << "Цех остановлен\n";
    }
    else {
        cout << "Невозможно выполнить действие\n";
        cerr << "Не удалось изменить КС ID: " << id << "\n";
    }
}

bool CS::CheckName(const string& value) const {
    return name.find(value) != string::npos;
}

bool CS::CheckPercent(int percent) const {
    return (cehvsego - cehwork) * 100.0 / cehvsego >= percent;
}