#pragma once
#include "../../Render/UIObjectsManager.h"
#include "../../UI/MiniMenu/MiniMenu.h"
class BlankCanvasBuilder {
public:
  inline static MiniMenu* canvasCreator = nullptr;
  static void Build() {
    if (canvasCreator != nullptr) return;
    canvasCreator = MiniMenu::CreateInstance();
    canvasCreator->OnDestroy([](){canvasCreator = nullptr;});

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

    auto field1Value2 = UIObjectsManager::CreateInputField();
    field1Value2->color = WHITE;
    field1Value2->size = {75, 20};

    auto slash = UIObjectsManager::Create();
    slash->size = {20, 20};
    slash->text = " /";

    auto cancelButton = UIObjectsManager::CreateButton();
    cancelButton->size = {75, 20};
    cancelButton->color = GRAY;
    cancelButton->text = "Cancel";

    auto okButton = UIObjectsManager::CreateButton();
    okButton->size = {50, 20};
    okButton->color = GRAY;
    okButton->text = "Ok";
    okButton->text.center = true;

    okButton->OnClick([field1Value1, field1Value2]() {
      std::string sValue1 = field1Value1->GetValue();
      std::string sValue2 = field1Value2->GetValue();
      if (sValue1.empty() || sValue2.empty()) return;

      Vec2f size = Vec2f(stof(sValue1), stof(sValue2));
      Matx<Color> blankImageMatrix(size.CastTo<int>());
      blankImageMatrix.fill(WHITE);
      Texture2D blankImage = Utils::MatrixToTexture(blankImageMatrix);
      App::Instance->canvas.AddTexture(blankImage);
    });

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

  void BlankCanvasBuilder::_CreateBlank(Vec2i size) {
    Matx<Color> blankImageMatrix(size);
    blankImageMatrix.fill(WHITE);
    Texture2D blankImage = Utils::MatrixToTexture(blankImageMatrix);
    App::Instance->canvas.AddTexture(blankImage);
  }
};