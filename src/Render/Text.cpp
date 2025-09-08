//
// Created by lukas on 8.09.2025.
//

#include "Text.h"
#include "UIObject.h"
#include "../Static/Utils/Utils.h"

#include <__msvc_ostream.hpp>
#include <iostream>
void Text::SetParent(UIObject *parent) {
  uiObject_parent = parent;
}

void Text::Draw() {
  if (uiObject_parent == nullptr) return;

  Vector2 finalPos = uiObject_parent->GetAnimatedPosition();
  Vector2 finalSize = uiObject_parent->GetAnimatedSize();

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

  DrawTextEx(Utils::GetDefaultFont(), value.c_str(), { x, y }, (float)fontSize, 1.0f, textColor);

  EndScissorMode();
}
