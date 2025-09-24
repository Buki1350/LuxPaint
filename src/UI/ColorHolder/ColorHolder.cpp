//
// Created by lukas on 5.09.2025.
//

#include "ColorHolder.h"

#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Utils/Utils.h"

void ColorHolder::Init() {
  _uiObject = UIObjectsManager::Create();
  _uiObject->color = Utils::LoadColor("colorHolderButton");
  _uiObject->roundness = 0.3f;
}

void ColorHolder::Update() {
  Vec2i monitorSize = Utils::GetCurrentMonitorSize();
  Vec2i windowSize = Utils::GetWindowSize();
  float width = monitorSize.x * _widthScale;
  float height = monitorSize.y * _heightScale;

  _uiObject->size = {width, height};
  _uiObject->position = {
    (float)monitorSize.x * _marginScale,
    (float)windowSize.y - height - monitorSize.y * _marginScale
  };
}