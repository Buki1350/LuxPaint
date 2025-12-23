#include "utiTime.h"

#include "App/App.h"

namespace uti::time {
  float getDeltaTime() {
    return GetFrameTime();
  }
}