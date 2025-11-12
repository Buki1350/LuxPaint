//
// Created by lukas on 8.09.2025.
//

#include "Text.h"
#include "../StaticShared/Utils/Utils.h"
#include "UIObject.h"

#include <iostream>
void Text::SetParent(UIObject *parent) { uiObject_parent = parent; }

float Text::GetPixelWidth() {
  return (float)MeasureText(value.c_str(), fontSize);
}

void Text::Draw() {
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

  DrawTextEx(Utils::AppData::GetDefaultFont(), value.c_str(), { x, y }, (float)fontSize, 1.0f, textColor);

  EndScissorMode();
}
