#include "Keybindings.h"
#include "../../Shared/Utils/Utils.h"

#include <iostream>

void Keybindings::_Load() {
  actionsDictionary = Utils::Files::LoadActions(); // <--- default actions set here

}

bool Keybindings::ActionDetected(InputAction action) {
  if (actionsDictionary.empty()) {
    _Load();
  }

  for (auto e : actionsDictionary) {
    for (auto k : e.second) {
      if (IsKeyDown(k)) {
        if (std::find(_pushedKeys.begin(), _pushedKeys.end(), k) ==
            _pushedKeys.end()) {
          _pushedKeys.push_back(k);
          __previouslyInvokedAction = IA_NONE;
        }
      } else {
        if (std::find(_pushedKeys.begin(), _pushedKeys.end(), k) !=
            _pushedKeys.end()) {
          _pushedKeys.erase(
              std::find(_pushedKeys.begin(), _pushedKeys.end(), k));
          __previouslyInvokedAction = IA_NONE;
        }
      }
    }
  }

  if (_pushedKeys.empty())
    return false;

  bool match = true;
  InputAction foundAction = IA_NONE;
  for (auto e : actionsDictionary) {
    if (e.second.size() != _pushedKeys.size())
      continue;
    match = true;
    for (int i = 0; i < e.second.size(); i++) {
      if (e.second[i] != _pushedKeys[i]) {
        match = false;
        break;
      }
    }
    if (match && e.first == action) {
      foundAction = action;
      break;
    }
  }

  if (match && foundAction != IA_NONE &&
      foundAction != __previouslyInvokedAction) {
    __previouslyInvokedAction = foundAction;
    return true;
  }

  return false;
}
std::vector<KeyboardKey> Keybindings::GetAllPushedKeys() { return _pushedKeys; }



