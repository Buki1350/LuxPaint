#include "utiAppData.h"

#include "../../../App/App.h"

namespace uti::appdata {

Font getDefaultFont() {
  return App::instance->getAppData().defaultFont;
}

}