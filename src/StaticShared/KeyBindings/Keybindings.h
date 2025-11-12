#pragma once
#include <map>
#include <vector>
#include <unordered_map>
#include "raylib.h"

enum InputAction {
    IA_NONE,
    MENU_NEXT,
    MENU_PREV,
    MENU_CONFIRM,
    MODE_ADD,
    // ... 1
    DEBUG_CREATEIMAGE_250x250
};

class Keybindings {
    // ... multiple keys to allow combinations
    inline static std::map<InputAction, std::vector<KeyboardKey>> actionsDictionary;
    inline static std::unordered_map<KeyboardKey, float> _blockSingleKey;
    inline static InputAction __previouslyInvokedAction = IA_NONE;
    inline static std::vector<KeyboardKey> _pushedKeys;

    static void _Load();
public:
    static bool ActionDetected(InputAction action);
};