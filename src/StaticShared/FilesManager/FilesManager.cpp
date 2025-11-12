#include "FilesManager.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

std::string FilesManager::CreateOrGetDirectory(std::string path) {
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  return path;
}

Image FilesManager::LoadImage(const std::string& localPath) {
  std::string path = (CreateOrGetDirectory(PATH_IMAGES) + localPath);
  return ::LoadImage(path.c_str());
}

bool FilesManager::Exists(const char *str) {
  return std::filesystem::exists(str);
}

std::vector<std::string> FilesManager::LoadFileLines(const char *path) {
  std::string path_ = (CreateOrGetDirectory(PATH_DATA) + path).c_str();

  if (!std::filesystem::exists(path_)) {
    return std::vector<std::string>();
  }
  std::ifstream file(path_);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}

void FilesManager::SaveFileLines(const std::string& path, const std::vector<std::string>& lines) {
  std::string path_ = CreateOrGetDirectory(PATH_DATA) + path;

  std::ofstream file(path_, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    std::cerr << "Nie można zapisać pliku: " << path_ << std::endl;
    return;
  }
  for (const auto& line : lines) { file << line << '\n'; }
  file.close();
}

