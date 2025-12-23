#pragma once
#include "../Shared/Tool.h"
#include <algorithm>

class RectangleTool final : public Tool {
  Vec2i _start{-1, -1};
  bool _active = false;

public:
  explicit RectangleTool(const std::string& name) : Tool(name) {}

protected:
  bool canSizeBeChanged() const override { return true; };

  void _handleMousePressedImpl(UIObject* img) override {
    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) return;

    _start = p;
    _active = true;
  }

  void _handleMouseDownImpl(UIObject* imageToPaint) override {
    // celowo puste — prostokąt rysujemy dopiero na mouse release
  }

  void _handleMouseReleaseImpl(UIObject *img) override {
    if (!_active || !img)
      return;

    Vec2i end = img->getOnImageCursorPosition();
    if (end.x < 0) {
      _active = false;
      return;
    }

    Image image = img->getImage();
    Color c = App::instance->canvas.getCurrentColor();

    int x1 = std::min(_start.x, end.x);
    int y1 = std::min(_start.y, end.y);
    int x2 = std::max(_start.x, end.x);
    int y2 = std::max(_start.y, end.y);

    int thickness = std::max(size, 1);

    for (int t = 0; t < thickness; t++) {
      for (int x = x1; x <= x2; x++) {
        ImageDrawPixel(&image, x, y1 + t, c);
        ImageDrawPixel(&image, x, y2 - t, c);
      }
      for (int y = y1; y <= y2; y++) {
        ImageDrawPixel(&image, x1 + t, y, c);
        ImageDrawPixel(&image, x2 - t, y, c);
      }
    }

    img->updateTexture();
    _active = false;
  }

};
