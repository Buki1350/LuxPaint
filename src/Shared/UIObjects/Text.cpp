#include "Text.h"
#include "App/App.h"
#include "Shared/Utils/Text/utiText.h"
#include "UIObject.h"

#include <iostream>
void Text::setParent(UIObject *parent) {
  uiObject_parent = parent;
}

#include <sstream>

void Text::wrapToWidth(TextWrapType wrapType) {
  if (!uiObject_parent) return;

  Font font = App::instance().getAppData().defaultFont;
  float maxWidth = uiObject_parent->size.x - 2 * margin;
  float maxHeight = uiObject_parent->size.y - 2 * margin;
  float lineHeight = (float)fontSize;
  uti::text::normalizeNewLines(value);
  std::istringstream stream(value);
  std::string originalLine;
  std::string newText;
  int lineCount = 0;
  bool firstLine = true;

  while (std::getline(stream, originalLine)) {
    if (!firstLine) newText += "\n";
    firstLine = false;

    std::istringstream words(originalLine);
    std::string word;
    std::string line;

    while (words >> word) {
      std::string testLine = line.empty() ? word : line + " " + word;
      float testWidth = MeasureTextEx(font, testLine.c_str(), fontSize, 1.0f).x;

      if (testWidth > maxWidth && !line.empty()) {
        newText += line + "\n";
        line = word;
        lineCount++;

        if (wrapType == CUT && lineCount * lineHeight > maxHeight) {
          value = newText;
          return;
        }
      } else {
        line = testLine;
      }
    }

    if (!line.empty()) {
      newText += line;
      lineCount++;
    }
  }

  value = newText;

  if (wrapType == EXPAND) {
    uiObject_parent->size.y =
        lineCount * lineHeight + 2 * margin + lineHeight * 0.65f;
  }

  for (unsigned char c : value) {
    std::cout << (int)c << " ";
  }
  std::cout << std::endl;
}

float Text::getPixelWidth() {
  return (float)MeasureText(value.c_str(), fontSize);
}


void Text::draw() {
  if (uiObject_parent == nullptr) return;

  Vec2f finalPos = uiObject_parent->position;
  Vec2f finalSize = uiObject_parent->size;

  int textWidth = MeasureText(value.c_str(), fontSize);
  int textHeight = fontSize;

  float x = finalPos.x + margin;
  float y = finalPos.y + margin;

  if (center) {
    x = finalPos.x + (finalSize.x - textWidth) / 2.0f;
    y = finalPos.y + (finalSize.y - textHeight) / 2.0f;
  }

  BeginScissorMode(
      (int)finalPos.x,
      (int)finalPos.y,
      (int)finalSize.x,
      (int)finalSize.y
  );

  DrawTextEx(App::instance().getAppData().defaultFont, value.c_str(), { x, y }, (float)fontSize, 1.0f, textColor);

  EndScissorMode();
}
