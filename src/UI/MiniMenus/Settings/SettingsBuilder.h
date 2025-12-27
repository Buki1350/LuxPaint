#pragma once
#include "../../ColorPicker/ColorPicker.h"
#include "../../MiniMenus/MiniMenu/MiniMenuBuilder.h"
#include "ColorsSettingsBuilder.h"
#include "KeybindingsSettingsBuilder.h"

#include <map>

class SettingsBuilder : public MiniMenuBuilderBase<SettingsBuilder> {
    public:
    void BuildContext() {
      Vec2f elementsSize = { 600, 20 };
      Color darkerColor = uti::colors::darkenColor(menu->getOBackground()->color, 0.25f);

      // === TITLE ===
      auto menuName = new UIObject();
      menuName->size = elementsSize;
      menuName->text = "SETTINGS";
      menuName->text.center = true;

      // === SEPARATOR ===
      auto bigSeparator = new UIObject();
      bigSeparator->size = { elementsSize.x, 5 } ;
      bigSeparator->color = darkerColor;
      bigSeparator->roundness = 1;

      menu->packRow( { menuName } );
      menu->packRow( { bigSeparator } );

      // === COLORS ===
      auto colorsSettingsButton = new Button();
      colorsSettingsButton->size = elementsSize;
      colorsSettingsButton->color = darkerColor;
      colorsSettingsButton->text.center = true;
      colorsSettingsButton->text = "Colors";
      colorsSettingsButton->onClick([]() {
         ColorsSettingsBuilder::Build();
      });
      menu->packRow( { colorsSettingsButton } );

      // === KEYBINDINGS ===
      auto keybindingsSettingsButton = new Button();
      keybindingsSettingsButton->size = elementsSize;
      keybindingsSettingsButton->color = darkerColor;
      keybindingsSettingsButton->text.center = true;
      keybindingsSettingsButton->text = "Keybindings";
      keybindingsSettingsButton->onClick([]() {
         KeybindingsSettingsBuilder::Build();
      });
      menu->packRow( { keybindingsSettingsButton } );
    }
};