#include <format>
#include <iostream>

#include "float4.hpp"
#include "glm/geometric.hpp"
#include "glm/vec4.hpp"

template <>
struct std::formatter<hamu::float4>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }


    auto format(const hamu::float4& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
    }
};

template <>
struct std::formatter<glm::vec4>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }


    auto format(const glm::vec4& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
    }
};

int main() {

    hamu::float4 a {1.0f, -13.14f, -17.0f, -8.9f};
    hamu::float4 b {-13.1f, 22.0f, 2.0f, 7.7f};

    std::cout << std::format("hamu::dot -> {}\n", a / b);

    glm::vec4 x {1.0f, -13.14f, -17.0f, -8.9f};
    glm::vec4 y {-13.1f, 22.0f, 2.0f, 7.7f};

    std::cout << std::format("glm::dot -> {}\n", x / y);
}
