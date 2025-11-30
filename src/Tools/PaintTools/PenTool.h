#pragma once
#include "../Shared/Tool.h"

class PenTool final : public Tool {
  Vec2i _lastPixel{-1, -1};

public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void HandleMousePressed(UIObject* imageToPaint) override {
    _lastPixel = {-1, -1};
  }

  void HandleMouseDown(UIObject* imageToPaint) override {
    if (!imageToPaint) return;

    Vec2i p = imageToPaint->GetOnImageCursorPosition();
    if (p.x < 0) return; // poza obrazem

    Image img = imageToPaint->GetImage();
    Color color = App::Instance->canvas.GetCurrentColor();

    // Jeśli mamy poprzedni punkt — rysujemy linię interpolowaną
    if (_lastPixel.x >= 0) {
      int dx = p.x - _lastPixel.x;
      int dy = p.y - _lastPixel.y;

      int steps = std::max(abs(dx), abs(dy));
      if (steps == 0) steps = 1;

      for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        int x = (int)(_lastPixel.x + dx * t);
        int y = (int)(_lastPixel.y + dy * t);
        ImageDrawPixel(&img, x, y, color);
      }
    }
    else {
      ImageDrawPixel(&img, p.x, p.y, color);
    }

    imageToPaint->UpdateTexture();
    _lastPixel = p;
  }

  void HandleMouseRelease(UIObject *imageToPaint) override {
    _lastPixel = {-1, -1};
  }

protected:
  bool CanSizeBeChanged() const override { return true; };
};
