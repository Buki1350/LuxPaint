#pragma once
#include "Shared/UIObjects/UIObject.h"
#include "raylib.h"
#include <string>

namespace uti::colors {

Color hexToColor(std::string hex);
std::string colorToHex(const Color &color, bool includeAlpha = false);

Color darkenColor(const Color& color, float factor);
Color lightenColor(Color color, float x);

float getColorLuminance(Color c);
Color getDynamicBlackOrWhiteFor(Color color);

bool areColorsEqual(Color color1, Color color2);

}
