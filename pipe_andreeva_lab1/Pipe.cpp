#include "Pipe.h"
#include <iostream>
#include "Utils.h"

using namespace std;

int Diapozon(int min, int max);

ostream& operator<<(ostream& out, const Pipe& pipe) {
	out << "ID трубы: " << pipe.id;
	out << "\n Название трубы: " << pipe.name;
	out << "\n Длина: " << pipe.l;
	out << "\n Диаметр: " << pipe.d;
	out << "\n В ремонте (Да - 1, Нет - 0): " << pipe.vremonte << "\n";
	return out;
}

ofstream& operator<<(ofstream& out, const Pipe& pipe) {
	out << pipe.id << "\n"
		<< pipe.name << "\n"
		<< pipe.l << "\n"
		<< pipe.d << "\n"
		<< pipe.vremonte << "\n";
	return out;
}

ifstream& operator>>(ifstream& in, Pipe& pipe) {
	in >> pipe.id;
	getline(in >> ws, pipe.name);
	in >> pipe.l >> pipe.d >> pipe.vremonte;
	return in;
}

istream& operator>>(istream& in, Pipe& pipe) {
	cout << "Введите название трубы: ";
	INPUT_LINE(in, pipe.name);

	cout << "Введите длину: ";
	pipe.l = Diapozon(1, 1000000);

	cout << "Введите диаметр: ";
	pipe.d = Diapozon(1, 1000000);

	cout << "В ремонте? 1 - да, 0 - нет: ";
	pipe.vremonte = Diapozon(0, 1);
	return in;
}

int Pipe::getId() const {
	return id;
}

void Pipe::setId(int newId) {
	id = newId;
}

void Pipe::EditPipe() {
	cout << "Сейчас в ремонте: " << vremonte << "\n";
	cout << "Новое значение (Да - 1, Нет - 0): ";
	vremonte = Diapozon(0, 1);
	cerr << "Новое состояние ремонта трубы: " << vremonte << "\n";
}

bool Pipe::CheckPipe() const {
	return id > 0 && !name.empty() && l > 0 && d > 0;
}

bool Pipe::CheckName(const string& value) const {
	return name.find(value) != string::npos;
}

bool Pipe::CheckRepair(bool value) const {
	return vremonte == value;
}

void Pipe::SetRepair(bool value) {
	vremonte = value;
}