#pragma once
#include "../Shared/Tool.h"
#include "../../App.h"
#include "../../StaticShared/Utils/Utils.h"

// prosty tool do zaznaczania prostokątnego
class RectangleSelection final : public Tool {
public:
  explicit RectangleSelection(const std::string& name) : Tool(name) {}

  bool selecting = false;
  Vec2f startPos{0, 0};
  Vec2f endPos{0, 0};

  // Start/drag - reagujemy na klik i trzymanie
  void HandleClick(UIObject* imageToPaint, Vec2f localPos) override;

  // Release - kończymy selekcję
  void HandleRelease(UIObject* imageToPaint, Vec2f localPos) override;

  Rectangle GetSelectionRect() const;
};
