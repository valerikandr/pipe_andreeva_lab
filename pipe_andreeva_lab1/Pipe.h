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
    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);
    friend std::istream& operator>>(std::istream& in, Pipe& pipe);
    friend std::ofstream& operator<<(std::ofstream& out, const Pipe& pipe);
    friend std::ifstream& operator>>(std::ifstream& in, Pipe& pipe);
    int getId() const;
    void setId(int newId);
    void EditPipe();
    bool CheckPipe() const;
    bool CheckName(const std::string& value) const;
    bool CheckRepair(bool value) const;
    void SetRepair(bool value);
};