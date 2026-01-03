#pragma once
#include "Debug/Tests/TestTypes.h"
#include "Shared/Keybindings/Keybindings.h"

class UnitTest_Keybindings_HasEveryActionAStringPair final : public UnitTest {
public:
    TestResult check() override {
    if (Keybindings::getActionsNames().size() != Keybindings::getDefaultBindings().size()) {
      return TestResult::error("[Keybindings] Not all InputActions have corresponding string names");
    }
    return TestResult::ok();
  };
};

class UnitTest_Keybindings_IsKeyboardKeyNameMissing final : public UnitTest {
public:
  TestResult check() override {
    auto keysWithNames = Keybindings::getKeyNames();
    auto defaultBindings = Keybindings::getDefaultBindings();

    for (const auto &binding : defaultBindings) {
      for (KeyboardKey keycode : binding.second) {
        bool found = false;

        for (const auto &keyWithName : keysWithNames) {
          if (keyWithName.key == keycode) {
            found = true;
            break;
          }
        }

        if (!found) {
          return TestResult::error(
              ("[Keybindings] Missing string name for KeyboardKey code " +
               std::to_string(keycode))
                  .c_str()
          );
        }
      }
    }

    return TestResult::ok();
  };
};
