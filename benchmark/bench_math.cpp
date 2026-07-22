#include "benchmark/benchmark.h"

#include "Float4.hpp"   // Float4

#include "glm/geometric.hpp"
#include "glm/vec4.hpp"

#include <random>
#include <vector>

using namespace hamu;

// ---------- 工具函数：生成随机向量数组 ----------
template <typename Vec>
std::vector<Vec> generate_vecs(size_t n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0f, 1000.0f);
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

// 每轮迭代批量处理 BATCH_SIZE 个元素，摊平框架开销
constexpr size_t BATCH_SIZE = 4096;

// ---------- 针对 Float4 的基准测试 ----------
static void BM_Hamu_Add(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] + vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Add);

static void BM_Hamu_Sub(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] - vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Sub);

static void BM_Hamu_MulVecVec(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] * vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_MulVecVec);

static void BM_Hamu_DivVecVec(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] / vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_DivVecVec);

static void BM_Hamu_MulScalar(benchmark::State& state) {
    static const auto va      = generate_vecs<Float4>(BATCH_SIZE);
    static const auto scalars = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] * scalars[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_MulScalar);

static void BM_Hamu_DivScalar(benchmark::State& state) {
    static const auto va      = generate_vecs<Float4>(BATCH_SIZE);
    static const auto scalars = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] / scalars[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_DivScalar);

// ---------- 针对 glm::vec4 的基准测试 ----------
static void BM_GLM_Add(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] + vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Add);

static void BM_GLM_Sub(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] - vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Sub);

static void BM_GLM_MulVecVec(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] * vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_MulVecVec);

static void BM_GLM_DivVecVec(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] / vb[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_DivVecVec);

static void BM_GLM_MulScalar(benchmark::State& state) {
    static const auto va      = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto scalars = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] * scalars[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_MulScalar);

static void BM_GLM_DivScalar(benchmark::State& state) {
    static const auto va      = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto scalars = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = va[i] / scalars[i];
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_DivScalar);

// ---------- 针对 Float4 的复杂运算 ----------
static void BM_Hamu_Dot(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        float sink = 0;
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = dot(va[i], vb[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Dot);

static void BM_Hamu_Length(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        float sink = 0;
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = length(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Length);

static void BM_Hamu_Normalize(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = normalize(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Normalize);

static void BM_Hamu_Lerp(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    static const auto vb = generate_vecs<Float4>(BATCH_SIZE);
    static const auto ts = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = lerp(va[i], vb[i], ts[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Lerp);

// ---------- 针对 glm::vec4 的复杂运算 ----------
static void BM_GLM_Dot(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        float sink = 0;
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = glm::dot(va[i], vb[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Dot);

static void BM_GLM_Length(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        float sink = 0;
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = glm::length(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Length);

static void BM_GLM_Normalize(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = glm::normalize(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Normalize);

static void BM_GLM_Lerp(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto vb = generate_vecs<glm::vec4>(BATCH_SIZE);
    static const auto ts = generate_scalars(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = glm::mix(va[i], vb[i], ts[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Lerp);

static void BM_Hamu_Abs(benchmark::State& state) {
    static const auto va = generate_vecs<Float4>(BATCH_SIZE);
    for (auto _ : state) {
        Float4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = abs(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_Hamu_Abs);

static void BM_GLM_Abs(benchmark::State& state) {
    static const auto va = generate_vecs<glm::vec4>(BATCH_SIZE);
    for (auto _ : state) {
        glm::vec4 sink{0};
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            sink = glm::abs(va[i]);
            benchmark::DoNotOptimize(sink);
        }
    }
    state.SetItemsProcessed(BATCH_SIZE * state.iterations());
}
BENCHMARK(BM_GLM_Abs);

BENCHMARK_MAIN();
