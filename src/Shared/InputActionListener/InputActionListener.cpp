#include "InputActionListener.h"
#include "../../App/App.h"
#include "../../UI/MiniMenus/BlankCanvas/BlankCanvasBuilder.h"
#include "../Keybindings/Keybindings.h"

static InputActionListener singleton;

void InputActionListener::update() {
#ifdef DEBUG
  updateDebug();
#endif

  if (Keybindings::actionDetected(UNDO)) { App::instance().snapshotManager.undo(); }
  if (Keybindings::actionDetected(REDO)) { App::instance().snapshotManager.redo(); }
}

#ifdef DEBUG
void InputActionListener::updateDebug() {
  if (Keybindings::actionDetected(DEBUG_CREATEIMAGE_250x250)) { BlankCanvasBuilder::CreateBlank(Vec2i(250, 250)); }
}
#endif
