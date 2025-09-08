#pragma once
#include "../../Render/UIObject.h"
#include "../../Updatables/Updatable.h"

class ColorHolder final : public Updatable {
  UIObject* _uiObject = nullptr;
  float _marginScale = 0.01f;
  float _widthScale = 0.15f;
  float _heightScale = 0.05f;
public:
  void Init();
  void Update() override;
};
