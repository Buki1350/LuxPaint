#include "InputManager.h"
#include "../../App.h"
#include "../../UI/ManagerButton/BlankCanvasBuilder.h"
#include "../KeyBindings/Keybindings.h"

static InputManager singleton;

void InputManager::Update() {
#ifdef DEBUG
  UpdateDebug();
#endif
}

#ifdef DEBUG
void InputManager::UpdateDebug() {
  if (Keybindings::ActionDetected(DEBUG_CREATEIMAGE_250x250)) { BlankCanvasBuilder::CreateBlank(Vec2i(250, 250)); }
}
#endif
