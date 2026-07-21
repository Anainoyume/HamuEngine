#include "benchmark/benchmark.h"

#include "float4.hpp" // hamu::float4
#include "simd/batch.hpp"
#include "simd/simd_ext.hpp"

#include "glm/geometric.hpp"
#include "glm/vec4.hpp"

#include <random>
#include <vector>

constexpr size_t kBATCH_SIZE = 1024 * 1024; // 1M elements

// ---------- 工具函数：生成随机向量数组 ----------
template <typename Vec>
std::vector<Vec> generate_vecs(size_t n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
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
    std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
    std::vector<float> scalars;
    scalars.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        scalars.push_back(dis(gen));
    }
    return scalars;
}

// ==================== Hamu Batch ====================
#if defined(HAMU_BATCH)
static void BM_Hamu_Add_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] + vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Add_Batch);



static void BM_Hamu_Sub_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] - vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Sub_Batch);



static void BM_Hamu_MulVecVec_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] * vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_MulVecVec_Batch);



static void BM_Hamu_DivVecVec_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] / vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_DivVecVec_Batch);



static void BM_Hamu_MulScalar_Batch(benchmark::State& state) {
    static const auto va      = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto scalars = generate_scalars(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] * scalars[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_MulScalar_Batch);



static void BM_Hamu_DivScalar_Batch(benchmark::State& state) {
    static const auto va      = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto scalars = generate_scalars(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] / scalars[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_DivScalar_Batch);



using simd8f = hamu::simd::simd<8, float>;
static void BM_Hamu_Dot_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<float> result(kBATCH_SIZE);

    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; i += 2) {
            auto a        = simd8f::load(&va[i].x);
            auto b        = simd8f::load(&vb[i].x);
            auto [d1, d2] = hamu::simd::DotPair(a, b);

            result[i]     = d1;
            result[i + 1] = d2;
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Dot_Batch);



static void BM_Hamu_Length_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<float> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; i += 2) {
            auto a        = simd8f::load(&va[i].x);
            auto [d1, d2] = hamu::simd::LengthPair(a);

            result[i]     = d1;
            result[i + 1] = d2;
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Length_Batch);



static void BM_Hamu_Normalize_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; i += 2) {
            auto a = simd8f::load(&va[i].x);
            auto x = hamu::simd::Normalize(a);
            x.store(&result[i].x);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Normalize_Batch);



static void BM_Hamu_Lerp_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static const auto ts = generate_scalars(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; i += 2) {
            auto a = simd8f::load(&va[i].x);
            auto b = simd8f::load(&vb[i].x);
            auto x = hamu::simd::Lerp(a, b, ts[i], ts[i + 1]);
            x.store(&result[i].x);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Lerp_Batch);



static void BM_Hamu_Abs_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<hamu::float4>(kBATCH_SIZE);
    static std::vector<hamu::float4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; i += 2) {
            auto a = simd8f::load(&va[i].x);
            auto x = hamu::simd::Abs(a);
            x.store(&result[i].x);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Abs_Batch);
#endif


// ==================== GLM Batch ====================
#if defined(GLM_BATCH)
static void BM_GLM_Add_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] + vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Add_Batch);

static void BM_GLM_Sub_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] - vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Sub_Batch);

static void BM_GLM_MulVecVec_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] * vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_MulVecVec_Batch);

static void BM_GLM_DivVecVec_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] / vb[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_DivVecVec_Batch);

static void BM_GLM_MulScalar_Batch(benchmark::State& state) {
    static const auto va      = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto scalars = generate_scalars(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] * scalars[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_MulScalar_Batch);

static void BM_GLM_DivScalar_Batch(benchmark::State& state) {
    static const auto va      = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto scalars = generate_scalars(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = va[i] / scalars[i];
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_DivScalar_Batch);

static void BM_GLM_Dot_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<float> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = glm::dot(va[i], vb[i]);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Dot_Batch);

static void BM_GLM_Length_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<float> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = glm::length(va[i]);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Length_Batch);

static void BM_GLM_Normalize_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = glm::normalize(va[i]);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Normalize_Batch);

static void BM_GLM_Lerp_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static const auto ts = generate_scalars(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = glm::mix(va[i], vb[i], ts[i]);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Lerp_Batch);

static void BM_GLM_Abs_Batch(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(kBATCH_SIZE);
    static std::vector<glm::vec4> result(kBATCH_SIZE);
    for (auto _ : state) {
        for (size_t i = 0; i < kBATCH_SIZE; ++i) {
            result[i] = glm::abs(va[i]);
        }
        benchmark::DoNotOptimize(result.data());
    }
    state.SetItemsProcessed(kBATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Abs_Batch);
#endif

BENCHMARK_MAIN();
