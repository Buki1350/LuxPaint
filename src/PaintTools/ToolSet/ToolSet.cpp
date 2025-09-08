#include "ToolSet.h"

#include "../../Render/UIObjectManager.h"

ToolSet::ToolSet(const std::string &name, const std::vector<Tool> &tools) {
  Texture2D icon = LoadTextureFromImage(FilesManager::LoadImage(name + ".png"));
  this->tools = tools;

  // ... adjusted by ToolBox
  uiObject = UIObjectManager::CreateUIObject();
  uiObject->color = WHITE;
  uiObject->SetImage(icon);
  uiObject->roundness = 0.3f;
  uiObject->zLayer = 1;
}