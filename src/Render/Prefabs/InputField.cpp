#include "../Prefabs/InputField.h"
#include "../../StaticShared/Utils/Utils.h"
#include "raylib.h"

InputField::InputField() {
  text.SetParent(this);
  text.center = false;
  text.margin = 5;
  color = WHITE;
}
InputField::~InputField() {
    if (this == _instance) { _instance = nullptr; }
}

void InputField::SetMode(InputMode mode) {
    _mode = mode;
}

std::string InputField::GetValue() const {
    return _value;
}

void InputField::SetValue(const std::string &v) {
  _value = v;
  _cursorPos = (int)_value.size();
}
void InputField::SetFocused(bool value) {
    if (value == true) {
        _queued = this;
    }
    else {
        _instance = nullptr;
    }
}

void InputField::Update() {
    if (Clicked()) {
        _queued = this;
    } else if (IsMouseButtonPressed(0) && !CursorAbove()) {
        _instance = nullptr;
    }
    if (_queued != nullptr) {
        _instance = _queued;
        _queued = nullptr;
    }

    if (this == _instance) {
        int key = GetCharPressed();
        while (key > 0) {
            char c = (char)key;
            bool accept = false;

            if (_mode == InputMode::NUMBERS_ONLY && (c >= '0' && c <= '9')) accept = true;
            else if (_mode == InputMode::LETTERS_ONLY && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) accept = true;
            else if (_mode == InputMode::LETTERS_AND_NUMBERS && (isalnum(c) || c == ' ')) accept = true;

            if (accept) {
                _value.insert(_cursorPos, 1, c);
                _cursorPos++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && _cursorPos > 0) {
            _value.erase(_cursorPos - 1, 1);
            _cursorPos--;
        }
        if (IsKeyPressed(KEY_LEFT) && _cursorPos > 0) {
            _cursorPos--;
        }
        if (IsKeyPressed(KEY_RIGHT) && _cursorPos < (int)_value.size()) {
            _cursorPos++;
        }
    }

    _cursorTimer += Utils::GetDeltaTime();
    if (_cursorTimer > 0.5f) {
        _cursorVisible = !_cursorVisible;
        _cursorTimer = 0.0f;
    }

    int textWidth = MeasureText(_value.c_str(), text.fontSize);
    if (textWidth > size.x - 2 * text.margin) {
        int cursorPixel = MeasureText(_value.substr(0, _cursorPos).c_str(), text.fontSize);
        if (cursorPixel - _scrollOffset > size.x - 2 * text.margin) {
            _scrollOffset = cursorPixel - (size.x - 2 * text.margin);
        } else if (cursorPixel - _scrollOffset < 0) {
            _scrollOffset = cursorPixel;
        }
    } else {
        _scrollOffset = 0;
    }
}

void InputField::Draw() {
    UIObject::Draw();

    Vec2f finalPos = position;
    Vec2f finalSize = size;

    BeginScissorMode((int)finalPos.x, (int)finalPos.y, (int)finalSize.x, (int)finalSize.y);

    DrawTextEx(
        Utils::GetDefaultFont(),
        _value.c_str(),
        { finalPos.x + text.margin - _scrollOffset, finalPos.y + (finalSize.y - text.fontSize) / 2.0f },
        (float)text.fontSize,
        1.0f,
        text.textColor
    );

    if (this == _instance && _cursorVisible) {
        std::string beforeCursor = _value.substr(0, _cursorPos);
        int cursorX = MeasureText(beforeCursor.c_str(), text.fontSize);
        int x = (int)(finalPos.x + text.margin + cursorX - _scrollOffset);
        int y = (int)(finalPos.y + (finalSize.y - text.fontSize) / 2.0f);
        DrawRectangle(x, y, 2, text.fontSize, text.textColor);
    }

    EndScissorMode();
}
