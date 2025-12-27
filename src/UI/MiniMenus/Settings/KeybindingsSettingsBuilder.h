#pragma once

#pragma once
#include "../../ColorPicker/ColorPicker.h"
#include "../../MiniMenus/MiniMenu/MiniMenuBuilder.h"
#include "ColorsSettingsBuilder.h"

#include <map>

class KeybindingsSettingsBuilder : public MiniMenuBuilderBase<KeybindingsSettingsBuilder> {
public:
  void BuildContext() {
    auto uiColorsHeader = new UIObject();
    uiColorsHeader->text = "Keybindings";
    uiColorsHeader->size = { uiColorsHeader->text.getPixelWidth() + 5, 20 };

    menu->packRow({ MiniMenu::flexSeparator(), uiColorsHeader, MiniMenu::flexSeparator() });
    menu->createSmallSeparator();

    std::string text = Serializer::LoadFromResources<std::string>("Hints/info.dat", "keybindings_info");
    if (text == "") { Serializer::SaveToResources<std::string>("Hints/info.dat", "keybindings_info", "INFO.DAT INCOMPLETE"); }
    auto textField = new UIObject();
    textField->text = text;
    textField->size = { uti::view::getWindowSize().CastTo<float>().x / 2, 200 };
    textField->text.wrapToWidth(TextWrapType::EXPAND);
    textField->setZLayer(menu->getOBackground()->getZLayer());
    menu->packRow( { textField } );
  }
};