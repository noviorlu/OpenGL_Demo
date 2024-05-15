#pragma once

#include <memory>

namespace GLCore::Utils {
    template <typename T>
    class MonoSingleton {
    public:
        static T& Instance() {
            static std::unique_ptr<T> instance(new T());
            return *instance;
        }

        MonoSingleton(const MonoSingleton&) = delete;
        MonoSingleton& operator=(const MonoSingleton&) = delete;

    protected:
        MonoSingleton() = default;
        virtual ~MonoSingleton() = default;
    };
}