#pragma once
#include "../ColorPicker/ColorPicker.h"
#include "../MiniMenu/MiniMenuBuilder.h"

#include <map>

class SettingsBuilder : public MiniMenuBuilderBase<SettingsBuilder> {
  inline static std::map<std::string, Color> colorDict;

  void CreateColorPickRow(
    const char* text,
    const char* colorName,
    std::function<void(Color)> colorChangeFunction
  ) {
    Color initialColor = colorDict[colorName];

    auto colorLabel = new UIObject();
    colorLabel->text = text;
    colorLabel->size = { colorLabel->text.GetPixelWidth(), 20 };

    auto separator = new UIObject();
    separator->size = { 10, 20 };

    auto colorButton = new Button();
    colorButton->size = { 50, 20 };
    colorButton->color = initialColor;
    colorButton->roundness = 0.001f * Utils::View::GetSmallerMonitorEdge();
    colorButton->outlineScale = 0;

    colorButton->OnClick([colorButton, colorChangeFunction, colorName]() {
        menu->DisableClosing();

        ColorPicker* cp = new ColorPicker();
        cp->position = Utils::Input::GetMousePosition();

        cp->OnColorChange([colorButton, colorChangeFunction](Color newColor) {
            colorButton->color = newColor;
            colorChangeFunction(newColor);  // tutaj wywołujemy przekazaną funkcję
        });

        cp->OnDestroy([colorButton, colorName]() {
            Utils::Files::SaveColor(colorName, colorButton->color);
            menu->EnableClosing();
        });

        cp->SetZLayer(colorButton->GetZLayer() + 1);
    });

    menu->PackRow({ colorLabel, MiniMenu::FlexSeparator(), colorButton });
  }

  void CreateIndentedColorPickRow(
    const char* text,
    const char* colorName,
    std::function<void(Color)> colorChangeFunction
) {
    std::string indentedText = "  - ";
    indentedText += text;

    auto label = new UIObject();
    label->text = indentedText;
    label->size = { label->text.GetPixelWidth(), 20 };

    auto separator = new UIObject();
    separator->size = { 10, 20 };

    Color initialColor = colorDict[colorName];
    auto button = new Button();
    button->size = { 50, 20 };
    button->color = initialColor;
    button->roundness = 0.001f * Utils::View::GetSmallerMonitorEdge();

    button->OnClick([button, colorName, colorChangeFunction]() {
        menu->DisableClosing();

        ColorPicker* cp = new ColorPicker();
        cp->position = Utils::Input::GetMousePosition();

        cp->OnColorChange([button, colorChangeFunction](Color newColor) {
            button->color = newColor;
            colorChangeFunction(newColor);
        });

        cp->OnDestroy([button, colorName]() {
            Utils::Files::SaveColor(colorName, button->color);
            menu->EnableClosing();
        });
    });

    menu->PackRow({ label, MiniMenu::FlexSeparator(), button });
  }

  void ApplyGlobalUIColor(Color c) {
    App::Instance->toolBox.SetBackgroundColor(c);
    App::Instance->colorHolder.SetBackgroundColor(c);
    App::Instance->managerButton.color = c;
    MiniMenu::SetBackgroundColorForAll(c);
    App::Instance->toolSizeSlider.color = c;
  }

  public:
    void BuildContext() {
        colorDict = Utils::Files::LoadAllColors();

        auto menuName = new UIObject();
        menuName->size = { 500, 20 };
        menuName->text = "SETTINGS";
        menuName->text.center = true;

        auto bigSeparator = new UIObject();
        bigSeparator->size = { 500, 5 };
        bigSeparator->color = { 100, 50, 255, 255 };
        bigSeparator->roundness = 1;

        auto CreateSmallSeparator = []() {
            auto sep = new UIObject();
            sep->size = { 500, 2 };
            sep->color = BLACK;
            sep->roundness = 1;
            sep->outlineScale = 0;
            menu->PackRow({ sep });
        };

        menu->PackRow({ menuName });
        menu->PackRow({ bigSeparator });

        // === UI COLORS HEADER ===
        auto uiColorsHeader = new UIObject();
        uiColorsHeader->text = "UI colors";
        uiColorsHeader->size = { uiColorsHeader->text.GetPixelWidth(), 20 };

        menu->PackRow({ uiColorsHeader });
        CreateSmallSeparator();

        // === GLOBAL UI COLOR PICKER ===
        CreateColorPickRow(
            "Global UI color",
            "uiGlobal",
            [this](Color c) {
                ApplyGlobalUIColor(c);
            }
        );

        CreateSmallSeparator();

        // === INDIVIDUAL COLORS (INDENTED) ===
        CreateIndentedColorPickRow(
            "Toolbox color",
            "toolbox",
            [this](Color c) { App::Instance->toolBox.SetBackgroundColor(c); }
        );

        CreateIndentedColorPickRow(
            "Color holder color",
            "colorHolder",
            [this](Color c) { App::Instance->colorHolder.SetBackgroundColor(c); }
        );

        CreateIndentedColorPickRow(
            "Manager button color",
            "managerButton",
            [this](Color c) { App::Instance->managerButton.color = c; }
        );

        CreateIndentedColorPickRow(
            "Popup-menu color",
            "miniMenu",
            [this](Color c) { MiniMenu::SetBackgroundColorForAll(c); }
        );

        CreateIndentedColorPickRow(
            "Tool size slider color",
            "toolSizeSlider",
            [this](Color c) { App::Instance->toolSizeSlider.color = c; }
        );
    }
};