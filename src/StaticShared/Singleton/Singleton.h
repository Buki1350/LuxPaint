#pragma once

///
/// [IMPORTANT] Requires declaration of static type in .cpp file.
///
template <typename T>
class Singleton {
public:
  inline static T* Instance = nullptr;

protected:
    Singleton() {
        if (Instance == nullptr) Instance = static_cast<T*>(this);
    }

    ~Singleton() {
        Instance = nullptr;
    }
};