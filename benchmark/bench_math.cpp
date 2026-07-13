#include "benchmark/benchmark.h"

#include "float4.hpp"   // hamu::Float4

#include "glm/geometric.hpp"
#include "glm/vec4.hpp"

#include <random>
#include <vector>

// ---------- 工具函数：生成随机向量数组 ----------
template <typename Vec>
std::vector<Vec> generate_vecs(size_t n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0f, 1000.0f); // 避开0，防止除零
    std::vector<Vec> vecs;
    vecs.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vecs.emplace_back(dis(gen), dis(gen), dis(gen), dis(gen));
    }
    return vecs;
}

std::vector<float> generate_scalars(size_t n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0f, 1000.0f);
    std::vector<float> scalars;
    scalars.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        scalars.push_back(dis(gen));
    }
    return scalars;
}

// ---------- 针对 hamu::Float4 的基准测试 ----------
static void BM_Hamu_Add(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] + vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Add);

static void BM_Hamu_Sub(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] - vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Sub);

static void BM_Hamu_MulVecVec(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] * vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_MulVecVec);

static void BM_Hamu_DivVecVec(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] / vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_DivVecVec);

static void BM_Hamu_MulScalar(benchmark::State& state) {
    const size_t N            = 1024 << 2 << 2;
    static const auto va      = generate_vecs<hamu::Float4>(N);
    static const auto scalars = generate_scalars(N);
    size_t idx                = 0;
    for (auto _ : state) {
        auto result = va[idx] * scalars[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_MulScalar);

static void BM_Hamu_DivScalar(benchmark::State& state) {
    const size_t N            = 1024 << 2 << 2;
    static const auto va      = generate_vecs<hamu::Float4>(N);
    static const auto scalars = generate_scalars(N);
    size_t idx                = 0;
    for (auto _ : state) {
        auto result = va[idx] / scalars[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_DivScalar);

// ---------- 针对 glm::vec4 的基准测试 ----------
static void BM_GLM_Add(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] + vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Add);

static void BM_GLM_Sub(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] - vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Sub);

static void BM_GLM_MulVecVec(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] * vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_MulVecVec);

static void BM_GLM_DivVecVec(benchmark::State& state) {
    const size_t N       = 1024 << 2 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    size_t idx           = 0;
    for (auto _ : state) {
        auto result = va[idx] / vb[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_DivVecVec);

static void BM_GLM_MulScalar(benchmark::State& state) {
    const size_t N            = 1024 << 2 << 2;
    static const auto va      = generate_vecs<glm::vec4>(N);
    static const auto scalars = generate_scalars(N);
    size_t idx                = 0;
    for (auto _ : state) {
        auto result = va[idx] * scalars[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_MulScalar);

static void BM_GLM_DivScalar(benchmark::State& state) {
    const size_t N            = 1024 << 2 << 2;
    static const auto va      = generate_vecs<glm::vec4>(N);
    static const auto scalars = generate_scalars(N);
    size_t idx                = 0;
    for (auto _ : state) {
        auto result = va[idx] / scalars[idx];
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_DivScalar);

// ---------- 针对 hamu::Float4 的复杂运算 ----------
static void BM_Hamu_Dot(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        float result = hamu::dot(va[idx], vb[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Dot);

static void BM_Hamu_Length(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        float result = hamu::length(va[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Length);

static void BM_Hamu_Normalize(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        auto result = hamu::normalize(va[idx]); // 或 normalize_safe
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Normalize);

static void BM_Hamu_Lerp(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<hamu::Float4>(N);
    static const auto vb = generate_vecs<hamu::Float4>(N);
    static const auto ts = generate_scalars(N);
    size_t idx = 0;
    for (auto _ : state) {
        auto result = hamu::lerp(va[idx], vb[idx], ts[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_Hamu_Lerp);

// ---------- 针对 glm::vec4 的复杂运算 ----------
static void BM_GLM_Dot(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        float result = glm::dot(va[idx], vb[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Dot);

static void BM_GLM_Length(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        float result = glm::length(va[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Length);

static void BM_GLM_Normalize(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    size_t idx = 0;
    for (auto _ : state) {
        auto result = glm::normalize(va[idx]);
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Normalize);

static void BM_GLM_Lerp(benchmark::State& state) {
    const size_t N = 1024 << 2;
    static const auto va = generate_vecs<glm::vec4>(N);
    static const auto vb = generate_vecs<glm::vec4>(N);
    static const auto ts = generate_scalars(N);
    size_t idx = 0;
    for (auto _ : state) {
        auto result = glm::mix(va[idx], vb[idx], ts[idx]); // GLM 的 lerp 叫 mix
        benchmark::DoNotOptimize(result);
        idx = (idx + 1) & (N - 1);
    }
}
BENCHMARK(BM_GLM_Lerp);

BENCHMARK_MAIN();