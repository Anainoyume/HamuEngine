#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <span>

#include "simd.hpp"

namespace hamu::simd
{

    template <typename T>
    struct batch
    {
    public:
        static constexpr int step = 8;

        batch(std::span<T> datas) : _datas(datas) {}

        batch(std::vector<T>& vec) : _datas(vec.data(), vec.size()) {}

        template <size_t N>
        batch(std::array<T, N>& arr) : _datas(arr.data(), N) {}

        template <size_t N>
        batch(T (&arr)[N]) : _datas(arr, N) {}

        batch(T* ptr, size_t count) : _datas(ptr, count) {}

        [[nodiscard]]
        simd<8, T> operator[](size_t index) const noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        simd<8, T> load(size_t index) const noexcept;

        template <bool Aligned = false>
        void store(size_t index, simd<8, T> value) noexcept;

    private:
        std::span<T> _datas;
    };

    template <typename T>
    inline simd<8, T> batch<T>::operator[](size_t index) const noexcept {
        return load<false>(index);
    }

    template <typename T>
    template <bool Aligned>
    inline simd<8, T> batch<T>::load(size_t index) const noexcept {
        using simd8 = simd<8, T>;
        return simd8::template load<Aligned>(&_datas[index]);
    }

    template <typename T>
    template <bool Aligned>
    inline void batch<T>::store(size_t index, simd<8, T> value) noexcept {
        value.template store<Aligned>(&_datas[index]);
    }

} // namespace hamu::simd
