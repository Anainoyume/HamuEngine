#include <cmath>
#include <format>
#include <iomanip>
#include <iostream>

#include "float2.hpp"
#include "float3.hpp"
#include "float4.hpp"

#include "glm/geometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

// ==================== formatters ====================
template <>
struct std::formatter<hamu::float4> : std::formatter<std::string>
{
    auto format(const hamu::float4& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f}, {:>12.8f}, {:>12.8f})", v.x, v.y, v.z, v.w);
    }
};

template <>
struct std::formatter<glm::vec4> : std::formatter<std::string>
{
    auto format(const glm::vec4& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f}, {:>12.8f}, {:>12.8f})", v.x, v.y, v.z, v.w);
    }
};

template <>
struct std::formatter<hamu::float3> : std::formatter<std::string>
{
    auto format(const hamu::float3& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f}, {:>12.8f})", v.x, v.y, v.z);
    }
};

template <>
struct std::formatter<glm::vec3> : std::formatter<std::string>
{
    auto format(const glm::vec3& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f}, {:>12.8f})", v.x, v.y, v.z);
    }
};

template <>
struct std::formatter<hamu::float2> : std::formatter<std::string>
{
    auto format(const hamu::float2& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f})", v.x, v.y);
    }
};

template <>
struct std::formatter<glm::vec2> : std::formatter<std::string>
{
    auto format(const glm::vec2& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:>12.8f}, {:>12.8f})", v.x, v.y);
    }
};

// ==================== helpers ====================
#define SECTION(title) std::cout << "\n========== " << title << " ==========\n"
#define SUBSECTION(title) std::cout << "\n--- " << title << " ---\n"

void print_header() {
    std::cout << std::left << std::setw(22) << "Operation" << std::right << std::setw(66) << "hamu" << std::setw(66)
              << "glm" << "\n";
    std::cout << std::string(154, '-') << "\n";
}

// ---------- float4 helpers ----------
template <typename H, typename G>
void diff_vec(const H& h, const G& g, int n) {
    std::cout << std::left << std::setw(22) << "  diff";
    for (int i = 0; i < n; ++i)
        std::cout << std::scientific << std::setprecision(4) << std::right << std::setw(20) << (&h.x)[i] - (&g.x)[i];
    std::cout << "\n";
}

void compare_vec4(const char* name, const hamu::float4& h, const glm::vec4& g) {
    std::cout << std::left << std::setw(22) << name << std::setw(66) << std::format("{}", h) << std::setw(66)
              << std::format("{}", g) << "\n";
    diff_vec(h, g, 4);
}

void compare_scalar(const char* name, float h, float g) {
    std::cout << std::left << std::setw(22) << name << std::right << std::setw(20) << std::fixed << std::setprecision(8)
              << h << std::right << std::setw(20) << std::fixed << std::setprecision(8) << g << "\n";
    std::cout << std::left << std::setw(22) << "  diff" << std::scientific << std::setprecision(4) << std::right
              << std::setw(20) << (h - g) << "\n";
}

// ---------- float3 helpers ----------
void compare_vec3(const char* name, const hamu::float3& h, const glm::vec3& g) {
    std::cout << std::left << std::setw(22) << name << std::setw(53) << std::format("{}", h) << std::setw(53)
              << std::format("{}", g) << "\n";
    diff_vec(h, g, 3);
}

// ---------- float2 helpers ----------
void compare_vec2(const char* name, const hamu::float2& h, const glm::vec2& g) {
    std::cout << std::left << std::setw(22) << name << std::setw(40) << std::format("{}", h) << std::setw(40)
              << std::format("{}", g) << "\n";
    diff_vec(h, g, 2);
}

