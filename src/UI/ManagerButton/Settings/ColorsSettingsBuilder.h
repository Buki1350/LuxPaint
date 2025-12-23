#pragma once
#include "App/App.h"
#include "SettingsBuilder.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "UI/MiniMenu/MiniMenuBuilder.h"

class ColorsSettingsBuilder : public MiniMenuBuilderBase<ColorsSettingsBuilder> {
  inline static std::map<std::string, Color> colorDict;

    void CreateColorPickRow(
        const char* text,
        const char* colorName,
        std::function<void(Color)> colorChangeFunction
    ) {
        Color initialColor = colorDict[colorName];

        auto colorLabel = new UIObject();
        colorLabel->text = text;
        colorLabel->size = { colorLabel->text.getPixelWidth(), 20 };

        auto separator = new UIObject();
        separator->size = { 10, 20 };

        auto button = new Button();
        button->size = { 50, 20 };
        button->color = initialColor;
        button->roundness = 0.001f * uti::view::getSmallerMonitorEdge();
        button->outlineScale = .002f;
        button->outlineColor = uti::colors::getDynamicBlackOrWhiteFor(menu->getOBackground()->color);
        button->name = "colorButtonOutline";

        button->onClick([button, colorChangeFunction, colorName]() {
            menu->disableClosing();

            ColorPicker* cp = new ColorPicker();
            cp->position = uti::input::getMousePosition();

            cp->onColorChange([button, colorChangeFunction](Color newColor) {
                button->color = newColor;
                colorChangeFunction(newColor);  // tutaj wywołujemy przekazaną funkcję
            });

            cp->onDestroy([button, colorName]() {
                uti::files::saveColor(colorName, button->color);
                menu->enableClosing();
            });

            cp->setZLayer(button->getZLayer() + 1);
        });

        menu->packRow({ colorLabel, MiniMenu::flexSeparator(), button });
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
        label->size = { label->text.getPixelWidth(), 20 };

        auto separator = new UIObject();
        separator->size = { 10, 20 };

        Color initialColor = colorDict[colorName];
        auto button = new Button();
        button->size = { 50, 20 };
        button->color = initialColor;
        button->roundness = 0.001f * uti::view::getSmallerMonitorEdge();
        button->outlineScale = .002f;
        button->outlineColor = uti::colors::getDynamicBlackOrWhiteFor(menu->getOBackground()->color);
        button->name = "colorButtonOutline";


        button->onClick([button, colorName, colorChangeFunction]() {
            menu->disableClosing();

            ColorPicker* cp = new ColorPicker();
            cp->position = uti::input::getMousePosition();

            cp->onColorChange([button, colorChangeFunction](Color newColor) {
                button->color = newColor;
                colorChangeFunction(newColor);
            });

            cp->onDestroy([button, colorName]() {
                uti::files::saveColor(colorName, button->color);
                menu->enableClosing();
            });
        });

        menu->packRow({ label, MiniMenu::flexSeparator(), button });
    }

    void ApplyGlobalUIColor(Color c) {
        App::instance->toolBox.setBackgroundColor(c);
        App::instance->colorHolder.setBackgroundColor(c);
        App::instance->managerButton.color = c;
        MiniMenu::setBackgroundColorForAll(c);
        App::instance->toolSizeSlider.color = c;
    }

public:
    void BuildContext() {
        colorDict = uti::files::loadAllColors();

        auto uiColorsHeader = new UIObject();
        uiColorsHeader->text = "UI colors";
        uiColorsHeader->size = { uiColorsHeader->text.getPixelWidth(), 20 };

        menu->packRow({ MiniMenu::flexSeparator(), uiColorsHeader, MiniMenu::flexSeparator() });
        menu->createSmallSeparator();

        // === GLOBAL UI COLOR PICKER ===
        CreateColorPickRow(
            "Global UI color",
            "uiGlobal",
            [this](Color c) {
                ApplyGlobalUIColor(c);
            }
        );

        menu->createSmallSeparator();

        // === INDIVIDUAL COLORS (INDENTED) ===
        CreateIndentedColorPickRow(
            "Toolbox color",
            "toolbox",
            [this](Color c) { App::instance->toolBox.setBackgroundColor(c); }
        );

        CreateIndentedColorPickRow(
            "Color holder color",
            "colorHolder",
            [this](Color c) { App::instance->colorHolder.setBackgroundColor(c); }
        );

        CreateIndentedColorPickRow(
            "Manager button color",
            "managerButton",
            [this](Color c) { App::instance->managerButton.color = c; }
        );

        CreateIndentedColorPickRow(
            "Popup-menu color",
            "miniMenu",
            [this](Color c) { MiniMenu::setBackgroundColorForAll(c); }
        );

        CreateIndentedColorPickRow(
            "Tool size slider color",
            "toolSizeSlider",
            [this](Color c) { App::instance->toolSizeSlider.color = c; }
        );
    }
};