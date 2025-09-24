#include "ToolSet.h"

#include "../../Defines.h"
#include "../../Render/UIObjectsManager.h"

ToolSet::ToolSet(const std::string &name, const std::vector<Tool> &tools) {
  Texture2D icon = LoadTextureFromImage(FilesManager::LoadImage(name + ".png"));
  this->tools = tools;

  // ... adjusted by ToolBox
  uiObject = UIObjectsManager::Create();
  uiObject->color = WHITE;
  uiObject->SetImage(icon);
  uiObject->roundness = 0.3f;
  uiObject->zLayer = 2;
  uiObject->imageMarginScale = UIOBJECT_ICON_MARGIN;
}