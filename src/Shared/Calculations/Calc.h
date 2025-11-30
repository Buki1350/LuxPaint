#pragma once

class Calc {
public:
  template <typename T>
  static T Clamp(const T &value, const T &minVal, const T &maxVal) {
    if (value < minVal)
      return minVal;
    if (value > maxVal)
      return maxVal;
    return value;
  }

  static float Lerp(float current, float x, float t) {
    return current + t * (x - current);
  }

  class UIO {
    public:
    static Vec2f LeftCorner(const UIObject *uio);
  };
};
