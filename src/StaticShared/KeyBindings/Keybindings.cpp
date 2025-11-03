#include "Keybindings.h"

Keybindings::Keybindings() {
    //TODO keybindings from file system

    actionsDictionary[MENU_NEXT] = { KEY_TAB };
    actionsDictionary[MENU_PREV] = { KEY_LEFT_SHIFT, KEY_TAB };
    actionsDictionary[MENU_CONFIRM] = { KEY_ENTER };

}

bool Keybindings::ActionDetected(InputAction action) {
    const auto& keys = actionsDictionary[action];

    if (keys.size() > 1) {
        for (size_t i = 0; i < keys.size() - 1; i++) {
            if (!IsKeyDown(keys[i])) return false;
        }
        return IsKeyPressed(keys.back());
    }

    return IsKeyPressed(keys[0]);
}

