

#include "Tool.h"

#include "../../Render/UIObjectsManager.h"

Tool::Tool(const std::string &name) {
  toolName = name;
  icon = LoadTextureFromImage(FilesManager::LoadImage(name));
}
