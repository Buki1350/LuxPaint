#pragma once

#include "../../Render/UIObject.h"

#include <vector>

class FrontPanel {
  UIObject *_oBackground = nullptr;

  public:
  static std::vector<FrontPanel*> Instances;
  static FrontPanel* CreateInstance();

  FrontPanel* Pack(UIObject *object);
};