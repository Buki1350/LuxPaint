#include "utiView.h"

#include "App/App.h"

namespace uti::view {

Vec2i getCurrentMonitorSize() {
  return App::instance->getAppData().monitorSize;
}

Vec2i getWindowSize() {
  return App::instance->getAppData().windowSize;
}

float getSmallerMonitorEdge() {
  return App::instance->getAppData().smallerMonitorEdge;
}

}