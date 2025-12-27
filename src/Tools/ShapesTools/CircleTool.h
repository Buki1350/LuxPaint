#pragma once
#include "../Shared/Tool.h"
#include <cmath>

class CircleTool final : public Tool {
  Vec2i _start{-1, -1};
  bool _active = false;

public:
  explicit CircleTool(const std::string& name) : Tool(name) {}

protected:
  bool canSizeBeChanged() const override { return true; };

  void handleMousePressedImpl(UIObject* img) override {
    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) return;

    _start = p;
    _active = true;
  }

  void handleMouseDownImpl(UIObject* imageToPaint) override {
    // celowo puste — okrąg rysujemy dopiero na release
  }

  void handleMouseReleaseImpl(UIObject *img) override {
    if (!_active || !img)
      return;

    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) {
      _active = false;
      return;
    }

    Image image = img->getImage();
    Color c = App::instance->canvas.getCurrentColor();

    int dx = p.x - _start.x;
    int dy = p.y - _start.y;
    int r = (int)std::sqrt(dx * dx + dy * dy);

    // Midpoint circle algorithm (ładne, równe okręgi)
    int x = r;
    int y = 0;
    int err = 1 - x;

    while (x >= y) {
      auto px = [&](int ox, int oy) {
        int thickness = std::max(size, 1);
        for (int t = 0; t < thickness; t++) {
          ImageDrawCircle(
            &image,
            _start.x + ox,
            _start.y + oy,
            t,
            c
          );
        }
      };


      px(x, y);
      px(y, x);
      px(-y, x);
      px(-x, y);
      px(-x, -y);
      px(-y, -x);
      px(y, -x);
      px(x, -y);

      y++;
      if (err < 0)
        err += 2 * y + 1;
      else {
        x--;
        err += 2 * (y - x + 1);
      }
    }

    img->updateTexture();
    _active = false;
  }
};
