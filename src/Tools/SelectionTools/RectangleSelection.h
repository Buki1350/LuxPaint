#pragma once
#include "../Shared/Tool.h"

class RectangleSelection final : public Tool {

  bool _selecting = false;
  std::vector<UIObject*> _selections;
  UIObject* _currentSelection = nullptr;

  Vec2f _startPos{0, 0};
  Vec2f _endPos{0, 0};

  Rectangle GetSelectionRect() const;

public:
  bool canSizeBeChanged() const override { return false; }
  explicit RectangleSelection(const std::string& name) : Tool(name) {}

  void _handleMousePressedImpl(UIObject* imageToPaint) override;
  void _handleMouseDownImpl(UIObject* imageToPaint) override;
  void _handleMouseReleaseImpl(UIObject *imageToPaint) override;
};
