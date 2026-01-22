#include "ColorPicker.h"

#include "../../Shared/Serializer/Serializer.h"
#include "App/App.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/View/utiView.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

ColorPicker::ColorPicker() {
  _backgroundColor = uti::files::loadColor("colorPicker");
  this->color = _backgroundColor;
  this->size = Vec2f(uti::view::getSmallerMonitorEdge() * _overallSizeScale);
  this->setZLayer(LAYER_COLORPICKER);

  _oRainbow = new UIObject;
  _oRainbow->outlineScale = 0;
  _oRainbow->imageStretch = true;
  _oRainbow->imageMarginScale = 0.0f;
  _oRainbow->setZLayer(getZLayer() + 1);
  _generateRainbowTexture();

  _oDot = new UIObject;
  _oDot->roundness = 1;
  _oDot->color = WHITE;
  //_oDot->outlineScale = 0.5f;

  _hexInputField = new InputField;
  _hexInputField->setMode(InputMode::LETTERS_AND_NUMBERS);

  _sRSlider = new Slider(HORIZONTAL);
  _sGSlider = new Slider(HORIZONTAL);
  _sBSlider = new Slider(HORIZONTAL);
  _sASlider = new Slider(HORIZONTAL);
  _sASlider->setValue(1);
  _sliders = {_sRSlider, _sGSlider, _sBSlider, _sASlider};

  for (auto slider : _sliders)
    slider->setZLayer(this->getZLayer() + 1);

  // Etykiety R G B A
  for (int i = 0; i < 4; i++) {
    const char *labels = "RGBA";
    UIObject * label = new UIObject;
    label->text = std::string(1, labels[i]);
    label->color = uti::files::loadColor("colorPickerLabel", "uiGlobal");
    label->outlineScale = 0;
    label->setZLayer(this->getZLayer() + 2);
    _sliderLabels.push_back(label);
  }

  _currentColor = App::instance().colorHolder.getCurrentColor();
  __previousColor = _currentColor;
}

void ColorPicker::update() {
  float scale = uti::view::getSmallerMonitorEdge();
  float margin = _marginScale * scale;

  this->roundness = uti::view::getSmallerMonitorEdge() *  0.00005;

  float oRainbowWidth = this->size.x - 2 * margin;
  float oRainbowHeight = this->size.y * 0.5f;
  _oRainbow->size = {oRainbowWidth, oRainbowHeight};
  _oRainbow->position = this->position + Vec2f(margin, margin);
  _oRainbow->setZLayer(this->getZLayer() + 1);

  float dotSize = 0.01f * scale;
  _oDot->size = {dotSize, dotSize};
  _oDot->setZLayer(this->getZLayer() + 3);

  float hexInputFieldWidth = this->size.x / 2;
  float hexInputFieldHeight = 0.03f * scale;
  _hexInputField->size = {hexInputFieldWidth, hexInputFieldHeight};
  _hexInputField->position = {
      _oRainbow->position.x,
      _oRainbow->position.y + _oRainbow->size.y + margin};
  _hexInputField->setZLayer(this->getZLayer() + 1);

  float availableHeight =
      this->size.y - _oRainbow->size.y - _hexInputField->size.y - 7 * margin;
  float sliderHeight = availableHeight / _sliders.size();

  for (int i = 0; i < _sliders.size(); i++) {
    auto slider = _sliders[i];
    float yPos = _hexInputField->position.y + _hexInputField->size.y + margin +
                 i * (sliderHeight + margin);

    UIObject* label = _sliderLabels[i];
    label->size = {sliderHeight, sliderHeight};
    label->position = {_oRainbow->position.x, yPos};

    float sliderX = label->position.x + label->size.x + margin;
    float sliderWidth =
        this->size.x - sliderX - margin;
    slider->position = {sliderX, yPos};
    slider->size = {sliderWidth, sliderHeight};
  }

  _updateFromSliders();
  _updateFromHex();
  _updateFromRainbow();
  _syncUI();

  if (clickedButNotThis())
    destroy();

  // ... when colorpicker popped up, _OnColorChange was being invoked with default BLACK color
  if (_justStarted) _justStarted = false;
  else if (
    _currentColor.r != __previousColor.r ||
    _currentColor.g != __previousColor.g ||
    _currentColor.b != __previousColor.b ||
    _currentColor.a != __previousColor.a
    ) {
    _OnColorChange(_currentColor);
  }

  __previousColor = _currentColor;
}

void ColorPicker::_generateRainbowTexture() {
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

  _oRainbow->setImage(img);
  UnloadImage(img);
}



void ColorPicker::_updateFromSliders() {
  if (!_sRSlider || !_sGSlider || !_sBSlider || !_sASlider)
    return;

  int r = (int)(_sRSlider->getValue() * 255);
  int g = (int)(_sGSlider->getValue() * 255);
  int b = (int)(_sBSlider->getValue() * 255);
  int a = (int)(_sASlider->getValue() * 255);
  _currentColor = {(unsigned char)r, (unsigned char)g, (unsigned char)b,
                   (unsigned char)a};
}

void ColorPicker::_updateFromHex() {
  if (_hexInputField->isFocused()) {
    std::string hex = _hexInputField->getValue();
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

void ColorPicker::_updateFromRainbow() {
  if (_oRainbow->cursorAbove() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

    Vec2f mp = uti::input::getMousePosition();
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



void ColorPicker::_syncUI() {
  std::stringstream ss;
  ss << "#" << std::uppercase << std::hex << std::setfill('0')
     << std::setw(2) << (int)_currentColor.r << std::setw(2)
     << (int)_currentColor.g << std::setw(2) << (int)_currentColor.b;
  _hexInputField->setValue(ss.str());

  _sRSlider->setValue(_currentColor.r / 255.0f);
  _sGSlider->setValue(_currentColor.g / 255.0f);
  _sBSlider->setValue(_currentColor.b / 255.0f);
  _sASlider->setValue(_currentColor.a / 255.0f);
}

void ColorPicker::destroy() {
  _oRainbow->destroy();
  _oDot->destroy();
  _hexInputField->destroy();

  for (auto &label : _sliderLabels)
    if (label) label->destroy();

  for (auto &slider : _sliders)
    if (slider) slider->destroy();

  if (_OnDestroy) _OnDestroy();

  Updatable::markedForDeletion = true;
  UIObject::destroy();
}


Color ColorPicker::getColor() const { return _currentColor; }

void ColorPicker::setColor(Color color) {
  _currentColor = color;
  __previousColor = _currentColor;
  _syncUI();
}

void ColorPicker::onColorChange(std::function<void(Color)> callback) {
  _OnColorChange = callback;
}

void ColorPicker::onDestroy(std::function<void()> labdaFunction) { _OnDestroy = labdaFunction; }
