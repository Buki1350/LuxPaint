#pragma once
#include "../../../Shared/UIObjects/UIObjectsManager.h"
#include "../MiniMenu/MiniMenu.h"
#include "../MiniMenu/MiniMenuBuilder.h"
#include "Math/Matrx.h"
#include "Shared/SnapshotsManager/SnapshotsTypes/ImageSnapshot.h"
#include "Shared/Utils/Convert/utiConvert.h"

class BlankCanvasBuilder : public MiniMenuBuilderBase<BlankCanvasBuilder> {
public:
  void BuildContext() {
    auto actionName = new UIObject();
    actionName->size = {500, 20};
    actionName->color = uti::colors::darkenColor(menu->getOBackground()->color, 0.25f);
    actionName->text = "New blank image";
    actionName->text.center = true;

    auto field1Info = new UIObject();
    field1Info->text = "Size in pixels: ";
    field1Info->size = {150, 20};

    auto field1Value1 = new InputField();
    field1Value1->color = WHITE;
    field1Value1->size = {75, 20};

    auto field1Value2 = new InputField();
    field1Value2->color = WHITE;
    field1Value2->size = {75, 20};

    auto slash = new UIObject();
    slash->size = {20, 20};
    slash->text = " /";

    auto okButton = new Button();
    okButton->size = {50, 20};
    okButton->color = GRAY;
    okButton->text = "Ok";
    okButton->text.center = true;
    okButton->onClick([field1Value1, field1Value2]() {
      std::string sValue1 = field1Value1->getValue();
      std::string sValue2 = field1Value2->getValue();
      if (sValue1.empty() || sValue2.empty()) return;

      Vec2f size = Vec2f(stof(sValue1), stof(sValue2));
      Matrx<Color> blankImageMatrix(size.CastTo<int>());
      blankImageMatrix.fill(WHITE);
      Texture2D blankImage = uti::convert::matrixToTexture(blankImageMatrix);
      App::instance().canvas.addTexture(blankImage);

      menu->destroy();
    });

    auto cancelButton = new Button();
    cancelButton->size = {75, 20};
    cancelButton->color = GRAY;
    cancelButton->text = "Cancel";
    cancelButton->onClick([]() {
      menu->destroy();
    });

    menu->packRow({actionName});

    menu->packRow({
      field1Info,
      MiniMenu::flexSeparator(),
      field1Value1,
      slash,
      field1Value2
    });

    auto separator = new UIObject();
    separator->size = {50, 40};
    menu->packRow({separator});

    menu->packRow({
      cancelButton,
      MiniMenu::flexSeparator(),
      okButton,
    });
  }

  static void CreateBlank(Vec2i size) {
    Matrx<Color> blankImageMatrix(size);
    blankImageMatrix.fill(WHITE);
    Texture2D blankImage = uti::convert::matrixToTexture(blankImageMatrix);
    App::instance().canvas.addTexture(blankImage);
    App::instance().snapshotManager.saveSnapshot(
        std::make_unique<ImageSnapshot>(App::instance().canvas.getLayersInfo()));
  }
};