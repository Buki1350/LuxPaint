#pragma once

///
/// [IMPORTANT] Requires declaration of static type in .cpp file.
///
template <typename T>
class Singleton {
public:
  inline static T* instance = nullptr;

protected:
    Singleton() {
        if (instance == nullptr) instance = static_cast<T*>(this);
    }

    ~Singleton() {
        instance = nullptr;
    }
};