#include <random>

#include "gtest/gtest.h"
#include <tuple>

#include "float4.hpp"

#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

constexpr float eps = 1e-6f;
constexpr float k1  = 275827.273f;
constexpr float k2  = 0.007284f;

template <typename Vec>
constexpr Vec random_vector() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-100.0f, 100.0f);
    return Vec(dis(gen), dis(gen), dis(gen), dis(gen));
}

using vec_pack = std::tuple<hamu::float4, hamu::float4, glm::vec4, glm::vec4>;
vec_pack get_random_data() {
    auto a = random_vector<hamu::float4>();
    auto b = random_vector<hamu::float4>();
    return vec_pack {a, b, glm::vec4(a.x, a.y, a.z, a.w), glm::vec4(b.x, b.y, b.z, b.w)};
}

void ExpectFloat4Near(const hamu::float4& a, const glm::vec4& b) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
    EXPECT_NEAR(a.w, b.w, eps);
}

TEST(Math, Add) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a + b, x + y);
}

TEST(Math, Sub) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a - b, x - y);
}

TEST(Math, Mul) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a * b, x * y);
}

TEST(Math, Div) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a / b, x / y);
}

TEST(Math, Scalar_Mul) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a * k1, x * k1);
    ExpectFloat4Near(b * k2, y * k2);
}

TEST(Math, Scalar_Div) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(a / k1, x / k1);
    ExpectFloat4Near(b / k2, y / k2);
}

TEST(Math, Dot) {
    auto [a, b, x, y] = get_random_data();
    EXPECT_NEAR(hamu::dot(a, b), glm::dot(x, y), eps);
}

TEST(Math, Normalize) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(hamu::normalize(a), glm::normalize(x));
    ExpectFloat4Near(hamu::normalize(b), glm::normalize(y));
}

TEST(Math, Length) {
    auto [a, b, x, y] = get_random_data();
    EXPECT_NEAR(hamu::length(a), glm::length(x), eps);
    EXPECT_NEAR(hamu::length(b), glm::length(y), eps);
}

TEST(Math, Distance) {
    auto [a, b, x, y] = get_random_data();
    EXPECT_NEAR(hamu::distance(a, b), glm::distance(x, y), eps);
}

TEST(Math, Lerp) {
    auto [a, b, x, y] = get_random_data();
    ExpectFloat4Near(hamu::lerp(a, b, 0.34f), glm::mix(x, y, 0.34f));
    ExpectFloat4Near(hamu::lerp(a, b, 0.77f), glm::mix(x, y, 0.77f));
    ExpectFloat4Near(hamu::lerp(a, b, 0.98f), glm::mix(x, y, 0.98f));
    ExpectFloat4Near(hamu::lerp(a, b, 0.03f), glm::mix(x, y, 0.03f));
}

#define Neg(v, s) (v.s = v.s > 0.0f ? -v.s : v.s)
#define NEG(v) Neg(v, x); Neg(v, y); Neg(v, z); Neg(v, w)
TEST(Math, Abs) {
    auto [a, b, x, y] = get_random_data();
    NEG(a);
    NEG(b);
    NEG(x);
    NEG(y);

    ExpectFloat4Near(hamu::abs(a), glm::abs(x));
    ExpectFloat4Near(hamu::abs(b), glm::abs(y));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}