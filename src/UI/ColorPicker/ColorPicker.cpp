#include "ColorPicker.h"

#include "../../Shared/FilesManager/FilesManager.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

ColorPicker::ColorPicker() {
  _backgroundColor = Utils::Files::LoadColor("colorPicker");
  this->color = _backgroundColor;
  this->size = Vec2f(Utils::View::GetSmallerMonitorEdge() * _overallSizeScale);

  _oRainbow = new UIObject;
  _oRainbow->outlineScale = 0;
  _oRainbow->imageStretch = true;
  _oRainbow->imageMarginScale = 0.0f;
  _GenerateRainbowTexture();

  _oDot = new UIObject;
  _oDot->roundness = 1;
  _oDot->color = WHITE;
  //_oDot->outlineScale = 0.5f;

  _hexInputField = new InputField;
  _hexInputField->SetMode(InputMode::LETTERS_AND_NUMBERS);

  // Slidery
  _sRSlider = new Slider(HORIZONTAL);
  _sGSlider = new Slider(HORIZONTAL);
  _sBSlider = new Slider(HORIZONTAL);
  _sASlider = new Slider(HORIZONTAL);
  _sASlider->SetValue(1);
  _sliders = {_sRSlider, _sGSlider, _sBSlider, _sASlider};

  for (auto slider : _sliders)
    slider->zLayer = this->zLayer + 1;

  // Etykiety R G B A
  for (int i = 0; i < 4; i++) {
    const char *labels = "RGBA";
    UIObject * label = new UIObject;
    label->text = std::string(1, labels[i]);
    label->color = Utils::Files::LoadColor("colorPickerLabel", "uiGlobal");
    label->outlineScale = 0;
    label->zLayer = this->zLayer + 2;
    _sliderLabels.push_back(label);
  }

  _currentColor = WHITE;
  __previousColor = _currentColor;
}

void ColorPicker::Update() {
  float scale = Utils::View::GetSmallerMonitorEdge();
  float margin = _marginScale * scale;
  //float spaceUnderRainbow = _spaceUnderRainbowScale * scale;

  this->roundness = Utils::View::GetSmallerMonitorEdge() *  UI_WIDGETS_ROUNDNESS;

  float oRainbowWidth = this->size.x - 2 * margin;
  float oRainbowHeight = this->size.y * 0.5f; // panoramiczne
  _oRainbow->size = {oRainbowWidth, oRainbowHeight};
  _oRainbow->position = this->position + Vec2f(margin, margin);
  _oRainbow->zLayer = this->zLayer + 1;

  float dotSize = 0.01f * scale;
  _oDot->size = {dotSize, dotSize};
  _oDot->zLayer = this->zLayer + 3;

  float hexInputFieldWidth = this->size.x / 2;
  float hexInputFieldHeight = 0.03f * scale;
  _hexInputField->size = {hexInputFieldWidth, hexInputFieldHeight};
  _hexInputField->position = {
      _oRainbow->position.x,
      _oRainbow->position.y + _oRainbow->size.y + margin};
  _hexInputField->zLayer = this->zLayer + 1;

  // Slidery RGBA + etykiety
  float availableHeight =
      this->size.y - _oRainbow->size.y - _hexInputField->size.y - 7 * margin;
  float sliderHeight = availableHeight / _sliders.size();

  for (int i = 0; i < _sliders.size(); i++) {
    auto slider = _sliders[i];
    float yPos = _hexInputField->position.y + _hexInputField->size.y + margin +
                 i * (sliderHeight + margin);

    // Etykieta
    UIObject* label = _sliderLabels[i];
    label->size = {sliderHeight, sliderHeight};
    label->position = {_oRainbow->position.x, yPos};
    //label->textSize = 0.4f * sliderHeight;

    // Suwak obok etykiety
    float sliderX = label->position.x + label->size.x + margin;
    float sliderWidth =
        this->size.x - sliderX - margin; // mieści się w tle
    slider->position = {sliderX, yPos};
    slider->size = {sliderWidth, sliderHeight};
  }

  // Logika
  _UpdateFromSliders();
  _UpdateFromHex();
  _UpdateFromRainbow();
  _SyncUI();

  if (ClickedButNotThis())
    Destroy();

  if (
    _currentColor.r != __previousColor.r ||
    _currentColor.g != __previousColor.g ||
    _currentColor.b != __previousColor.b ||
    _currentColor.a != __previousColor.a
    ) { _OnColorChange(_currentColor); }

  __previousColor = _currentColor;
}

