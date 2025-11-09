#pragma once
#include "MiniMenu.h"
#include "../../StaticShared/Utils/Utils.h"

class MiniMenuBuilder {
  public:
  inline static MiniMenu* menuInstance = nullptr;
  static void Build() {
    if (menuInstance != nullptr) return;
    menuInstance = MiniMenu::CreateInstance();
    menuInstance->OnDestroy([](){menuInstance = nullptr;});
    menuInstance->oBackground->roundness = Utils::GetSmallerMonitorEdge() * 0.00001f;
  }
};