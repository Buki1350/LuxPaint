#include "utiFiles.h"

#include "../../../App/App.h"

namespace uti::files {

  Color loadColor(const std::string& token,
                  const std::string& defaultToken,
                  const std::string& defaultNewColor)
  {
    std::string colorStr =
        Serializer::loadFromData<std::string>(FILE_COLOR_PALETTE, token);

    if (!colorStr.empty()) {
      return colors::hexToColor(colorStr);
    }

    if (!defaultToken.empty()) {
      std::string defaultStr =
          Serializer::loadFromData<std::string>(FILE_COLOR_PALETTE, defaultToken);

      if (!defaultStr.empty()) {
        // zapisz kopię defaulta pod tokenem
        Serializer::saveToData<std::string>(
            FILE_COLOR_PALETTE, token, defaultStr);

        return colors::hexToColor(defaultStr);
      }
    }

    Serializer::saveToData<std::string>(
        FILE_COLOR_PALETTE, token, defaultNewColor);

    return colors::hexToColor(defaultNewColor);
  }


void saveColor(const char *token, const Color &color) {
  Serializer::saveToData<std::string>("ColorPalette.dat", token, colors::colorToHex(color));
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