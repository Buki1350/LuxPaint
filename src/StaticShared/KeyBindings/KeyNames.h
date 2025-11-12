#pragma once
#include "raylib.h"

#include <string>

class KeyNames final {
public:
  static std::string GetKeyName(KeyboardKey key)
  {
    switch (key)
    {
    case KEY_TAB: return "TAB";
    case KEY_LEFT_SHIFT: return "LEFT_SHIFT";
    case KEY_ENTER: return "ENTER";
    case KEY_F5: return "F5";
    default: return std::to_string(key); // fallback
    }
  }

  static KeyboardKey GetKeyFromName(const std::string& name)
  {
    if (name == "TAB") return KEY_TAB;
    if (name == "LEFT_SHIFT") return KEY_LEFT_SHIFT;
    if (name == "ENTER") return KEY_ENTER;
    if (name == "F5") return KEY_F5;
    return KEY_NULL;
  }
};