#pragma once
#include "../Shared/Tool.h"
#include <algorithm>

class RectangleTool final : public Tool {
  Vec2i _start{-1, -1};
  bool _active = false;

public:
  explicit RectangleTool(const std::string& name) : Tool(name) {}

protected:
  bool CanSizeBeChanged() const override { return true; };

  void HandleMousePressed(UIObject* img) override {
    Vec2i p = img->GetOnImageCursorPosition();
    if (p.x < 0) return;

    _start = p;
    _active = true;
  }

  void HandleMouseDown(UIObject* imageToPaint) override {
    // celowo puste — prostokąt rysujemy dopiero na mouse release
  }

  void HandleMouseRelease(UIObject *img) override {
    if (!_active || !img)
      return;

    Vec2i end = img->GetOnImageCursorPosition();
    if (end.x < 0) {
      _active = false;
      return;
    }

    Image image = img->GetImage();
    Color c = App::Instance->canvas.GetCurrentColor();

    int x1 = std::min(_start.x, end.x);
    int y1 = std::min(_start.y, end.y);
    int x2 = std::max(_start.x, end.x);
    int y2 = std::max(_start.y, end.y);

    // rysujemy krawędzie prostokąta
    for (int x = x1; x <= x2; x++) {
      ImageDrawPixel(&image, x, y1, c);
      ImageDrawPixel(&image, x, y2, c);
    }
    for (int y = y1; y <= y2; y++) {
      ImageDrawPixel(&image, x1, y, c);
      ImageDrawPixel(&image, x2, y, c);
    }

    img->UpdateTexture();
    _active = false;
  }

};
