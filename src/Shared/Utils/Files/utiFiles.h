#pragma once
#include "Shared/Keybindings/Keybindings.h"
#include "Shared/Utils/Colors/Colors.h"

#include <map>

namespace uti::files {
  Color loadColor(const std::string& token, const std::string& defaultToken = "", const std::string& defaultNewColor = "#7d7d7d");
  void saveColor(const char *token, const Color &color);
  std::map<InputAction, std::vector<KeyboardKey>> loadActions();
  std::map<std::string, Color> loadAllColors();
  void saveAllColors(std::pmr::map<std::string, Color> colorMap);
  bool compareFiles(const std::string& p1, const std::string& p2);
  void updateAppData();
};
