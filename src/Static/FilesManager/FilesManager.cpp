#include "FilesManager.h"
#include <filesystem>
#include <string>
#include <vector>

std::string FilesManager::CreateOrGetDirectory(std::string path) {
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  return path;
}


Image FilesManager::LoadImage(const std::string& localPath) {
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::string path = (CreateOrGetDirectory(PATH_RESOURCES) + localPath);
  return ::LoadImage(path.c_str());
}
