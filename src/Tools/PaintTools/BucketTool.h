#pragma once
#include "../Shared/Tool.h"
#include <queue>

class BucketTool final : public Tool {
public:
    explicit BucketTool(const std::string& name) : Tool(name) {}

protected:
  bool canSizeBeChanged() const override { return false; };

    void handleMousePressedImpl(UIObject* img) override {

        Vec2i p = img->getOnImageCursorPosition();
        if (p.x < 0)
          return;

        Image image = img->getImage();

        Color target = GetPixelUnsafe(image, p.x, p.y);
        Color replacement = App::instance().canvas.getCurrentColor();

        if (ColorsEqual(target, replacement)) return;

        FloodFill(image, p.x, p.y, target, replacement);
        img->updateTexture();
    }


    void handleMouseDownImpl(UIObject* imageToPaint) override {}
    void handleMouseReleaseImpl(UIObject* imageToPaint) override {}

private:

    // *** Pobieranie koloru piksela (bez zabezpieczeń, bo wcześniej sprawdzam bounds) ***
    inline Color GetPixelUnsafe(const Image& img, int x, int y) {
        Color* data = (Color*)img.data;
        return data[y * img.width + x];
    }

    // *** Ustawianie koloru piksela ***
    inline void SetPixelUnsafe(Image& img, int x, int y, Color c) {
        Color* data = (Color*)img.data;
        data[y * img.width + x] = c;
    }

    // *** Porównanie kolorów (Raylib ma słaby ColorEquals) ***
    inline bool ColorsEqual(const Color& a, const Color& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }

    // *** Właściwy flood fill ***
    void FloodFill(Image &img, int startX, int startY, Color target,
                   Color repl) {

      int w = img.width;
      int h = img.height;

      std::queue<std::pair<int, int>> q;
      q.emplace(startX, startY);
      SetPixelUnsafe(img, startX, startY, repl);

      while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // sąsiedzi 4-kierunkowi
        constexpr int OFF[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (int i = 0; i < 4; i++) {
          int nx = x + OFF[i][0];
          int ny = y + OFF[i][1];

          if (nx < 0 || ny < 0 || nx >= w || ny >= h)
            continue;

          if (ColorsEqual(GetPixelUnsafe(img, nx, ny), target)) {
            SetPixelUnsafe(img, nx, ny, repl);
            q.emplace(nx, ny);
          }
        }
      }
    }
};
