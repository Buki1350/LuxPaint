#include "AppData.h"

#include "Shared/Serializer/Serializer.h"

void AppData::init() {
  defaultFont = LoadFont((std::string(PATH_FONTS) + "Comfortaa-Bold.ttf").c_str());
  appBackgroundColor = uti::files::loadColor("appBackground");
}

void AppData::updateValues() {
  Vec2i currentMonitorSize = Vec2i(GetMonitorWidth(GetCurrentMonitor()),
                                   GetMonitorHeight(GetCurrentMonitor()));
  Vec2i currentWindowSize = Vec2i(GetScreenWidth(), GetScreenHeight());

  monitorSize = currentMonitorSize;
  smallerMonitorEdge = std::min(currentMonitorSize.x, currentMonitorSize.y);
  windowSize = currentWindowSize;
}

void AppData::saveValues() {
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  int windowX = (int)GetWindowPosition().x;
  int windowY = (int)GetWindowPosition().y;

  Serializer::saveArray<int>("UserPreferences.dat", "windowSize", {screenWidth, screenHeight});
  Serializer::saveArray<int>("UserPreferences.dat", "windowPosition", {windowX, windowY});
}