#pragma once
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/MiniMenu/MiniMenu.h"
class BlankCanvasBuilder {
public:
  static void Build() {
    MiniMenu* canvasCreator = MiniMenu::CreateInstance();

    auto actionName = UIObjectsManager::Create();
    actionName->size = {500, 20};
    actionName->color = RED;
    actionName->text = "New blank image";
    actionName->text.center = true;

    auto field1Info = UIObjectsManager::Create();
    field1Info->text = "Size: ";
    field1Info->size = {50, 20};

    auto field1Value1 = UIObjectsManager::CreateInputField();
    field1Value1->color = WHITE;
    field1Value1->size = {75, 20};

    auto field1Value2 = UIObjectsManager::Create();
    field1Value2->color = WHITE;
    field1Value2->size = {75, 20};

    auto slash = UIObjectsManager::Create();
    slash->size = {20, 20};
    slash->text = " /";

    auto cancelButton = UIObjectsManager::Create();
    cancelButton->size = {75, 20};
    cancelButton->color = GRAY;
    cancelButton->text = "Cancel";

    auto okButton = UIObjectsManager::Create();
    okButton->size = {50, 20};
    okButton->color = GRAY;
    okButton->text = "Ok";
    okButton->text.center = true;

    canvasCreator->PackRow({actionName});

    canvasCreator->PackRow({
      field1Info,
      MiniMenu::FlexSeparator(),
      field1Value1,
      slash,
      field1Value2
    });

    auto separator = UIObjectsManager::Create();
    separator->size = {50, 40};
    canvasCreator->PackRow({separator});

    canvasCreator->PackRow({
      cancelButton,
      MiniMenu::FlexSeparator(),
      okButton,
    });

  }
};