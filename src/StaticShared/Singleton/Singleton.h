#pragma once

template <typename T>
class Singleton {
public:
  inline static T* Instance = nullptr; // działa od razu

protected:
    Singleton() {
        if (Instance == nullptr) Instance = static_cast<T*>(this);
    }

    ~Singleton() {
        Instance = nullptr;
    }
};