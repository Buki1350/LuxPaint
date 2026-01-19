#pragma once
#include "../../../Automatition/Updatables/Updatable.h"
#include "../UIObject.h"

#include <functional>
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
  std::function<void()> _onValueChangedCallback = nullptr;

public:
  InputField();

  void setMode(InputMode mode);
  std::string getValue() const;
  void setValue(const std::string &v);
  void setFocused(bool value);

  void update() override;
  void draw() override;
  void destroy() override;

  bool isFocused() const { return this == _instance; }
  void onValueChanged(std::function<void()> lambdaFunc);
};
