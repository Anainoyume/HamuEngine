#include "include/utils.h"
#include <iostream>

// -----------------------------------------------------------------------------
struct Base {
    int x;
};

struct Derived : public Base {
    int y;
};

struct MegaDerived : public Derived {
    int z;
};

// -----------------------------------------------------------------------------
struct BaseA {
    int a;
};

struct BaseB {
    int b;
};

struct DerivedAB : public BaseA, public BaseB {
    int c;
};

void print(auto&& msg) {
    ShowTypes<decltype(msg)>();
}

int rl() {
    return 1;
}


int main() {
    
    std::cout << "Hello World" << '\n';
    

    return 0;
}