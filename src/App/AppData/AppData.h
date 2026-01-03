#pragma once
#include "Shared/Utils/Files/utiFiles.h"

class AppData final {
  friend class App;
  void init();
  void updateValues();
  void saveValues();
public:
  Font defaultFont = {};
  Vec2i monitorSize = Vec2i::zero();
  Vec2i windowSize = Vec2i::zero();;
  float smallerMonitorEdge = 0;
  Color appBackgroundColor = {};
};

