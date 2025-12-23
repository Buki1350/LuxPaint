#pragma once

#pragma once
#include "../../ColorPicker/ColorPicker.h"
#include "../../MiniMenu/MiniMenuBuilder.h"
#include "ColorsSettingsBuilder.h"

#include <map>

class KeybindingsSettingsBuilder : public MiniMenuBuilderBase<KeybindingsSettingsBuilder> {
public:
  void BuildContext() {
    auto uiColorsHeader = new UIObject();
    uiColorsHeader->text = "Keybindings";
    uiColorsHeader->size = { uiColorsHeader->text.getPixelWidth(), 20 };

    menu->packRow({ MiniMenu::flexSeparator(), uiColorsHeader, MiniMenu::flexSeparator() });
    menu->createSmallSeparator();

    std::string text = Serializer::LoadFromResources<std::string>("info.dat", "keybindings_info");
    auto textField = new UIObject();
    textField->text = text;
    textField->text.wrapToWidth(TextWrapType::EXPAND);
  }
};