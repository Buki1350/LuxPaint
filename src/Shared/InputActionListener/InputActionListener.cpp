#include "InputActionListener.h"
#include "../../App/App.h"
#include "../../UI/MiniMenus/BlankCanvas/BlankCanvasBuilder.h"
#include "../Keybindings/Keybindings.h"

void InputActionListener::init() {

}

void InputActionListener::update() {
#ifdef DEBUG
  updateDebug();
#endif

  switch (Keybindings::getDetectedAction()) {
    case UNDO: App::instance().snapshotManager.undo(); break;
    case REDO: App::instance().snapshotManager.redo(); break;
    default: break;
  }
}

#ifdef DEBUG
void InputActionListener::updateDebug() {
  switch (Keybindings::getDetectedAction()) {
  case DEBUG_CREATEIMAGE_250x250:
    BlankCanvasBuilder::CreateBlank(Vec2i(250, 250));
    break;
  default:
    break;
  }
}
#endif
