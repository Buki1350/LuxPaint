#pragma once
#include "../Shared/Tool.h"
#include "../../App.h"
#include "../../StaticShared/Utils/Utils.h"

// prosty tool do zaznaczania prostokątnego
class RectangleSelection final : public Tool {

  bool _selecting = false;
  std::vector<UIObject*> _selections;
  UIObject* _currentSelection = nullptr;
  Vec2f _startPos{0, 0};
  Vec2f _endPos{0, 0};
  Vec2f _prevPos{0, 0};

  void _HandleMousePressed(UIObject* imageToPaint, Vec2f localPos) override;
  void _HandleMouseDown(UIObject* imageToPaint, Vec2f localPos) override;
  void _HandleMouseRelease(UIObject* imageToPaint, Vec2f localPos) override;

  Rectangle GetSelectionRect() const;

public:
  explicit RectangleSelection(const std::string& name) : Tool(name) {}
};
