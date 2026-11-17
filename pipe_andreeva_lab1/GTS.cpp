#include "GTS.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

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

void GTS::PaketPipe() {
    unordered_set<int> found = FindPipes(Pipes);

    if (found.empty()) {
        cout << "Трубы не найдены\n";
        return;
    }

    for (int id : found)
        cout << Pipes.at(id);

    unordered_set<int> ids = ViborPipe(found);
    if (ids.empty())
        return;

    cout << "1. Изменить ремонт\n2. Удалить\n";
    int action = GetCorrectNumber(1, 2);

    if (action == 1) {
        cout << "Новое состояние ремонта (0 или 1): ";
        bool repair = GetCorrectNumber(0, 1);

        for (int id : ids)
            Pipes.at(id).SetRepair(repair);

        cout << "Трубы изменены\n";
    }
    else {
        for (int id : ids) {
            if (Links.count(id)) {
                cout << "Труба " << id
                    << " подключена. Сначала отсоедините\n";
                continue;
            }

            Pipes.erase(id);
            cout << "Труба " << id << " удалена\n";
        }
    }
}

unordered_set<int> ViborPipe(const unordered_set<int>& found) {
    unordered_set<int> ids;

    cout << "1. Все найденные\n2. Выбрать по ID\n";
    int choice = GetCorrectNumber(1, 2);

    if (choice == 1)
        return found;

    cout << "Введите ID (0 - закончить): ";
    while (true) {
        int id = GetCorrectNumber(0, 1000000);
        if (id == 0)
            break;

        if (found.count(id))
            ids.insert(id);
        else
            cout << "ID не найден\n";
    }

    return ids;
}

void GTS::AddPipe() {
    Pipe pipe;
    cin >> pipe;
    Pipes.emplace(pipe.getId(), pipe);
}

void GTS::AddCS() {
    CS station;
    cin >> station;
    CSS.emplace(station.getId(), station);
}


void GTS::Show() const {
    cout << "\nТрубы:\n";
    if (Pipes.empty())
        cout << "Труб нет\n";

    for (const auto& [id, pipe] : Pipes)
        cout << pipe;

    cout << "\nКС:\n";
    if (CSS.empty())
        cout << "КС нет\n";

    for (const auto& [id, station] : CSS)
        cout << station;
}

void GTS::EditPipe() {
    cout << "Введите ID трубы: ";
    int id = GetCorrectNumber(1, 1000000);

    if (Pipes.count(id))
        Pipes.at(id).EditPipe();
    else
        cout << "Труба не найдена\n";
}

void GTS::EditCS() {
    cout << "Введите ID КС: ";
    int id = GetCorrectNumber(1, 1000000);

    if (CSS.count(id))
        CSS.at(id).EditCS();
    else
        cout << "КС не найдена\n";
}


void GTS::DeletePipe() {
    cout << "Введите ID трубы: ";
    int id = GetCorrectNumber(1, 1000000);

    if (!Pipes.count(id)) {
        cout << "Труба не найдена\n";
        return;
    }

    if (Links.count(id)) {
        cout << "Сначала отсоедините трубу\n";
        return;
    }

    Links.erase(id);
    Pipes.erase(id);
    cout << "Труба удалена\n";
}

void GTS::DeleteCS() {
    cout << "Введите ID КС: ";
    int id = GetCorrectNumber(1, 1000000);

    if (!CSS.count(id)) {
        cout << "КС не найдена\n";
        return;
    }

    for (const auto& [pipeId, link] : Links)
        if (link.idIn == id || link.idOut == id) {
            cout << "Сначала отсоедините трубы\n";
            return;
        }

    CSS.erase(id);
    cout << "КС удалена\n";
}


void GTS::PoiskPipe() const {
    cout << "1. По названию\n2. По ремонту\n";
    int choice = GetCorrectNumber(1, 2);

    unordered_set<int> found;

    if (choice == 1) {
        string name;
        cout << "Введите название трубы: ";
        INPUT_LINE(cin, name);
        found = FindByFilter(Pipes, FilterPipeName, name);
    }
    else {
        cout << "В ремонте? 1 - да, 0 - нет: ";
        bool repair = GetCorrectNumber(0, 1);
        found = FindByFilter(Pipes, FilterPipeRepair, repair);
    }

    if (found.empty()) {
        cout << "Трубы не найдены\n";
        return;
    }

    for (int id : found)
        cout << Pipes.at(id);
}

void GTS::PoiskStation() const {
    cout << "1. По названию\n2. По проценту цехов\n";
    int choice = GetCorrectNumber(1, 2);

    unordered_set<int> found;

    if (choice == 1) {
        string name;
        cout << "Введите название КС: ";
        INPUT_LINE(cin, name);
        found = FindByFilter(CSS, FilterCSName, name);
    }
    else {
        cout << "Минимальный процент незадействованных цехов: ";
        int percent = GetCorrectNumber(0, 100);
        found = FindByFilter(CSS, FilterCSPercent, percent);
    }

    if (found.empty()) {
        cout << "КС не найдены\n";
        return;
    }

    for (int id : found)
        cout << CSS.at(id);
}


int GTS::FindFreePipe(int diameter) const {
    for (const auto& [id, pipe] : Pipes)
        if (!Links.count(id) &&
            pipe.getDiameter() == diameter &&
            !pipe.CheckRepair(true))
            return id;

    return 0;
}


