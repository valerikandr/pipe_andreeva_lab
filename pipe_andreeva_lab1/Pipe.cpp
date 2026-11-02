#include "Pipe.h"
#include <iostream>

using namespace std;

int Diapozon(int min, int max);

void Pipe::Newpipe() {
	cout << "Введите название трубы: ";
	getline(cin >> ws, name);
	cout << "Введите длину: ";
	l = Diapozon(1, 1000000);
	cout << "Введите диаметр: ";
	d = Diapozon(1, 1000000);
	cout << "В ремонте? Введите 1 - да, 0 - нет ";
	vremonte = Diapozon(0, 1);
}

ostream& operator<<(ostream& out, const Pipe& pipe) {
	out << "ID трубы: " << pipe.id;
	out << "\n Название трубы: " << pipe.name;
	out << "\n Длина: " << pipe.l;
	out << "\n Диаметр: " << pipe.d;
	out << "\n В ремонте (Да - 1, Нет - 0): " << pipe.vremonte << "\n";
	return out;
}

void Pipe::SavePipe(ofstream& fout) const {
	fout << id << "\n"
		<< name << "\n"
		<< l << "\n"
		<< d << "\n"
		<< vremonte << "\n";
}

void Pipe::ZagruzkaPipe(ifstream& fin) {
	fin >> id;
	fin.ignore();
	getline(fin, name);
	fin >> l
		>> d
		>> vremonte;
	fin.ignore();
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