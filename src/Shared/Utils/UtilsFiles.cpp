#pragma once
#include "Utils.h"
#include "../FilesManager/FilesManager.h"
#include "../KeyBindings/KeyNames.h"

void Utils::Files::SetDefaultFont() {
  _defaultFont = LoadFont((std::string(PATH_FONTS) + std::string("Comfortaa-Bold.ttf")).c_str());
}

Color Utils::Files::LoadColor(std::string token, std::string defaultToken) {
  if (!defaultToken.empty()) {
    std::string foundDefaultColorString = FilesManager::Load<std::string>(FILE_COLOR_PALETTE, defaultToken);
    if (!foundDefaultColorString.empty()) {
      return Colors::HexToColor(foundDefaultColorString);
    }
  }

  Color color = Colors::HexToColor(FilesManager::Load<std::string>(FILE_COLOR_PALETTE, token));
  if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 255) {
    FilesManager::Save(std::string(FILE_COLOR_PALETTE), token, "#7d7d7d");
    color = Colors::HexToColor("#7d7d7d");
  }
  return color;
}

void Utils::Files::SaveColor(const char *token, const Color &color) {
  FilesManager::Save("ColorPalette.dat", token, Colors::ColorToHex(color));
}

std::map<InputAction, std::vector<KeyboardKey>> Utils::Files::LoadActions() {
  const std::string path = "Keybindings.dat";
  std::map<InputAction, std::vector<KeyboardKey>> bindings;

  auto lines = FilesManager::LoadFileLines(path.c_str());
  if (lines.empty()) {

    // ... default - exactly in the same order as in InputAction in Keybindings.h
    bindings = {
      {MENU_NEXT, {KEY_TAB}},
      {MENU_PREV, {KEY_LEFT_SHIFT, KEY_TAB}},
      {MENU_CONFIRM, {KEY_ENTER}},
      {MODE_ADD, {KEY_LEFT_SHIFT}},
      {DEBUG_CREATEIMAGE_250x250, {KEY_F5}}};

    // ... save
    std::vector<std::string> saveLines;
    for (auto &[action, keys] : bindings) {
      std::string actionName;
      switch (action) {
      case MENU_NEXT:
        actionName = "MENU_NEXT";
        break;
      case MENU_PREV:
        actionName = "MENU_PREV";
        break;
      case MENU_CONFIRM:
        actionName = "MENU_CONFIRM";
        break;
      case DEBUG_CREATEIMAGE_250x250:
        actionName = "DEBUG_CREATEIMAGE_250x250";
        break;
      default:
        continue;
      }

      std::string keyList;
      for (size_t i = 0; i < keys.size(); i++) {
        keyList += KeyNames::GetKeyName(keys[i]);
        if (i < keys.size() - 1)
          keyList += ", ";
      }

      saveLines.push_back(actionName + ": " + keyList);
    }
    FilesManager::SaveFileLines(path, saveLines);
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
    InputAction action = IA_NONE;
    if (actionName == "MENU_NEXT")
      action = MENU_NEXT;
    else if (actionName == "MENU_PREV")
      action = MENU_PREV;
    else if (actionName == "MENU_CONFIRM")
      action = MENU_CONFIRM;
    else if (actionName == "DEBUG_CREATEIMAGE_250x250")
      action = DEBUG_CREATEIMAGE_250x250;
    else
      continue;

    // ... parse keys
    std::vector<KeyboardKey> keys;
    std::stringstream ss(keysPart);
    std::string token;
    while (std::getline(ss, token, ',')) {
      trim(token);
      KeyboardKey key = KeyNames::GetKeyFromName(token);
      if (key != KEY_NULL)
        keys.push_back(key);
    }

    if (!keys.empty())
      bindings[action] = keys;
  }

  return bindings;
}

void Utils::Files::SaveActions(const std::map<InputAction, std::vector<KeyboardKey>>& bindings)
{
  std::vector<std::string> lines;

  for (auto& [action, keys] : bindings)
  {
    std::string name;
    switch (action)
    {
    case MENU_NEXT: name = "MENU_NEXT"; break;
    case MENU_PREV: name = "MENU_PREV"; break;
    case MENU_CONFIRM: name = "MENU_CONFIRM"; break;
    case DEBUG_CREATEIMAGE_250x250: name = "DEBUG_CREATEIMAGE_250x250"; break;
    default: continue;
    }

    std::string keyList;
    for (size_t i = 0; i < keys.size(); i++)
    {
      keyList += KeyNames::GetKeyName(keys[i]);
      if (i < keys.size() - 1)
        keyList += ", ";
    }

    lines.push_back(name + ": " + keyList);
  }

  FilesManager::SaveFileLines("keybindings.dat", lines);
}


std::map<std::string, Color> Utils::Files::LoadAllColors() {
  std::map<std::string, Color> colors;
  auto lines = FilesManager::LoadFileLines("ColorPalette.dat");

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
      colors[key] = Colors::HexToColor(val);
    }
  }

  return colors;
}

void Utils::Files::SaveAllColors(std::pmr::map<std::string, Color> colorMap) {
  std::vector<std::string> lines;
  for (auto [key, color] : colorMap) {
    std::string colorKeyStr = key;
    std::string colorStr = Colors::ColorToHex(color);
    lines.push_back(colorKeyStr + ": " + colorStr);
  }

  FilesManager::SaveFileLines("ColorPalette.dat", lines);
}