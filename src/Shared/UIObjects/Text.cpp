//
// Created by lukas on 8.09.2025.
//

#include "Text.h"
#include "Shared/Utils/AppData/utiAppData.h"
#include "UIObject.h"

#include <iostream>
void Text::setParent(UIObject *parent) {
  uiObject_parent = parent;
}

#include <sstream>

void Text::wrapToWidth(TextWrapType wrapType) {
  if (!uiObject_parent) return;

  Font font = uti::appdata::getDefaultFont();
  float maxWidth = uiObject_parent->size.x - 2 * margin;
  float maxHeight = uiObject_parent->size.y - 2 * margin;

  std::istringstream words(value);
  std::string word;
  std::string newText;
  std::string line;

  int lineCount = 0;
  float lineHeight = (float)fontSize;

  while (words >> word) {
    std::string testLine = line.empty() ? word : line + " " + word;
    float testWidth = MeasureTextEx(font, testLine.c_str(), (float)fontSize, 1.0f).x;

    if (testWidth > maxWidth && !line.empty()) {
      newText += line + "\n";
      line = word;
      lineCount++;

      if (wrapType == CUT && (lineCount + 1) * lineHeight > maxHeight) {
        // cut
        break;
      }
    } else {
      line = testLine;
    }
  }

  if (!(wrapType == CUT && (lineCount + 1) * lineHeight > maxHeight)) {
    newText += line;
    lineCount++;
  }

  value = newText;

  if (wrapType == EXPAND) {
    uiObject_parent->size.y = lineCount * lineHeight + 2 * margin;
  }
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

  DrawTextEx(uti::appdata::getDefaultFont(), value.c_str(), { x, y }, (float)fontSize, 1.0f, textColor);

  EndScissorMode();
}
