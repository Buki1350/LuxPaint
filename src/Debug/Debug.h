#pragma once
#include "../Math/Vec2.h"

#include <__msvc_ostream.hpp>
#include <iostream>
#include <string>
#include <vector>

class Debug {
  struct DebugPoint {
    Vec2f position;
    Color color;
  };
  inline static std::vector<DebugPoint> _debugPoints;

  public:
  static void Print(std::string text);
  static void DrawPoint(Vec2f position, Color color);
  static void DrawAll();
  static void PrintPushedKeys();
};