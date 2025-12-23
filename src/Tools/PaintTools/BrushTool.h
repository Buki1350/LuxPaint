#pragma once

class BrushTool final : public Tool {
protected:
  bool canSizeBeChanged() const override { return true; };

public:
  explicit BrushTool(const std::string& name) : Tool(name) {}

  void _handleMousePressedImpl(UIObject* img) override {
    _holdFrames = 0;
    Draw(img);
  }

  void _handleMouseDownImpl(UIObject* img) override {
    _holdFrames++;
    Draw(img);
  }

  void _handleMouseReleaseImpl(UIObject *img) override {
    _holdFrames = 0;
  }

private:
  int _holdFrames = 0;
  static constexpr int MAX_EXTRA_RADIUS = 10;
  static constexpr float GROW_SPEED = 0.25f;

  void Draw(UIObject* img) {
    if (!img) return;

    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) return;

    Image im = img->getImage();
    Color c = App::instance->canvas.getCurrentColor();

    int base = std::max(size, 1);
    int extra = std::min(
      (int)(_holdFrames * GROW_SPEED),
      MAX_EXTRA_RADIUS
    );

    int radius = base + extra;

    ImageDrawCircle(&im, p.x, p.y, radius, c);
    img->updateTexture();
  }
};
