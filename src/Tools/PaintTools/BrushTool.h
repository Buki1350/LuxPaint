#pragma once
#include "../Shared/Tool.h"

class BrushTool final : public Tool {
protected:
  bool CanSizeBeChanged() const override { return true; };

public:
  explicit BrushTool(const std::string& name) : Tool(name) {}

  void HandleMousePressed(UIObject* img) override {
    Draw(img);
  }

  void HandleMouseDown(UIObject* img) override {
    Draw(img);
  }

  void HandleMouseRelease(UIObject *img) override {}

private:
  void Draw(UIObject* img) {
    if (!img) return;
    Vec2i p = img->GetOnImageCursorPosition();
    if (p.x < 0) return;

    Image im = img->GetImage();
    Color c = App::Instance->canvas.GetCurrentColor();

    int radius = std::max(size, 1);

    ImageDrawCircle(&im, p.x, p.y, radius, c);

    img->UpdateTexture();
  }
};
