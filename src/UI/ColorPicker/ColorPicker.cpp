#include "ColorPicker.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

ColorPicker::ColorPicker() {
    _oRainbow = new UIObject();//UIObjectsManager::Create();
    _GenerateRainbowTexture();

    _oPreview = new UIObject();//UIObjectsManager::Create();
    _oPreview->color = WHITE;
    _oPreview->roundness = 0.2f;

    _oHexInput = new InputField();// UIObjectsManager::CreateInputField();
    _oHexInput->SetMode(InputMode::LETTERS_AND_NUMBERS);
    _oHexInput->SetValue("#FFFFFF");

    _oAlphaInput = new InputField();// UIObjectsManager::CreateInputField();
    _oAlphaInput->SetMode(InputMode::NUMBERS_ONLY);
    _oAlphaInput->SetValue("1.0");

    _isActive = true;
}

void ColorPicker::_GenerateRainbowTexture() {
    const int width = 256, height = 256;
    Image img = GenImageColor(width, height, BLANK);
    Color* pixels = LoadImageColors(img);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float hue = (float)x / width * 360.0f;
            float sat = 1.0f - ((float)y / height);
            pixels[y * width + x] = ColorFromHSV(hue, sat, 1.0f);
        }
    }

    Image newImg = { pixels, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

    _oRainbow->SetImage(newImg);
    UnloadImageColors(pixels);
}

Color ColorPicker::_GetColorFromRainbowAt(int x, int y) {
    Image img = _oRainbow->GetImage();
    if (!img.data) return WHITE;

    x = std::clamp(x, 0, img.width - 1);
    y = std::clamp(y, 0, img.height - 1);

    Color* pixels = LoadImageColors(img);
    Color c = pixels[y * img.width + x];
    UnloadImageColors(pixels);
    return c;
}

void ColorPicker::_UpdateHexFromColor() {
    std::stringstream ss;
    ss << "#" << std::uppercase << std::hex
       << std::setw(2) << std::setfill('0') << (int)_selectedColor.r
       << std::setw(2) << std::setfill('0') << (int)_selectedColor.g
       << std::setw(2) << std::setfill('0') << (int)_selectedColor.b;
    _oHexInput->SetValue(ss.str());
}

void ColorPicker::_UpdateColorFromHex() {
    std::string val = _oHexInput->GetValue();
    if (val.size() != 7 || val[0] != '#') return;

    unsigned int rgb;
    std::stringstream ss;
    ss << std::hex << val.substr(1);
    ss >> rgb;

    _selectedColor.r = (rgb >> 16) & 0xFF;
    _selectedColor.g = (rgb >> 8) & 0xFF;
    _selectedColor.b = rgb & 0xFF;
    _oPreview->color = _selectedColor;
}

void ColorPicker::_UpdateAlphaFromInput() {
    try {
        _alpha = std::clamp(std::stof(_oAlphaInput->GetValue()), 0.0f, 1.0f);
    } catch (...) {
        _alpha = 1.0f;
    }
}

void ColorPicker::Update() {
    if (!_isActive) return;

    _oHexInput->Update();
    _oAlphaInput->Update();

    // kliknięcie na tęczę
    if (_oRainbow->CursorAbove() && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vec2f mouse = Utils::GetMousePosition();
        int localX = (mouse.x - _oRainbow->position.x) * (_oRainbow->GetImageSize().x / _oRainbow->size.x);
        int localY = (mouse.y - _oRainbow->position.y) * (_oRainbow->GetImageSize().y / _oRainbow->size.y);

        _selectedColor = _GetColorFromRainbowAt(localX, localY);
        _oPreview->color = _selectedColor;
        _UpdateHexFromColor();
    }

    if (_oHexInput->IsFocused()) _UpdateColorFromHex();
    if (_oAlphaInput->IsFocused()) _UpdateAlphaFromInput();
}

Color ColorPicker::GetSelectedColor() const {
    Color c = _selectedColor;
    c.a = (unsigned char)(_alpha * 255);
    return c;
}

void ColorPicker::SetActive(bool active) {
    _isActive = active;
}
