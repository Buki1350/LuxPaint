//
// Created by lukas on 30.08.2025.
//

#include "Utils.h"

#include "../FilesManager/FilesManager.h"
#include "../KeyBindings/KeyNames.h"

#include <raylib.h>

static Utils _; // singleton declaration

Vec2i Utils::_monitorSize;
Vec2i Utils::_windowSize;
float Utils::_smallerMonitorEdge;
Font Utils::_defaultFont;
Vec2f Utils::_mousePosition;
float Utils::_deltaTime;

Vec2i Utils::View::GetCurrentMonitorSize() { return _monitorSize; }
Vec2i Utils::View::GetWindowSize() { return _windowSize; }
float Utils::View::GetSmallerMonitorEdge() { return _smallerMonitorEdge; }
float Utils::Time::GetDeltaTime() { return _deltaTime; }


void Utils::Files::SetDefaultFont() {
  _defaultFont = LoadFont((std::string(PATH_FONTS) + std::string("Comfortaa-Bold.ttf")).c_str());
}

Color Utils::Colors::HexToColor(const std::string hex) {
  std::string value = hex;

  if (!value.empty() && value[0] == '#') {
    value = value.substr(1);
  }

  for (auto& c : value) c = (char)tolower(c);

  if (value.size() != 6 && value.size() != 8) {
    return Color{255, 255, 255, 255}; // domyślny kolor (biały)
  }

  auto hexToByte = [](const std::string& str, size_t pos) -> unsigned char {
    return (unsigned char)std::stoul(str.substr(pos, 2), nullptr, 16);
  };

  Color color{};
  color.r = hexToByte(value, 0);
  color.g = hexToByte(value, 2);
  color.b = hexToByte(value, 4);
  color.a = (value.size() == 8) ? hexToByte(value, 6) : 255;

  return color;
}

std::string Utils::Colors::ColorToHex(const Color& color, bool includeAlpha) {
  char buffer[10];
  if (includeAlpha)
    snprintf(buffer, sizeof(buffer), "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
  else
    snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", color.r, color.g, color.b);
  return std::string(buffer);
}



Vec2i Utils::_GetCurrentMonitorSize() {
  int monitor = GetCurrentMonitor();
  int monitorHeight = (int)GetMonitorHeight(monitor);
  int monitorWidth = (int)GetMonitorWidth(monitor);
  return {monitorWidth, monitorHeight};
}

Vec2i Utils::_GetWindowSize() {
  return {GetScreenWidth(), GetScreenHeight()};
}

Color Utils::Files::LoadColor(std::string token) {
  Color color =
      Colors::HexToColor(FilesManager::Load<std::string>(FILE_COLOR_PALETTE, token));
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

Font Utils::AppData::GetDefaultFont() { return _defaultFont; }

float Utils::_GetSmallerMonitorEdge() {
  return _monitorSize.y < _monitorSize.x? _monitorSize.y : _monitorSize.x;
}

void Utils::Update() {
  _monitorSize = Utils::_GetCurrentMonitorSize();
  _windowSize = Utils::_GetWindowSize();
  _smallerMonitorEdge = Utils::_GetSmallerMonitorEdge();
  Vector2 RLmousePosition = ::GetMousePosition();
  _mousePosition = {RLmousePosition.x, RLmousePosition.y};
  _deltaTime = GetFrameTime();
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

bool Utils::RaylibSpecific::VectorsEqual(Vector2 start, Vector2 end) {
  return start.x == end.x && start.y == end.y;
}

Vec2f Utils::Input::GetMousePosition() { return _mousePosition; }

Matx<Color> Utils::Convert::TextureToMatrix(Texture &texture) {
  Image image = LoadImageFromTexture(texture);

  Matx<Color> matrix(image.height, image.width);

  Color *pixels = LoadImageColors(image);

  for (int y = 0; y < image.height; y++) {
    for (int x = 0; x < image.width; x++) {
      matrix[y][x] = pixels[y * image.width + x];
    }
  }

  UnloadImageColors(pixels);
  return matrix;
}

Matx<Color> Utils::Convert::ImageToMatrix(Image &texture) {
  Texture2D tex = LoadTextureFromImage(texture);
  return TextureToMatrix(tex);
}

Texture2D Utils::Convert::MatrixToTexture(Matx<Color> &matrix) {
  int height = static_cast<int>(matrix.rowCount());
  int width  = static_cast<int>(matrix.colCount());

  Image img = GenImageColor(width, height, BLANK);

   Color* pixels = (Color*)img.data;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      pixels[y * width + x] = matrix[y][x];
    }
  }

  Texture2D tex = LoadTextureFromImage(img);
  UnloadImage(img);

  return tex;
}


Image Utils::Convert::MatrixToImage(Matx<Color> &matrix) {
  return LoadImageFromTexture(MatrixToTexture(matrix));
}

bool Utils::Input::MouseReleased() { return IsMouseButtonReleased(0); }

bool Utils::Input::MousePressed() { return IsMouseButtonPressed(0); }

bool Utils::Input::MouseDown() { return IsMouseButtonDown(0); }

Color Utils::Colors::DarkenColor(const Color& color, float factor) {
  factor = 1 - factor;
  if (factor < 0.0f) factor = 0.0f;
  if (factor > 1.0f) factor = 1.0f;

  Color result;
  result.r = (unsigned char)(color.r * factor);
  result.g = (unsigned char)(color.g * factor);
  result.b = (unsigned char)(color.b * factor);
  result.a = color.a;
  return result;
}

Color Utils::Colors::LightenColor(Color color, float factor) {
  factor = 1 + factor;
  if (factor < 1.0f) factor = 1.0f;
  if (factor > 2.0f) factor = 2.0f;

  Color result;
  result.r = (unsigned char)std::min(255.0f, color.r * factor);
  result.g = (unsigned char)std::min(255.0f, color.g * factor);
  result.b = (unsigned char)std::min(255.0f, color.b * factor);
  result.a = color.a;
  return result;
}

