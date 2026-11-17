#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define INPUT_LINE(in, str) \
    std::getline(in >> std::ws, str); \
    std::cerr << str << std::endl

#define PRINT_PARAM(out, x) out << #x << "=" << x << std::endl

class redirect_output_wrapper {
    std::ostream& stream;
    std::streambuf* const old_buf;

public:
    redirect_output_wrapper(std::ostream& src)
        : stream(src), old_buf(src.rdbuf()) {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }

    void redirect(std::ostream& dest) {
        stream.rdbuf(dest.rdbuf());
    }
};

template <typename T>
T GetCorrectNumber(T min, T max) {
    T x;

    while (true) {
        if (!(std::cin >> x)) {
            if (std::cin.eof()) {
                std::exit(0);
            }

            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        else if (std::cin.peek() != '\n' || x < min || x > max) {
            std::cin.ignore(10000, '\n');
        }
        else {
            std::cerr << x << std::endl;
            return x;
        }

        std::cout << "Ошибка! Введите число от "
            << min << " до " << max << ": ";
    }
}

inline int GetDiameter() {
    int d;

    do {
        std::cout << "Диаметр (500, 700, 1000, 1400): ";
        d = GetCorrectNumber(500, 1400);
    } while (d != 500 && d != 700 &&
        d != 1000 && d != 1400);

    return d;
}

template<typename Obj, typename T>
using Filter = bool(const Obj& obj, T param);

template<typename Obj, typename T>
std::unordered_set<int> FindByFilter(
    const std::unordered_map<int, Obj>& objects,
    Filter<Obj, T> f, T param)
{
    std::unordered_set<int> result;

    for (const auto& [id, obj] : objects)
        if (f(obj, param))
            result.insert(id);

    return result;
}