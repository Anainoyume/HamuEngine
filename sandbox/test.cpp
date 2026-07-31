#include <iostream>
#include <vector>


struct Object
{
    int uuid;
    std::string name;

    static int version;

    Object() {}
    ~Object() {}

    Object(int id) : uuid(id) {}

    void foo() {
        std::cout << "foo\n";
    }

    float GetItem(int version, std::string_view s) {
        std::cout << version << " " << s << '\n';
        return 3.1f;
    }

    static void Print(int index) {
        std::cout << index << '\n';
    }
};

template <typename... Args>
struct method_arguments
{};

template <typename T, typename M>
struct field_info
{
    M T::* pointer;
};

template <typename T, typename R, typename... Args>
struct method_info
{
    R (T::*pointer)(Args...);
};

template <typename T>
struct meta_info {
    
};



int main() {

    Object o;

    using a = decltype(&Object::foo);
    using b = decltype(&Object::GetItem);
    using c = decltype(&Object::Print);

}