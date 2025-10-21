#pragma once
#include "../Shared/Tool.h"

class PenTool final : public Tool {
  public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void PenTool::Apply(UIObject* imageToPaint, Vec2f localPos) override {
    if (!imageToPaint) return;

    // oblicz lokalne współrzędne w obrazie
    float relX = localPos.x / imageToPaint->size.x;
    float relY = localPos.y / imageToPaint->size.y;

    int imgX = (int)(relX * imageToPaint->GetImageSize().x);
    int imgY = (int)(relY * imageToPaint->GetImageSize().y);

    // bezpieczeństwo
    if (imgX < 0 || imgY < 0 ||
        imgX >= imageToPaint->GetImageSize().x ||
        imgY >= imageToPaint->GetImageSize().y)
      return;

    // rysowanie np. czerwonego punktu
    ImageDrawPixel(&imageToPaint->GetImage(), imgX, imgY, RED);

    imageToPaint->UpdateTexture();
  }

};