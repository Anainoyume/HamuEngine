#pragma once

namespace hamu::utils
{

    template <typename Tp>
    class Singleton
    {
    public:
        Singleton(const Singleton&)            = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&)                 = delete;
        Singleton& operator=(Singleton&&)      = delete;

        static Tp& instance() {
            static Tp instance {};
            return instance;
        }

    private:
        Singleton()  = default;
        ~Singleton() = default;
    };

} // namespace hamu