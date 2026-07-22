#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <span>

#include "Simd.hpp"

namespace hamu::simd
{

    template <typename T>
    struct Batch
    {
    public:
        static constexpr int step = 8;

        Batch(std::span<T> datas) : _datas(datas) {}

        Batch(std::vector<T>& vec) : _datas(vec.data(), vec.size()) {}

        template <size_t N>
        Batch(std::array<T, N>& arr) : _datas(arr.data(), N) {}

        template <size_t N>
        Batch(T (&arr)[N]) : _datas(arr, N) {}

        Batch(T* ptr, size_t count) : _datas(ptr, count) {}

        [[nodiscard]]
        Simd<8, T> operator[](size_t index) const noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        Simd<8, T> load(size_t index) const noexcept;

        template <bool Aligned = false>
        void store(size_t index, Simd<8, T> value) noexcept;

    private:
        std::span<T> _datas;
    };

    template <typename T>
    inline Simd<8, T> Batch<T>::operator[](size_t index) const noexcept {
        return load<false>(index);
    }

    template <typename T>
    template <bool Aligned>
    inline Simd<8, T> Batch<T>::load(size_t index) const noexcept {
        using Simd8 = Simd<8, T>;
        return Simd8::template load<Aligned>(&_datas[index]);
    }

    template <typename T>
    template <bool Aligned>
    inline void Batch<T>::store(size_t index, Simd<8, T> value) noexcept {
        value.template store<Aligned>(&_datas[index]);
    }

} // namespace hamu::Simd
