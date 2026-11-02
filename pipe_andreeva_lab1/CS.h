#pragma once
#include <string>
#include <fstream>
#include <iostream>

class CS
{
private:
    std::string name;
    int cehvsego = 0;
    int cehwork = 0;
    int classstation = 0;
    int id = 0;

public:
    void Newcs();
    void SaveCS(std::ofstream& fout) const;
    void ZagruzkaCS(std::ifstream& fin);

    int getId() const;
    void setId(int newId);
    void EditCS();
    bool CheckCS() const;
    friend std::ostream& operator<<(std::ostream& out, const CS& station);
    bool CheckName(const std::string& value) const;
    bool CheckPercent(int percent) const;
};