int main() {
    // ==================== float4 ====================
    SECTION("float4");

    hamu::float4 ha {1.0f, -13.14f, 17.0f, -8.9f};
    hamu::float4 hb {-13.1f, 22.0f, 2.0f, 7.7f};
    float hs = 3.5f;

    glm::vec4 ga {1.0f, -13.14f, 17.0f, -8.9f};
    glm::vec4 gb {-13.1f, 22.0f, 2.0f, 7.7f};
    float gs = 3.5f;

    SUBSECTION("Arithmetic vec-vec");
    print_header();
    compare_vec4("a + b", ha + hb, ga + gb);
    compare_vec4("a - b", ha - hb, ga - gb);
    compare_vec4("a * b", ha * hb, ga * gb);
    compare_vec4("a / b", ha / hb, ga / gb);
    compare_vec4("-a", -ha, -ga);

    SUBSECTION("Arithmetic vec-scalar");
    print_header();
    compare_vec4("a * s", ha * hs, ga * gs);
    compare_vec4("s * a", hs * ha, gs * ga);
    compare_vec4("a / s", ha / hs, ga / gs);
    compare_vec4("s / a", hs / ha, gs / ga);

    SUBSECTION("Geometric");
    print_header();
    compare_scalar("dot(a, b)", hamu::dot(ha, hb), glm::dot(ga, gb));
    compare_scalar("length(a)", hamu::length(ha), glm::length(ga));
    compare_scalar("distance(a, b)", hamu::distance(ha, hb), glm::distance(ga, gb));
    compare_vec4("normalize(a)", hamu::normalize(ha), glm::normalize(ga));
    compare_vec4("lerp(a, b, 0.3)", hamu::lerp(ha, hb, 0.3f), glm::mix(ga, gb, 0.3f));
    compare_vec4("abs(a)", hamu::abs(ha), glm::abs(ga));

    SUBSECTION("normalize_safe (zero vector)");
    print_header();
    hamu::float4 hzero {0, 0, 0, 0};
    glm::vec4 gzero {0, 0, 0, 0};
    compare_vec4("normalize(0)", hamu::normalize(hzero), glm::normalize(gzero));
    compare_vec4("normalize_safe(0)", hamu::normalize_safe(hzero), glm::normalize(gzero));

    // ==================== float3 ====================
    SECTION("float3");

    hamu::float3 ha3 {1.0f, -13.14f, 17.0f};
    hamu::float3 hb3 {-13.1f, 22.0f, 2.0f};

    glm::vec3 ga3 {1.0f, -13.14f, 17.0f};
    glm::vec3 gb3 {-13.1f, 22.0f, 2.0f};

    SUBSECTION("Arithmetic vec-vec");
    print_header();
    compare_vec3("a + b", ha3 + hb3, ga3 + gb3);
    compare_vec3("a - b", ha3 - hb3, ga3 - gb3);
    compare_vec3("a * b", ha3 * hb3, ga3 * gb3);
    compare_vec3("a / b", ha3 / hb3, ga3 / gb3);
    compare_vec3("-a", -ha3, -ga3);

    SUBSECTION("Arithmetic vec-scalar");
    print_header();
    compare_vec3("a * s", ha3 * hs, ga3 * gs);
    compare_vec3("s * a", hs * ha3, gs * ga3);
    compare_vec3("a / s", ha3 / hs, ga3 / gs);
    compare_vec3("s / a", hs / ha3, gs / ga3);

    SUBSECTION("Geometric");
    print_header();
    compare_scalar("dot(a, b)", hamu::dot(ha3, hb3), glm::dot(ga3, gb3));
    compare_scalar("length(a)", hamu::length(ha3), glm::length(ga3));
    compare_scalar("distance(a, b)", hamu::distance(ha3, hb3), glm::distance(ga3, gb3));
    compare_vec3("normalize(a)", hamu::normalize(ha3), glm::normalize(ga3));
    compare_vec3("lerp(a, b, 0.3)", hamu::lerp(ha3, hb3, 0.3f), glm::mix(ga3, gb3, 0.3f));
    compare_vec3("abs(a)", hamu::abs(ha3), glm::abs(ga3));
    compare_vec3("cross(a, b)", hamu::cross(ha3, hb3), glm::cross(ga3, gb3));

    SUBSECTION("normalize_safe (zero vector)");
    print_header();
    hamu::float3 hzero3 {0, 0, 0};
    glm::vec3 gzero3 {0, 0, 0};
    compare_vec3("normalize(0)", hamu::normalize(hzero3), glm::normalize(gzero3));
    compare_vec3("normalize_safe(0)", hamu::normalize_safe(hzero3), glm::normalize(gzero3));

    // ==================== float2 ====================
    SECTION("float2");

    hamu::float2 ha2 {1.0f, -13.14f};
    hamu::float2 hb2 {-13.1f, 22.0f};

    glm::vec2 ga2 {1.0f, -13.14f};
    glm::vec2 gb2 {-13.1f, 22.0f};

    SUBSECTION("Arithmetic vec-vec");
    print_header();
    compare_vec2("a + b", ha2 + hb2, ga2 + gb2);
    compare_vec2("a - b", ha2 - hb2, ga2 - gb2);
    compare_vec2("a * b", ha2 * hb2, ga2 * gb2);
    compare_vec2("a / b", ha2 / hb2, ga2 / gb2);
    compare_vec2("-a", -ha2, -ga2);

    SUBSECTION("Arithmetic vec-scalar");
    print_header();
    compare_vec2("a * s", ha2 * hs, ga2 * gs);
    compare_vec2("s * a", hs * ha2, gs * ga2);
    compare_vec2("a / s", ha2 / hs, ga2 / gs);
    compare_vec2("s / a", hs / ha2, gs / ga2);

    SUBSECTION("Geometric");
    print_header();
    compare_scalar("dot(a, b)", hamu::dot(ha2, hb2), glm::dot(ga2, gb2));
    compare_scalar("length(a)", hamu::length(ha2), glm::length(ga2));
    compare_scalar("distance(a, b)", hamu::distance(ha2, hb2), glm::distance(ga2, gb2));
    compare_vec2("normalize(a)", hamu::normalize(ha2), glm::normalize(ga2));
    compare_vec2("lerp(a, b, 0.3)", hamu::lerp(ha2, hb2, 0.3f), glm::mix(ga2, gb2, 0.3f));
    compare_vec2("abs(a)", hamu::abs(ha2), glm::abs(ga2));

    SUBSECTION("normalize_safe (zero vector)");
    print_header();
    hamu::float2 hzero2 {0, 0};
    glm::vec2 gzero2 {0, 0};
    compare_vec2("normalize(0)", hamu::normalize(hzero2), glm::normalize(gzero2));
    compare_vec2("normalize_safe(0)", hamu::normalize_safe(hzero2), glm::normalize(gzero2));

    return 0;
}
