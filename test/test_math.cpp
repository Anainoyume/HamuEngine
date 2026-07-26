#include <cmath>
#include <random>

#include "gtest/gtest.h"
#include <tuple>
#include <utility>

#include "float4.hpp"
#include "geometry.hpp"

#include "float_test_utils.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

#define MAX_ULP 64

namespace detail
{
    struct HPfloat4
    {
        long double x;
        long double y;
        long double z;
        long double w;

        template <typename Vec>
        HPfloat4(const Vec& vec)
            : x(static_cast<long double>(vec.x)), y(static_cast<long double>(vec.y)),
              z(static_cast<long double>(vec.z)), w(static_cast<long double>(vec.w)) {}

        HPfloat4(long double x, long double y, long double z, long double w) : x(x), y(y), z(z), w(w) {}

        template <typename Vec>
        Vec ToLPfloat4() const {
            return Vec {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)};
        }
    };

    constexpr HPfloat4 operator+(const HPfloat4& a, const HPfloat4& b) {
        return HPfloat4 {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }

    constexpr HPfloat4 operator-(const HPfloat4& a, const HPfloat4& b) {
        return HPfloat4 {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }

    constexpr HPfloat4 operator*(const HPfloat4& a, const HPfloat4& b) {
        return HPfloat4 {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    }

    constexpr HPfloat4 operator/(const HPfloat4& a, const HPfloat4& b) {
        return HPfloat4 {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
    }

    constexpr HPfloat4 operator*(const HPfloat4& a, long double s) {
        return HPfloat4 {a.x * s, a.y * s, a.z * s, a.w * s};
    }

    constexpr HPfloat4 operator/(const HPfloat4& a, long double s) {
        return HPfloat4 {a.x / s, a.y / s, a.z / s, a.w / s};
    }

    constexpr long double dot(const HPfloat4& a, const HPfloat4& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    constexpr long double length(const HPfloat4& a) {
        return std::sqrtl(dot(a, a));
    }

    constexpr long double distance(const HPfloat4& a, const HPfloat4& b) {
        return length(b - a);
    }

    constexpr HPfloat4 normalize(const HPfloat4& a) {
        return a / length(a);
    }

    constexpr HPfloat4 lerp(const HPfloat4& a, const HPfloat4& b, long double t) {
        return a + (b - a) * t;
    }

    constexpr HPfloat4 abs(const HPfloat4& a) {
        return HPfloat4 {std::abs(a.x), std::abs(a.y), std::abs(a.z), std::abs(a.w)};
    }

} // namespace detail

template <typename Vec>
constexpr Vec GenerateVector() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
    return Vec(dis(gen), dis(gen), dis(gen), dis(gen));
}

constexpr float GetRandomFloat(float lower, float upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(lower, upper);
    return dis(gen);
}

template <typename Vec>
constexpr std::pair<Vec, Vec> GetRandomVectorPair() {
    return std::pair {GenerateVector<Vec>(), GenerateVector<Vec>()};
}

void CheckFloat(float actual, float expected, std::string_view name) {
    auto ulp = test::ulp_distance(actual, expected);
    EXPECT_LE(ulp, MAX_ULP) << std::format(
        "\nName     : {}"
        "\nExpected : {:.9f}"
        "\nActual   : {:.9f}"
        "\nAbs Error: {:.9e}"
        "\nULP      : {}",
        name,
        expected,
        actual,
        std::abs(expected - actual),
        ulp
    );
}

template <typename Vec>
void Expectfloat4Near(const Vec& a, const Vec& b) {
    CheckFloat(a.x, b.x, "vector - x");
    CheckFloat(a.y, b.y, "vector - y");
    CheckFloat(a.z, b.z, "vector - z");
    CheckFloat(a.w, b.w, "vector - w");
}

TEST(Math, Add) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(a + b, (x + y).ToLPfloat4<hamu::float4>());
}

TEST(Math, Sub) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(a - b, (x - y).ToLPfloat4<hamu::float4>());
}

TEST(Math, Mul) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(a * b, (x * y).ToLPfloat4<hamu::float4>());
}

TEST(Math, Div) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(a / b, (x / y).ToLPfloat4<hamu::float4>());
}

TEST(Math, Mul_Scalar) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    auto k1 = GetRandomFloat(-100.0f, 100.0f);
    auto k2 = GetRandomFloat(-100.0f, 100.0f);

    auto s1 = static_cast<long double>(k1);
    auto s2 = static_cast<long double>(k2);

    Expectfloat4Near(a * k1, (x * s1).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(b * k2, (y * s2).ToLPfloat4<hamu::float4>());
}

TEST(Math, Div_Scalar) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    auto k1 = GetRandomFloat(-100.0f, 100.0f);
    auto k2 = GetRandomFloat(-100.0f, 100.0f);

    auto s1 = static_cast<long double>(k1);
    auto s2 = static_cast<long double>(k2);

    Expectfloat4Near(a / k1, (x / s1).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(b / k2, (y / s2).ToLPfloat4<hamu::float4>());
}

TEST(Math, Dot) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    CheckFloat(hamu::dot(a, b), static_cast<float>(detail::dot(x, y)), "dot");
}

TEST(Math, Normalize) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(hamu::normalize(a), detail::normalize(x).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(hamu::normalize(b), detail::normalize(y).ToLPfloat4<hamu::float4>());
}

TEST(Math, Length) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    CheckFloat(hamu::length(a), static_cast<float>(detail::length(x)), "length");
    CheckFloat(hamu::length(b), static_cast<float>(detail::length(y)), "length");
}

TEST(Math, Distance) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    CheckFloat(hamu::distance(a, b), static_cast<float>(detail::distance(x, y)), "distance");
}

TEST(Math, Lerp) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    auto k1 = GetRandomFloat(0.0f, 1.0f);
    auto k2 = GetRandomFloat(0.0f, 1.0f);
    auto k3 = GetRandomFloat(0.0f, 1.0f);
    auto k4 = GetRandomFloat(0.0f, 1.0f);

    auto s1 = static_cast<long double>(k1);
    auto s2 = static_cast<long double>(k2);
    auto s3 = static_cast<long double>(k3);
    auto s4 = static_cast<long double>(k4);

    Expectfloat4Near(hamu::lerp(a, b, k1), detail::lerp(x, y, s1).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(hamu::lerp(a, b, k2), detail::lerp(x, y, s2).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(hamu::lerp(a, b, k3), detail::lerp(x, y, s3).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(hamu::lerp(a, b, k4), detail::lerp(x, y, s4).ToLPfloat4<hamu::float4>());
}

TEST(Math, Abs) {
    auto [a, b] = GetRandomVectorPair<hamu::float4>();

    auto x = detail::HPfloat4(a);
    auto y = detail::HPfloat4(b);

    Expectfloat4Near(hamu::abs(a), detail::abs(x).ToLPfloat4<hamu::float4>());
    Expectfloat4Near(hamu::abs(b), detail::abs(y).ToLPfloat4<hamu::float4>());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}