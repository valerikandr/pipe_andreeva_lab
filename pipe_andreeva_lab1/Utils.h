#pragma once
#include <iostream>
#include <string>

inline int LogCin(int min, int max) {
    int value;
    while (!(std::cin >> value) || value < min || value > max) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
    }
    std::cin.ignore(1000, '\n');
    std::cerr << "Введено число: " << value << "\n";
    return value;
}

#define INPUT_LINE(in, str) \
    { std::getline(in >> std::ws, str); std::cerr << "Введена строка: " << str << "\n"; }