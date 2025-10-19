

#include "Tool.h"

Tool::Tool(const std::string &name) {
  auto icon = LoadTextureFromImage(FilesManager::LoadImage(name));
}