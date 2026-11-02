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
    while ((std::cin >> x).fail() ||
        std::cin.peek() != '\n' ||
        x < min || x > max) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ошибка! Введите число от "
            << min << " до " << max << ": ";
    }
    std::cerr << x << std::endl;
    return x;
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