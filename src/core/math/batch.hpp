#pragma once

#include <vector>
namespace hamu
{
    struct packed
    {

    private:
        float* _ptr;
    };

    using std::vector;
    class batch
    {
    public:
        batch(const vector<float>& src, vector<float>& dest) noexcept;
        batch(const vector<float>& src_a, const vector<float>& src_b, vector<float>& dest) noexcept;

    private:
        const vector<float>& _src_a;
        const vector<float>& _src_b;
        vector<float>& _dest;
    };

} // namespace hamu