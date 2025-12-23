#include "utiFiles.h"

#include "../../../App/App.h"

namespace uti::files {

void updateAppData() {

  static bool defaultFontLoaded = false;
  if (!defaultFontLoaded) {
    App::instance->_appData.defaultFont =
       LoadFont((std::string(PATH_FONTS) + "Comfortaa-Bold.ttf").c_str());
    defaultFontLoaded = true;
  }

  Vec2i currentMonitorSize = Vec2i( GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
  Vec2i windowSize = Vec2i(GetScreenWidth(), GetScreenHeight());

  App::instance->_appData.monitorSize = currentMonitorSize;
  App::instance->_appData.smallerMonitorEdge = std::min(currentMonitorSize.x, currentMonitorSize.y);
  App::instance->_appData.windowSize = windowSize;
}

Color loadColor(std::string token, std::string defaultToken) {
  if (!defaultToken.empty()) {
    std::string foundDefaultColorString = Serializer::LoadFromResources<std::string>(FILE_COLOR_PALETTE, defaultToken);
    if (!foundDefaultColorString.empty()) {
      return colors::hexToColor(foundDefaultColorString);
    }
  }

  Color color = colors::hexToColor(Serializer::LoadFromResources<std::string>(FILE_COLOR_PALETTE, token));
  if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 255) {
    Serializer::SaveToResources<std::string>(FILE_COLOR_PALETTE, token, "#7d7d7d");
    color = colors::hexToColor("#7d7d7d");
  }
  return color;
}

void saveColor(const char *token, const Color &color) {
  Serializer::SaveToData<std::string>("ColorPalette.dat", token, colors::colorToHex(color));
}

std::map<InputAction, std::vector<KeyboardKey>> loadActions() {
  const std::string path = "Keybindings.dat";
  std::map<InputAction, std::vector<KeyboardKey>> bindings;

  auto lines = Serializer::loadFileLines(path.c_str());

  if (lines.empty()) {
    bindings = ::Keybindings::getDefaultBindings();

    // ... save
    std::vector<std::string> saveLines;
    for (auto &[action, keys] : bindings) {
      std::string actionName = ::Keybindings::actionToString(action);

      std::string keyList;
      for (size_t i = 0; i < keys.size(); i++) {
        keyList += ::Keybindings::keyboardKeyToString(keys[i]);
        if (i < keys.size() - 1)
          keyList += ", ";
      }
      saveLines.push_back(actionName + ": " + keyList);
    }
    Serializer::saveFileLines(path, saveLines);
    return bindings;
  }

  // ... parse
  for (auto &line : lines) {
    if (line.empty())
      continue;
    size_t pos = line.find(':');
    if (pos == std::string::npos)
      continue;

    std::string actionName = line.substr(0, pos);
    std::string keysPart = line.substr(pos + 1);

    auto trim = [](std::string &s) {
      s.erase(0, s.find_first_not_of(" \t"));
      s.erase(s.find_last_not_of(" \t") + 1);
    };
    trim(actionName);
    trim(keysPart);

    // ... map
    InputAction action = ::Keybindings::actionFromString(actionName);

    // ... parse keys
    std::vector<KeyboardKey> keys;
    std::stringstream ss(keysPart);
    std::string token;
    while (std::getline(ss, token, ',')) {
      trim(token);
      KeyboardKey key = ::Keybindings::stringToKeyboardKey(token);
      if (key != KEY_NULL)
        keys.push_back(key);
    }

    if (!keys.empty())
      bindings[action] = keys;
  }

  return bindings;
}

std::map<std::string, Color> loadAllColors() {
  std::map<std::string, Color> colors;
  auto lines = Serializer::loadFileLines("ColorPalette.dat");

  // ... white spaces around ereased
  auto trim = [](std::string& s) {
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
  };

  for (auto& line : lines) {
    if (line.empty()) continue;

    size_t pos = line.find(':');
    if (pos == std::string::npos) continue;

    std::string key = line.substr(0, pos);
    std::string val = line.substr(pos + 1);
    trim(key);
    trim(val);

    if (!key.empty() && !val.empty()) {
      colors[key] = colors::hexToColor(val);
    }
  }

  return colors;
}

void saveAllColors(std::pmr::map<std::string, Color> colorMap) {
  std::vector<std::string> lines;
  for (auto [key, color] : colorMap) {
    std::string colorKeyStr = key;
    std::string colorStr = colors::colorToHex(color);
    lines.push_back(colorKeyStr + ": " + colorStr);
  }

  Serializer::saveFileLines("ColorPalette.dat", lines);
}

bool compareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

}