#pragma once
#include "../Shared/Tool.h"

class PenTool final : public Tool {
  Vec2i _lastPixel{-1, -1};

public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void _handleMousePressedImpl(UIObject* img) override {
    _lastPixel = {-1, -1};
  }

  void _handleMouseDownImpl(UIObject* img) override {
    if (!img) return;

    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) return;

    Image im = img->getImage();
    Color col = App::instance->canvas.getCurrentColor();
    int radius = std::max(size, 1);   // grubość linii

    if (_lastPixel.x >= 0) {
      int dx = p.x - _lastPixel.x;
      int dy = p.y - _lastPixel.y;
      int steps = std::max(abs(dx), abs(dy));
      if (steps == 0) steps = 1;

      for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        int x = (int)(_lastPixel.x + dx * t);
        int y = (int)(_lastPixel.y + dy * t);

        ImageDrawCircle(&im, x, y, radius, col);
      }
    }
    else {
      ImageDrawCircle(&im, p.x, p.y, radius, col);
    }

    img->updateTexture();
    _lastPixel = p;
  }

  void _handleMouseReleaseImpl(UIObject *img) override {
    _lastPixel = {-1, -1};
  }

protected:
  bool canSizeBeChanged() const override { return true; };
};
