#include "AppData.h"

#include "Shared/Serializer/Serializer.h"

void AppData::_init() {
  defaultFont = LoadFont((std::string(PATH_FONTS) + "Comfortaa-Bold.ttf").c_str());
  appBackgroundColor = uti::files::loadColor("appBackground");
}

void AppData::_updateValues() {
  Vec2i currentMonitorSize = Vec2i(GetMonitorWidth(GetCurrentMonitor()),
                                   GetMonitorHeight(GetCurrentMonitor()));
  Vec2i currentWindowSize = Vec2i(GetScreenWidth(), GetScreenHeight());

  monitorSize = currentMonitorSize;
  smallerMonitorEdge = std::min(currentMonitorSize.x, currentMonitorSize.y);
  windowSize = currentWindowSize;
}