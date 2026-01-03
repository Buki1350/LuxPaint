#pragma once
#include "raylib.h"
#include <map>
#include <unordered_map>
#include <vector>

#include <string>

enum InputAction {
    IA_NONE,
    MENU_NEXT,
    MENU_PREV,
    MENU_CONFIRM,
    MODE_ADD,
    UNDO,
    REDO,
    DEBUG_CREATEIMAGE_250x250,
    UNKNOWN
};

class Keybindings {
    // ... for serialization
    struct KeyWithName {
        KeyboardKey key = KEY_NULL;
        std::string name = "";
    };

    struct InputActionWithString {
        InputAction action = InputAction::UNKNOWN;
        std::string name = "";
    };

    inline static std::vector<KeyWithName> _keysWithNames;
    inline static std::vector<InputActionWithString> _inputActionsWithStrings;

    inline static std::map<InputAction, std::vector<KeyboardKey>> actionsDictionary;
    inline static std::unordered_map<KeyboardKey, float> _blockSingleKey;
    inline static std::vector<KeyboardKey> _pushedKeys;

    static void load();
    static void assignActionsNames();
    static void assignKeyNames();
public:
    static InputAction getDetectedAction();
    static std::vector<KeyboardKey> getAllPushedKeys();
    static std::map<InputAction, std::vector<KeyboardKey>> getDefaultBindings();
    static std::vector<Keybindings::InputActionWithString> getActionsNames();
    static std::vector<Keybindings::KeyWithName> getKeyNames();
    static std::string actionToString(InputAction action);
    static InputAction actionFromString(std::string actionName);
    static std::string keyboardKeyToString(KeyboardKey key);
    static KeyboardKey stringToKeyboardKey(const std::string &name);
};