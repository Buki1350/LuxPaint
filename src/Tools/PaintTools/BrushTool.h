#pragma once
#include "../Shared/Tool.h"

class BrushTool final : public Tool {
public:
    explicit BrushTool(const std::string& name) : Tool(name) {}

    void BrushTool::Apply(UIObject* imageToPaint, Vec2f localPos) override {
        if (!imageToPaint) return;

        float relX = localPos.x / imageToPaint->size.x;
        float relY = localPos.y / imageToPaint->size.y;

        int imgX = (int)(relX * imageToPaint->GetImageSize().x);
        int imgY = (int)(relY * imageToPaint->GetImageSize().y);

        int radius = 5;
        ImageDrawCircle(&imageToPaint->GetImage(), imgX, imgY, radius, BLUE);

        imageToPaint->UpdateTexture();
    }
};