#pragma once
#include "../Shared/Tool.h"

class BrushTool final : public Tool {
protected:
  bool CanSizeBeChanged() const override { return true; };

public:
  explicit BrushTool(const std::string& name) : Tool(name) {}

  void BrushTool::HandleMousePressed(UIObject* img) override {
    Vec2i p = img->GetOnImageCursorPosition();
    if (p.x < 0) return;

    ImageDrawCircle(&img->GetImage(), p.x, p.y, 5, BLUE);
    img->UpdateTexture();
  }


  void HandleMouseDown(UIObject *imageToPaint) override { }

  void HandleMouseRelease(UIObject *imageToPaint) override {}

};