void GTS::Connect() {
    cout << "Введите ID КС входа: ";
    int idIn = GetCorrectNumber(1, 1000000);

    cout << "Введите ID КС выхода: ";
    int idOut = GetCorrectNumber(1, 1000000);

    if (!CSS.count(idIn) || !CSS.count(idOut) || idIn == idOut) {
        cout << "Некорректные ID станций\n";
        return;
    }

    int diameter = GetDiameter();
    int id = FindFreePipe(diameter);

    if (id == 0) {
        cout << "Свободной трубы нет. Создать новую? 1 - да, 0 - нет: ";
        if (GetCorrectNumber(0, 1) == 0)
            return;

        Pipe pipe;
        pipe.InputForConnection(diameter);
        id = pipe.getId();
        Pipes.emplace(id, pipe);
    }

    Links[id] = { idIn, idOut };
    cout << "Труба " << id << " соединяет КС "
        << idIn << " и КС " << idOut << "\n";
}

void GTS::Disconnect() {
    cout << "Введите ID трубы: ";
    int id = GetCorrectNumber(1, 1000000);

    if (!Links.count(id)) {
        cout << "Труба не подключена\n";
        return;
    }

    Links.erase(id);
    cout << "Труба отсоединена\n";
}

int GTS::FindFreePipe(int diameter) const {
    for (const auto& [id, pipe] : Pipes)
        if (!Links.count(id) &&
            pipe.getDiameter() == diameter &&
            !pipe.CheckRepair(true))
            return id;

    return 0;
}

void GTS::Connect() {
    cout << "Введите ID КС входа: ";
    int idIn = GetCorrectNumber(1, 1000000);

    cout << "Введите ID КС выхода: ";
    int idOut = GetCorrectNumber(1, 1000000);

    if (!CSS.count(idIn) || !CSS.count(idOut) || idIn == idOut) {
        cout << "Некорректные ID станций\n";
        return;
    }

    int diameter = GetDiameter();
    int id = FindFreePipe(diameter);

    if (id == 0) {
        cout << "Свободной трубы нет. Создать новую? 1 - да, 0 - нет: ";
        if (GetCorrectNumber(0, 1) == 0)
            return;

        Pipe pipe;
        pipe.InputForConnection(diameter);
        id = pipe.getId();
        Pipes.emplace(id, pipe);
    }

    Links[id] = { idIn, idOut };
    cout << "Труба " << id << " соединяет КС "
        << idIn << " и КС " << idOut << "\n";
}

void GTS::Disconnect() {
    cout << "Введите ID трубы: ";
    int id = GetCorrectNumber(1, 1000000);

    if (!Links.count(id)) {
        cout << "Труба не подключена\n";
        return;
    }

    Links.erase(id);
    cout << "Труба отсоединена\n";
}


void GTS::SaveFile() const {
    string filename;
    cout << "Введите имя файла: ";
    INPUT_LINE(cin, filename);

    ofstream fout(filename);
    if (!fout) {
        cout << "Не удалось открыть файл\n";
        return;
    }

    fout << Pipes.size() << "\n";
    for (const auto& [id, pipe] : Pipes)
        fout << pipe;

    fout << CSS.size() << "\n";
    for (const auto& [id, station] : CSS)
        fout << station;

    fout << Links.size() << "\n";
    for (const auto& [id, link] : Links)
        fout << id << " " << link.idIn << " "
        << link.idOut << "\n";

    cout << "Данные сохранены\n";
}


bool LoadPipes(ifstream& fin, unordered_map<int, Pipe>& pipes) {
    int count;
    if (!(fin >> count) || count < 0 || count > 1000000)
        return false;

    for (int i = 0; i < count; i++) {
        Pipe pipe(0);
        fin >> pipe;

        if (!fin || !pipe.CheckPipe() || pipes.count(pipe.getId()))
            return false;

        pipes.emplace(pipe.getId(), pipe);
    }
    return true;
}


bool LoadStations(ifstream& fin, unordered_map<int, CS>& stations) {
    int count;
    if (!(fin >> count) || count < 0 || count > 1000000)
        return false;

    for (int i = 0; i < count; i++) {
        CS station(0);
        fin >> station;

        if (!fin || !station.CheckCS() || stations.count(station.getId()))
            return false;

        stations.emplace(station.getId(), station);
    }
    return true;
}


void GTS::Zagruzka() {
    string filename;
    cout << "Введите имя файла: ";
    INPUT_LINE(cin, filename);

    ifstream fin(filename);
    if (!fin) {
        cout << "Файл не найден\n";
        return;
    }

    unordered_map<int, Pipe> newPipes;
    unordered_map<int, CS> newCSS;
    unordered_map<int, Link> newLinks;

    if (!LoadPipes(fin, newPipes) ||
        !LoadStations(fin, newCSS)) {
        cout << "Ошибка файла\n";
        return;
    }

    int count;
    if (!(fin >> count) || count < 0 ||
        count > static_cast<int>(newPipes.size())) {
        cout << "Ошибка файла\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        int id, idIn, idOut;
        if (!(fin >> id >> idIn >> idOut) ||
            !newPipes.count(id) ||
            !newCSS.count(idIn) ||
            !newCSS.count(idOut) ||
            idIn == idOut ||
            newLinks.count(id)) {
            cout << "Ошибка соединений\n";
            return;
        }

        newLinks[id] = { idIn, idOut };
    }

    fin >> ws;
    if (!fin.eof()) {
        cout << "Ошибка файла\n";
        return;
    }

    Pipes.swap(newPipes);
    CSS.swap(newCSS);
    Links.swap(newLinks);

    Pipe::UpdateMaxID(Pipes);
    CS::UpdateMaxID(CSS);

    cout << "Данные загружены\n";
}
