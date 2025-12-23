#pragma once
#include "../../ColorPicker/ColorPicker.h"
#include "../../MiniMenu/MiniMenuBuilder.h"
#include "ColorsSettingsBuilder.h"
#include "KeybindingsSettingsBuilder.h"

#include <map>

class SettingsBuilder : public MiniMenuBuilderBase<SettingsBuilder> {

    public:
    void BuildContext() {
        auto menuName = new UIObject();
        menuName->size = { 500, 20 };
        menuName->text = "SETTINGS";
        menuName->text.center = true;

        auto bigSeparator = new UIObject();
        bigSeparator->size = { 500, 5 };
        bigSeparator->color = { 100, 50, 255, 255 };
        bigSeparator->roundness = 1;

        menu->packRow( { menuName } );
        menu->packRow( { bigSeparator } );

        // === COLORS ===
        auto colorsSettingsButton = new Button();
        colorsSettingsButton->size = { 500, 20 };
        colorsSettingsButton->color = { 100, 50, 255, 255 };
        colorsSettingsButton->text.center = true;
        colorsSettingsButton->text = "Colors";
        colorsSettingsButton->onClick([]() {
           ColorsSettingsBuilder::Build();
        });
        menu->packRow( { colorsSettingsButton } );

      // === KEYBINDINGS ===
      // auto keybindingsSettingsButton = new Button();
      // keybindingsSettingsButton->size = { 500, 20 };
      // keybindingsSettingsButton->color = { 100, 50, 255, 255 };
      // keybindingsSettingsButton->text.center = true;
      // keybindingsSettingsButton->text = "Keybindings";
      // keybindingsSettingsButton->onClick([]() {
      //    KeybindingsSettingsBuilder::Build();
      // });
      // menu->packRow( { colorsSettingsButton } );
    }
};