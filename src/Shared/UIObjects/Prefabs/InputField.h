#pragma once
#include "../UIObject.h"
#include "../../../Automatition/Updatables/Updatable.h"

#include <string>

enum class InputMode {
  NUMBERS_ONLY,
  LETTERS_ONLY,
  LETTERS_AND_NUMBERS
};

class InputField final : public UIObject, public Updatable {
  inline static InputField* _instance = nullptr; // instance -> only one selected
  inline static InputField* _queued = nullptr; // helper; one doesn't delete another
  std::string _value;
  InputMode _mode = InputMode::LETTERS_AND_NUMBERS;
  int _cursorPos = 0;       // pozycja kursora w stringu
  float _cursorTimer = 0.0; // do mrugania
  bool _cursorVisible = true;
  float _scrollOffset = 0;  // przesunięcie tekstu w lewo gdy jest długi

public:
  InputField();
  ~InputField() override;

  void SetMode(InputMode mode);
  std::string GetValue() const;
  void SetValue(const std::string &v);
  void SetFocused(bool value);

  void Update() override;
  void Draw() override;

  bool IsFocused() const { return this == _instance; }
};
