#pragma once
#include "SerializationTests/SerializationTest.h"
#include "TestResult.h"

#include <iostream>

class Tests {
  inline const static std::vector<UnitTest*> _unitTests = {
    new UnitTest_Keybindings_HasEveryActionAStringPair(),
    new UnitTest_Keybindings_IsKeyboardKeyNameMissing()
  };

public:
  static bool validate() {
    bool allOk = true;
    for (auto& t : _unitTests) {
      TestResult result = t->check();
      addResult(result);
      if (result.status() == ERROR) allOk = false;
    }
    return allOk;
  }

private:
  std::string _allErrors = "";

  static void addResult(TestResult testResult) {
    std::string prefix;
    switch (testResult.status()) {
    case OK:
      prefix = "[ OK ] ";
      break;
    case WARNING:
      prefix = "[ WARN ] ";
      break;
    case ERROR:
      prefix = "[ ERR ] ";
      break;
    default:
      prefix = "[???] ";
      break;
    }

    if (!testResult.getMessage().empty()) {
      switch (testResult.status()) {
      case ERROR:
        std::cerr << prefix << testResult.getMessage() << "\n";
        break;
      default:
        std::cout << prefix << testResult.getMessage() << "\n";
        break;
      }
    }

  }
};
