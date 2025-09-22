#pragma once
#include "../../Render/UIObjectManager.h"
#include "../../StaticShared/FrontPanel/FrontPanel.h"
class BlankCanvasBuilder {
public:
  static void Build() {
    FrontPanel* canvasCreator = FrontPanel::CreateInstance();
    auto obj = UIObjectManager::CreateUIObject();
    obj->size = {500, 500};
    obj->position = (Utils::GetWindowSize().CastTo<float>() - obj->size) / 2;
    obj->roundness = .05f;
    canvasCreator->Pack(obj);
  }
};