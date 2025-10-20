//
// Created by lukas on 30.08.2025.
//

#include "Utils.h"

#include "../FilesManager/FilesManager.h"
#include <raylib.h>

static Utils _; // singleton declaration

Vec2i Utils::_monitorSize;
Vec2i Utils::_windowSize;
float Utils::_smallerMonitorEdge;
Font Utils::_defaultFont;
Vec2f Utils::_mousePosition;
float Utils::_deltaTime;

Vec2i Utils::GetCurrentMonitorSize() { return _monitorSize; }
Vec2i Utils::GetWindowSize() { return _windowSize; }
float Utils::GetSmallerMonitorEdge() { return _smallerMonitorEdge; }
float Utils::GetDeltaTime() { return _deltaTime; }

void Utils::SetDefaultFont() {
  _defaultFont = LoadFont((std::string(PATH_FONTS) + std::string("Comfortaa-Bold.ttf")).c_str());
}

Color Utils::HexToColor(std::string hex) {
  unsigned int r = 0, g = 0, b = 0, a = 255;

  if (hex.size() == 7) { // #RRGGBB
    sscanf(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
  } else if (hex.size() == 9) { // #RRGGBBAA
    sscanf(hex.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
  }

  return Color{(unsigned char)r, (unsigned char)g, (unsigned char)b,
               (unsigned char)a};
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

Color Utils::LoadColor(std::string token) {
  Color color = HexToColor(FilesManager::Load<std::string>(FILE_COLOR_PALETTE, token));
  if ( color.r == 0 && color.g == 0 && color.b == 0 && color.a == 255 ) {
    FilesManager::Save(std::string(FILE_COLOR_PALETTE), token, "#7d7d7d");
    color = Utils::HexToColor("#7d7d7d");
  }
  return color;
}

Font Utils::GetDefaultFont() { return _defaultFont; }

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

bool Utils::VectorsEqual(Vector2 start, Vector2 end) {
  return start.x == end.x && start.y == end.y;
}

Vec2f Utils::GetMousePosition() { return _mousePosition; }

Matx<Color> Utils::TextureToMatrix(Texture &texture) {
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

Matx<Color> Utils::ImageToMatrix(Image &texture) {
  Texture2D tex = LoadTextureFromImage(texture);
  return TextureToMatrix(tex);
}

Texture2D Utils::MatrixToTexture(Matx<Color> &matrix) {
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


Image Utils::MatrixToImage(Matx<Color> &matrix) {
  return LoadImageFromTexture(MatrixToTexture(matrix));
}

bool Utils::MouseClicked() { return IsMouseButtonReleased(0); }

Color Utils::DarkenColor(const Color& color, float factor) {
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