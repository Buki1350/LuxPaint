#include "Keybindings.h"

#include <iostream>

void Keybindings::_Load() {
    //TODO keybindings from file system

    actionsDictionary[MENU_NEXT] = { KEY_TAB };
    actionsDictionary[MENU_PREV] = { KEY_LEFT_SHIFT, KEY_TAB };
    actionsDictionary[MENU_CONFIRM] = { KEY_ENTER };
}

//TODO zbuggowane kombinacje!
bool Keybindings::ActionDetected(InputAction action) {
  if (actionsDictionary.empty()) { _Load(); }

  for (auto e : actionsDictionary) {
    for (auto k : e.second) {
      if (IsKeyDown(k)) {
        if (std::find(_pushedKeys.begin(), _pushedKeys.end(), k) == _pushedKeys.end()) {
          _pushedKeys.push_back(k);
          __previouslyInvokedAction = IA_NONE;
        }
      }
      else {
        if (std::find(_pushedKeys.begin(), _pushedKeys.end(), k) != _pushedKeys.end()) {
          _pushedKeys.erase(std::find(_pushedKeys.begin(), _pushedKeys.end(), k));
          __previouslyInvokedAction = IA_NONE;
        }
      }
    }
  }

  std::cout << "[ ";
  for (int i = 0; i < _pushedKeys.size(); i++) {
    std::cout << _pushedKeys[i];
    if (i == _pushedKeys.size() - 1) { std::cout << " "; }
    else { std::cout << ", "; }
  }
  std::cout << "]" << std::endl << std::endl;

  if (_pushedKeys.empty())
    return false;

  bool match = true;
  InputAction foundAction = IA_NONE;
  for (auto e : actionsDictionary) {
    if (e.second.size() != _pushedKeys.size()) continue;
    match = true;
    for (int i = 0; i < e.second.size(); i++) {
      if (e.second[i] != _pushedKeys[i]) { match = false; break; }
    }
    if (match && e.first == action) {
      foundAction = action;
      break;
    }
  }

  if (match && foundAction != IA_NONE && foundAction != __previouslyInvokedAction) {
    __previouslyInvokedAction = foundAction;
    return true;
  }

  return false;
}



