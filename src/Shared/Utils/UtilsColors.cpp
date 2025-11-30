#pragma once
#include "Utils.h"

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