void ColorPicker::_GenerateRainbowTexture() {
  constexpr int size = 512;
  Image img = GenImageColor(size, size, BLANK);
  Color* pixels = (Color*)img.data;

  float cx = size / 2.0f;
  float cy = size / 2.0f;

  float radiusX = size / 2.0f;
  float radiusY = size / 2.0f;

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {

      // NORMALIZACJA DO ELIPSY (działa też dla koła)
      float dx = (x - cx) / radiusX;
      float dy = (y - cy) / radiusY;

      float r = sqrtf(dx*dx + dy*dy);

      if (r > 1.0f) {
        pixels[y * size + x] = {0,0,0,0};
        continue;
      }

      float angle = atan2f(dy, dx);
      float hue = angle * 180.0f / PI;
      if (hue < 0) hue += 360.0f;

      float saturation = r;
      float value = powf(1.0f - r, 0.40f);

      pixels[y * size + x] = ColorFromHSV(hue, saturation, value);
    }
  }

  _oRainbow->SetImage(img);
  UnloadImage(img);
}



void ColorPicker::_UpdateFromSliders() {
  if (!_sRSlider || !_sGSlider || !_sBSlider || !_sASlider)
    return;

  int r = (int)(_sRSlider->GetValue() * 255);
  int g = (int)(_sGSlider->GetValue() * 255);
  int b = (int)(_sBSlider->GetValue() * 255);
  int a = (int)(_sASlider->GetValue() * 255);
  _currentColor = {(unsigned char)r, (unsigned char)g, (unsigned char)b,
                   (unsigned char)a};
}

void ColorPicker::_UpdateFromHex() {
  if (_hexInputField->IsFocused()) {
    std::string hex = _hexInputField->GetValue();
    if (hex.size() > 0 && hex[0] == '#')
      hex = hex.substr(1);

    if (hex.size() == 6 || hex.size() == 8) {
      unsigned int val = 0;
      std::stringstream ss;
      ss << std::hex << hex;
      ss >> val;

      if (hex.size() == 6)
        _currentColor = {(unsigned char)((val >> 16) & 0xFF),
                         (unsigned char)((val >> 8) & 0xFF),
                         (unsigned char)(val & 0xFF), 255};
      else
        _currentColor = {(unsigned char)((val >> 24) & 0xFF),
                         (unsigned char)((val >> 16) & 0xFF),
                         (unsigned char)((val >> 8) & 0xFF),
                         (unsigned char)(val & 0xFF)};
    }
  }
}

void ColorPicker::_UpdateFromRainbow() {
  if (_oRainbow->CursorAbove() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

    Vector2 mp = GetMousePosition();
    float relX = (mp.x - _oRainbow->position.x);
    float relY = (mp.y - _oRainbow->position.y);

    float cx = _oRainbow->size.x / 2.0f;
    float cy = _oRainbow->size.y / 2.0f;

    // normalizacja do elipsy
    float dx = (relX - cx) / cx;
    float dy = (relY - cy) / cy;

    float r = sqrtf(dx*dx + dy*dy);
    if (r > 1.0f)
      return; // klik poza elipsą

    float angle = atan2f(dy, dx);
    float hue = angle * 180.0f / PI;
    if (hue < 0) hue += 360.0f;

    // dokładnie tak samo jak w generatorze
    float saturation = r;
    float value = powf(1.0f - r, 0.75f);

    _currentColor = ColorFromHSV(hue, saturation, value);

    _oDot->position = {mp.x - _oDot->size.x/2, mp.y - _oDot->size.y/2};
  }
}



void ColorPicker::_SyncUI() {
  std::stringstream ss;
  ss << "#" << std::uppercase << std::hex << std::setfill('0')
     << std::setw(2) << (int)_currentColor.r << std::setw(2)
     << (int)_currentColor.g << std::setw(2) << (int)_currentColor.b;
  _hexInputField->SetValue(ss.str());

  _sRSlider->SetValue(_currentColor.r / 255.0f);
  _sGSlider->SetValue(_currentColor.g / 255.0f);
  _sBSlider->SetValue(_currentColor.b / 255.0f);
  _sASlider->SetValue(_currentColor.a / 255.0f);
}

void ColorPicker::Destroy() {
  _oRainbow->Destroy();
  _oDot->Destroy();
  _hexInputField->Destroy();

  for (auto &label : _sliderLabels)
    if (label) label->Destroy();

  for (auto &slider : _sliders)
    if (slider) slider->Destroy();

  if (_OnDestroy)   // <––––– tu!
    _OnDestroy();

  UIObject::Destroy();
}


Color ColorPicker::GetColor() const { return _currentColor; }

void ColorPicker::SetColor(Color color) {
  _currentColor = color;
  __previousColor = _currentColor;
  _SyncUI();
}

void ColorPicker::OnColorChange(std::function<void(Color)> callback) {
  _OnColorChange = callback;
}

void ColorPicker::OnDestroy(std::function<void()> labdaFunction) { _OnDestroy = labdaFunction; }
