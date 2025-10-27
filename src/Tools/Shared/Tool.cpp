#include "Tool.h"

Tool::Tool(const std::string &name) {
  toolName = name;
  icon = LoadTextureFromImage(FilesManager::LoadImage(name + ".png"));
}
