#include "ToolSet.h"

#include "../../Defines.h"
#include "../../Render/UIObjectsManager.h"

ToolSet::ToolSet(const std::string &name, const std::vector<Tool> &tools) {
  Texture2D icon = LoadTextureFromImage(FilesManager::LoadImage(name + ".png"));
  this->tools = tools;

  // ... adjusted by ToolBox
  button = UIObjectsManager::CreateButton();
  button->color = WHITE;
  button->SetImage(icon);
  button->roundness = 0.3f;
  button->zLayer = 2;
  button->imageMarginScale = UIOBJECT_ICON_MARGIN;
}