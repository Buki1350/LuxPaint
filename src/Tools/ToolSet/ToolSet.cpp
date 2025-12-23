#include "ToolSet.h"

#include "../../Defines.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"

ToolSet::ToolSet(const std::string &name, const std::vector<Tool *> &tools) {
  Texture2D icon = LoadTextureFromImage(Serializer::LoadImage(name + ".png"));
  this->name = name;
  this->tools = tools;

  // ... adjusted by ToolBox
  this->button = new Button();
  this->button->color = WHITE;
  this->button->setImage(icon);
  this->button->roundness = 0.3f;
  this->button->setZLayer(4);
  this->button->imageMarginScale = UIOBJECT_ICON_MARGIN;
}