#pragma once
#include "../../App/App.h"
#include "Math/Vec2.h"
#include "Tools/Shared/Tool.h"

class LineTool final : public Tool {
  Vec2i _start{-1, -1};

public:
  explicit LineTool(const std::string& name) : Tool(name) {}

protected:
  bool canSizeBeChanged() const override { return true; };

  void handleMousePressedImpl(UIObject* img) override {
    _start = img->getOnImageCursorPosition();
  }

  void handleMouseDownImpl(UIObject*) override {}

  void handleMouseReleaseImpl(UIObject* img) override {
    if (!img || _start.x < 0) return;

    Vec2i end = img->getOnImageCursorPosition();
    Image im = img->getImage();
    Color c = App::instance->canvas.getCurrentColor();

    int dx = end.x - _start.x;
    int dy = end.y - _start.y;
    int steps = std::max(abs(dx), abs(dy));
    int r = std::max(size, 1);

    for (int i = 0; i <= steps; i++) {
      float t = (float)i / steps;
      int x = _start.x + dx * t;
      int y = _start.y + dy * t;
      ImageDrawCircle(&im, x, y, r, c);
    }

    img->updateTexture();
  }
};
