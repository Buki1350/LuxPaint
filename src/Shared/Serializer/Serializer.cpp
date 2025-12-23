#include "Serializer.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

std::string Serializer::CreateOrGetDirectory(std::string path) {
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  return path;
}

Image Serializer::LoadImage(const std::string& fileName) {
  std::string path = (CreateOrGetDirectory(PATH_IMAGES) + fileName);
  Image image = ::LoadImage(path.c_str());
  if (IsImageValid(image)) {
    return image;
  }
  std::cerr << "Failed to load image: " << path << "\n";
  return ::LoadImage((CreateOrGetDirectory(PATH_IMAGES) + std::string("no_image.png")).c_str());
}

bool Serializer::Exists(const char *str) {
  return std::filesystem::exists(str);
}

std::vector<std::string> Serializer::loadFileLines(const char *path) {
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

void Serializer::saveFileLines(const std::string& path, const std::vector<std::string>& lines) {
  std::string path_ = CreateOrGetDirectory(PATH_DATA) + path;

  std::ofstream file(path_, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    std::cerr << "Nie można zapisać pliku: " << path_ << std::endl;
    return;
  }
  for (const auto& line : lines) { file << line << '\n'; }
  file.close();
}

