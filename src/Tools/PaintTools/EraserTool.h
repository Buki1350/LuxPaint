#pragma once
#include "Tools/Shared/Tool.h"

class EraserTool final : public Tool {
public:
  explicit EraserTool(const std::string& name) : Tool(name) {}

protected:
  bool canSizeBeChanged() const override { return true; };

  void _handleMousePressedImpl(UIObject* img) override { Erase(img); }
  void _handleMouseDownImpl(UIObject* img) override { Erase(img); }
  void _handleMouseReleaseImpl(UIObject*) override {}

private:
  void Erase(UIObject* img) {
    if (!img) return;
    Vec2i p = img->getOnImageCursorPosition();
    if (p.x < 0) return;

    Image im = img->getImage();
    Color transparent{0, 0, 0, 0};

    ImageDrawCircle(&im, p.x, p.y, std::max(size, 1), transparent);
    img->updateTexture();
  }
};
