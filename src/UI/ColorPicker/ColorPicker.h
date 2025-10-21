#pragma once
#include "raylib.h"
#include "../../Render/UIObject.h"
#include "../../Render/Prefabs/InputField.h"
#include "../../StaticShared/Utils/Utils.h"
#include "../../Render/UIObjectsManager.h"

class ColorPicker : public UIObject, public Updatable {
    UIObject* _oRainbow = nullptr;
    UIObject* _oPreview = nullptr;
    InputField* _oHexInput = nullptr;
    InputField* _oAlphaInput = nullptr;

    Color _selectedColor = WHITE;
    float _alpha = 1.0f;
    bool _isActive = false;

    void _GenerateRainbowTexture();
    Color _GetColorFromRainbowAt(int x, int y);
    void _UpdateHexFromColor();
    void _UpdateColorFromHex();
    void _UpdateAlphaFromInput();

public:
    ColorPicker();
    void Update() override;
    void SetActive(bool active);
    Color GetSelectedColor() const;
};
