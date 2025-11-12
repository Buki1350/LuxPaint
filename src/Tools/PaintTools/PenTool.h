#pragma once
#include "../Shared/Tool.h"

class PenTool final : public Tool {
  Vec2f _lastPos{-1, -1};
public:
  explicit PenTool(const std::string& name) : Tool(name) {}

  void _HandleMousePressed(UIObject* imageToPaint, Vec2f vec2) override { }

  void _HandleMouseDown(UIObject *imageToPaint, Vec2f localPos) override {
    if (!imageToPaint) return;


      auto DrawPixel = [&](Vec2f pos) {
        float relX = pos.x / imageToPaint->size.x;
        float relY = pos.y / imageToPaint->size.y;

        int imgX = (int)(relX * imageToPaint->GetImageSize().x);
        int imgY = (int)(relY * imageToPaint->GetImageSize().y);

        if (imgX < 0 || imgY < 0 || imgX >= imageToPaint->GetImageSize().x ||
            imgY >= imageToPaint->GetImageSize().y)
          return;

        ImageDrawPixel(&imageToPaint->GetImage(), imgX, imgY,
                       App::Instance->canvas.GetCurrentColor());
      };

        if (_lastPos.x >= 0 && _lastPos.y >= 0) {
          // Interpolacja liniowa między punktami
          Vec2f delta = localPos - _lastPos;
          float distance = delta.Magnitude();
          int steps = (int)distance;

          for (int i = 0; i <= steps; i++) {
            Vec2f interp = _lastPos + delta * ((float)i / (float)steps);
            DrawPixel(interp);
          }
        }
        else {
          DrawPixel(localPos);
        }

    imageToPaint->UpdateTexture();
    _lastPos = localPos;

  }

  void _HandleMouseRelease(UIObject *imageToPaint, Vec2f vec2) override {
    _lastPos = {-1, -1};
  }
};
