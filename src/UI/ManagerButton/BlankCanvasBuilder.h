#pragma once
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/MiniMenu/MiniMenu.h"
class BlankCanvasBuilder {
public:
  static void Build() {
    MiniMenu* canvasCreator = MiniMenu::CreateInstance();

    auto objj = UIObjectsManager::Create();
    objj->size = {500, 20};
    //objj->text = "New blank image";
    objj->text.center = true;

    auto obj = UIObjectsManager::Create();
    obj->size = {500, 20};
    obj->color = RED;
    obj->text = "New blank image";
    obj->text.center = true;

    auto obj1 = UIObjectsManager::Create();
    obj1->size = {500, 200};
    obj1->color = ORANGE;

    auto obj2 = UIObjectsManager::Create();
    obj2->size = {500, 200};
    obj2->color = YELLOW;

    canvasCreator->Pack(objj);
    canvasCreator->Pack(obj);
    canvasCreator->Pack(obj1);
    canvasCreator->Pack(obj2);
  }
};