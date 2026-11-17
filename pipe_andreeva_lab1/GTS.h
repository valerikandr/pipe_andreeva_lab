#pragma once
#include <unordered_map>
#include "Pipe.h"
#include "CS.h"

struct Link {
    int idIn = 0;
    int idOut = 0;
};

class GTS {
private:
    std::unordered_map<int, Pipe> Pipes;
    std::unordered_map<int, CS> CSS;
    std::unordered_map<int, Link> Links;
    int FindFreePipe(int diameter) const;
public:
    void EditPipe();
    void EditCS();
    void DeletePipe();
    void DeleteCS();
    void PoiskPipe() const;
    void PoiskStation() const;
    void AddPipe();
    void AddCS();
    void Show() const;
    void Connect();
    void Disconnect();
    void SaveFile() const;
    void Zagruzka();
    void PaketPipe();
};

