#include "Keybindings.h"

#include "Shared/Utils/Files/utiFiles.h"

#include <iostream>

void Keybindings::_load() {
  actionsDictionary = uti::files::loadActions();
  _createKeyNames();
  _createActionNames();
}

std::map<InputAction, std::vector<KeyboardKey>>Keybindings::getDefaultBindings() {
  return {
      {MENU_NEXT,                 {KEY_TAB}},
      {MENU_PREV,                 {KEY_LEFT_SHIFT, KEY_TAB}},
      {MENU_PREV,                 {KEY_ENTER}},
      {MODE_ADD,                  {KEY_LEFT_SHIFT}},
      {UNDO,                      {KEY_LEFT_CONTROL, KEY_Z}},
      {REDO,                      {KEY_LEFT_CONTROL, KEY_LEFT_SHIFT, KEY_Z}},
      {DEBUG_CREATEIMAGE_250x250, {KEY_F5}}
  };
}

void Keybindings::_createActionNames() {
  _inputActionsWithNames = std::vector<InputActionWithName> {
        {MENU_NEXT,   "MENU_NEXT"},
        {MENU_PREV,   "MENU_PREV"},
        {MENU_PREV,   "MENU_PREV"},
        {MODE_ADD,    "MENU_PREV"},
        {UNDO,        "UNDO"},
        {REDO,        "REDO"},
        {DEBUG_CREATEIMAGE_250x250, "DEBUG_CREATEIMAGE_250x250"}
  };
}

void Keybindings::_createKeyNames() {
  _keysWithNames = std::vector<KeyWithName> {
    {KEY_TAB,           "TAB"},
    {KEY_LEFT_SHIFT,    "LEFT_SHIFT"},
    {KEY_ENTER,         "ENTER"},
    {KEY_F5,            "F5"},
    {KEY_F6,            "F6"},
    {KEY_F7,            "F7"},
    {KEY_F8,            " F8"},
    {KEY_F9,            "F9"},
    {KEY_Z,             "Z"},
    {KEY_X,             "X"},
    {KEY_LEFT_CONTROL,  "LEFT_CONTROL"},
  };
}

std::string Keybindings::actionToString(InputAction action) {
  for (auto iaWithName : _inputActionsWithNames) {
    if (iaWithName.action == action)
      return iaWithName.name;
  }
  return "UNKNOWN";
}

InputAction Keybindings::actionFromString(std::string actionName) {
  for (auto iaWithName : _inputActionsWithNames) {
    if (iaWithName.name == actionName)
      return iaWithName.action;
  }
  return InputAction::UNKNOWN;
}

std::string Keybindings::keyboardKeyToString(KeyboardKey key) {
  for (auto keyWithName : _keysWithNames) {
    if (keyWithName.key == key)
      return keyWithName.name;
  }
  return "UNKNOWN";
}

KeyboardKey Keybindings::stringToKeyboardKey(const std::string& name)
{
  for (auto keyWithName : _keysWithNames) {
    if (keyWithName.name == name)
      return keyWithName.key;
  }
  return KEY_NULL;
}

bool Keybindings::actionDetected(InputAction action) {
  if (actionsDictionary.empty()) {
    _load();
  }

  _pushedKeys.clear();

  for (int k = KEY_NULL; k < KEY_KB_MENU; k++) {
    if (IsKeyDown(k)) {
      _pushedKeys.insert(_pushedKeys.begin(), (KeyboardKey)k);
    }
  }

  if (_pushedKeys.empty())
    return false;

  bool match = true;
  InputAction foundAction = IA_NONE;
  auto actDictCopy = actionsDictionary; // ## REMOVE ###################################################################################
  for (auto e : actionsDictionary) {

    if (e.second.size() != _pushedKeys.size()) continue;
    match = true;

    for (int i = 0; i < e.second.size(); i++) {
      if (e.second[i] != _pushedKeys[i]) {
        match = false;
        break;
      }
    }

    //if (match && e.first == action) {
    if (match && e.first == action) {
      foundAction = action;
      break;
    }
  }

  static InputAction previouslyInvokedAction = IA_NONE;
  static std::vector<KeyboardKey> previouslyPushedKeys = std::vector<KeyboardKey>();

  if (previouslyPushedKeys.size() > _pushedKeys.size()) {
    previouslyInvokedAction = IA_NONE;
  }
  previouslyPushedKeys = _pushedKeys;

  if ( match && foundAction != IA_NONE && foundAction != previouslyInvokedAction) {
      previouslyInvokedAction = foundAction;
      return true;
  }

  return false;
}

std::vector<KeyboardKey> Keybindings::getAllPushedKeys() { return _pushedKeys; }