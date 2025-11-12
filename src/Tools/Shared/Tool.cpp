#include "Tool.h"

Tool::Tool(const std::string &name) {
  this->_name = name;
  icon = LoadTextureFromImage(FilesManager::LoadImage(name + ".png"));
}
