#include "CS.h"
#include <iostream>

using namespace std;

int Diapozon(int min, int max);

void CS::Newcs() {
    cout << "Введите название КС: ";
    getline(cin >> ws, name);
    cout << "Введите общее количество цехов: ";
    cehvsego = Diapozon(1, 1000000);
    cout << "Введите количество работающих цехов: ";
    cehwork = Diapozon(0, cehvsego);
    cout << "Введите класс станции (1 или 2): ";
    classstation = Diapozon(1, 2);
}

void CS::SaveCS(ofstream& fout) const {
    fout << id << "\n"
        << name << "\n"
        << cehvsego << "\n"
        << cehwork << "\n"
        << classstation << "\n";
}

void CS::ZagruzkaCS(ifstream& fin) {
    fin >> id;
    fin.ignore();
    getline(fin, name);
    fin >> cehvsego >> cehwork >> classstation;
    fin.ignore();
}

ostream& operator<<(ostream& out, const CS& station) {
    out << "ID КС: " << station.id;
    out << "\n Название КС: " << station.name;
    out << "\n Всего цехов: " << station.cehvsego;
    out << "\n Работающих цехов: " << station.cehwork;
    out << "\n Класс станции: " << station.classstation << "\n";
    return out;
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
        cout << "Цех запущен\n";
    }
    else if (action == 2 && cehwork > 0) {
        cehwork--;
        cout << "Цех остановлен\n";
    }
    else cout << "Невозможно выполнить действие\n";
}