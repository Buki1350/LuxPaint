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

        cp->zLayer = colorButton->zLayer + 1;
    });

    menu->PackRow({ colorLabel, MiniMenu::FlexSeparator(), colorButton });
  }

  public:
    void BuildContext() {
      colorDict = Utils::Files::LoadAllColors();

      // ... 1
      auto menuName = new UIObject();
      menuName->size = { 500, 20 };
      menuName->text = "SETTINGS";
      menuName->text.center = true;

      auto bigSeparator = new UIObject();
      bigSeparator->size = { 500, 5 };
      bigSeparator->color = { 100, 50, 255, 255 };
      bigSeparator->roundness = 1;

      auto CreateSmallSeparator = []() {
        auto smallSeparator = new UIObject();
        smallSeparator->size = { 500, 2 };
        smallSeparator->color = BLACK;
        smallSeparator->roundness = 1;
        smallSeparator->outlineScale = 0;
        menu->PackRow({ smallSeparator });
      };


      menu->PackRow({ menuName });
      menu->PackRow({ bigSeparator });

      // ... 2
      CreateColorPickRow(
        "Toolbox color",
        "toolbox",
        [this](Color c) { App::Instance->toolBox.SetBackgroundColor(c); }
      );
      CreateSmallSeparator();

      CreateColorPickRow(
        "Color holder color",
        "colorHolder",
        [this](Color c) { App::Instance->colorHolder.SetBackgroundColor(c); }
      );
      CreateSmallSeparator();
      //menu->PackRow({ smallSeparator });

      CreateColorPickRow(
        "Manager button color",
        "managerButton",
        [this](Color c) { App::Instance->managerButton.SetBackgroundColor(c); }
      );
      CreateSmallSeparator();
      //menu->PackRow({ smallSeparator });

      CreateColorPickRow(
        "Popup-menu color",
        "miniMenu",
        [this](Color c) { MiniMenu::SetBackgroundColorForAll(c); }
      );

    }
};