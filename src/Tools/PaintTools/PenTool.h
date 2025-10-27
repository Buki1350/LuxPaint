#pragma once
#include "../Shared/Tool.h"

class PenTool final : public Tool {
  public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void PenTool::Apply(UIObject* imageToPaint, Vec2f localPos) override {
    if (!imageToPaint) return;

    float relX = localPos.x / imageToPaint->size.x;
    float relY = localPos.y / imageToPaint->size.y;

    int imgX = (int)(relX * imageToPaint->GetImageSize().x);
    int imgY = (int)(relY * imageToPaint->GetImageSize().y);

    if (imgX < 0 || imgY < 0 ||
        imgX >= imageToPaint->GetImageSize().x ||
        imgY >= imageToPaint->GetImageSize().y)
      return;

    ImageDrawPixel(&imageToPaint->GetImage(), imgX, imgY, App::Instance->canvas.GetCurrentColor());

    imageToPaint->UpdateTexture();
  }

};