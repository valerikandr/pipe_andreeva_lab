#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class CS
{
private:
    std::string name;
    int cehvsego = 0;
    int cehwork = 0;
    int classstation = 0;
    int id = 0;
    static int MaxID;

public:
    int getId() const;
    void EditCS();
    bool CheckCS() const;
    CS();
    explicit CS(int loadedId);
    static void UpdateMaxID(const std::unordered_map<int, CS>& CSs);
    friend std::ostream& operator<<(std::ostream& out, const CS& station);
    friend std::istream& operator>>(std::istream& in, CS& station);
    friend std::ofstream& operator<<(std::ofstream& out, const CS& station);
    friend std::ifstream& operator>>(std::ifstream& in, CS& station);
    bool CheckName(const std::string& value) const;
    bool CheckPercent(int percent) const;
};