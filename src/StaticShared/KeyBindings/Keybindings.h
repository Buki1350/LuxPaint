#pragma once
#include <map>
#include <vector>

#include "raylib.h"

enum InputAction {
    MENU_NEXT,
    MENU_PREV,
    MENU_CONFIRM,
};

class Keybindings {
    // ... multiple keys to allow combinations
    inline static std::map<InputAction, std::vector<KeyboardKey>> actionsDictionary;

public:
    Keybindings();
    static bool ActionDetected(InputAction action);
};