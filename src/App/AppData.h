#pragma once
#include "Shared/Utils/Files/utiFiles.h"

struct AppData {
  Font defaultFont = {};
  Vec2i monitorSize;
  Vec2i windowSize;
  float smallerMonitorEdge = 0;
};