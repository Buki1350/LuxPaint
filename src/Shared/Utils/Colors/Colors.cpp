#include "Colors.h"

#include <__msvc_ostream.hpp>
#include <iostream>

namespace uti::colors {

Color hexToColor(const std::string hex) {
  std::string value = hex;

  if (!value.empty() && value[0] == '#') {
    value = value.substr(1);
  }

  for (auto& c : value) c = (char)tolower(c);

  if (value.size() != 6 && value.size() != 8) {
    return Color{255, 255, 255, 255};
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

std::string colorToHex(const Color& color, bool includeAlpha) {
  char buffer[10];
  if (includeAlpha)
    snprintf(buffer, sizeof(buffer), "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
  else
    snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", color.r, color.g, color.b);
  return std::string(buffer);
}

Color darkenColor(const Color& color, float factor) {
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

Color lightenColor(Color color, float factor) {
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

float getColorLuminance(Color c) {
  float alpha = c.a / 255.0f;

  float bg = 255.0f;

  float r = c.r * alpha + bg * (1.0f - alpha);
  float g = c.g * alpha + bg * (1.0f - alpha);
  float b = c.b * alpha + bg * (1.0f - alpha);

  return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

// returns black or whhite depending on UIObject luminance
Color getDynamicBlackOrWhiteFor(Color color) {
  std::cout << colorToHex(color, true) << std::endl;
  return getColorLuminance(color) < 160.0f ? WHITE : BLACK;
}

bool areColorsEqual(Color color1, Color color2) {
  return
    color1.r == color2.r && color1.g == color2.g &&
    color1.b == color2.b && color1.a == color2.a;
}

}