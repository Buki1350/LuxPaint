#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../UIObject.h"
#include "raylib.h"

#include <string>

enum class InputMode {
  NUMBERS_ONLY,
  LETTERS_ONLY,
  LETTERS_AND_NUMBERS
};

class InputField : public UIObject, public Updatable {
  std::string _value;
  InputMode _mode = InputMode::LETTERS_AND_NUMBERS;
  bool _focused = false;
  int _cursorPos = 0;       // pozycja kursora w stringu
  float _cursorTimer = 0.0; // do mrugania
  bool _cursorVisible = true;
  float _scrollOffset = 0;  // przesunięcie tekstu w lewo gdy jest długi

public:
  InputField();

  void SetMode(InputMode mode);
  std::string GetValue() const;
  void SetValue(const std::string &v);

  void Update() override;
  void Draw() override;

  bool IsFocused() const { return _focused; }
};
