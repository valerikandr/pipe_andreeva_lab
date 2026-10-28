#pragma once
#include <string>
#include <fstream>
#include <iostream>

class Pipe
{
private:
    std::string name;
    int l = 0;
    int d = 0;
    bool vremonte = 0;
    int id = 0;

public:
    void Newpipe();
    void SavePipe(std::ofstream& fout) const;
    void ZagruzkaPipe(std::ifstream& fin);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);
    int getId() const;
    void setId(int newId);
    void EditPipe();
    bool CheckPipe() const;
    bool CheckName(const std::string& value) const;
    bool CheckRepair(bool value) const;
    void SetRepair(bool value);
};