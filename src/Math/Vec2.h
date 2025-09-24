#pragma once
#include "raylib.h"

#include <cmath>
#include <type_traits>

template<typename T>
class Vec2 {
public:
  T x = 0;
  T y = 0;

  Vec2() = default;
  Vec2(T x, T y) : x(x), y(y) {}
  Vec2(Vector2 rlVec2) : x(static_cast<T>(rlVec2.x)), y(static_cast<T>(rlVec2.y)) {}
  explicit Vec2(T val) : x(val), y(val) {}

  static Vec2 zero() { return Vec2(0, 0); }
  static Vec2 ones() { return Vec2(1, 1); }

  Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
  Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
  Vec2 operator*(const Vec2& other) const { return {x * other.x, y * other.y}; }
  Vec2 operator/(const Vec2& other) const { return {x / other.x, y / other.y}; }

  Vec2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
  Vec2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

  Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
  Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
  Vec2& operator*=(const Vec2& other) { x *= other.x; y *= other.y; return *this; }
  Vec2& operator/=(const Vec2& other) { x /= other.x; y /= other.y; return *this; }

  Vec2& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
  Vec2& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

  bool operator!=(const Vec2& other) const { return x != other.x || y != other.y; }
  bool operator==(const Vec2 & vec2) const { return x == vec2.x && y == vec2.y; };

  template <typename U> Vec2<U> CastTo() {
    return Vec2<U>(static_cast<U>(x), static_cast<U>(y));
  }

  float Magnitude() const {
    return std::sqrt(
      static_cast<float>(x) * static_cast<float>(x) +
      static_cast<float>(y) * static_cast<float>(y)
      );
  }
};

//.. alias'
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
