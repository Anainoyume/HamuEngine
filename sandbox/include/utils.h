#pragma once

#include <iostream>

template <typename T>
void TypeIs() {
#if defined(__clang__) || defined(__GNUC__)
    std::cout << __PRETTY_FUNCTION__ << '\n';
#elif defined(_MSC_VER)
    std::cout << __FUNCSIG__ << '\n';
#endif
}


template <typename... Tps>
void ShowTypes() {
    (TypeIs<Tps>(), ...);
};

template <typename T>
void TypeOfValue(T&& arg) {
    TypeIs<decltype(arg)>();
}