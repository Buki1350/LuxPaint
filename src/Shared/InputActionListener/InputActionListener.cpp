#include "InputActionListener.h"
#include "../../App/App.h"
#include "../../UI/MiniMenus/BlankCanvas/BlankCanvasBuilder.h"
#include "../Keybindings/Keybindings.h"

void InputActionListener::init() {

}

void InputActionListener::update() {
  InputAction action = Keybindings::getDetectedAction();

#ifdef DEBUG
  handleDebug(action);
#endif

  switch (action) {
  case UNDO: App::instance().snapshotManager.undo(); break;
  case REDO: App::instance().snapshotManager.redo(); break;
  default: break;
  }
}

#ifdef DEBUG
void InputActionListener::handleDebug(InputAction action) {
  switch (action) {
  case DEBUG_CREATEIMAGE_250x250:
    BlankCanvasBuilder::CreateBlank(Vec2i(250, 250));
    break;
  default:
    break;
  }
}
#endif

