#pragma once

#include "raylib.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define PATH_DATA "data/"
#define PATH_RESOURCES "resources/"
#define PATH_IMAGES "resources/Images/"
#define PATH_FONTS "resources/Fonts/"
#define FILE_COLOR_PALETTE "ColorPalette.dat"
#define FILE_USER_PREFERENCES "UserPreference.dat"

class Serializer {
  static std::string createOrGetDirectory(std::string path);
public:
  // ... load
  template <typename T>
  static T load(std::string path, std::string filename, const std::string &token);

  template <typename T>
  static std::vector<T> loadArray(std::string path, const std::string &token);

  template <typename T>
  static T loadFromData(std::string filename, const std::string &token);

  template <typename T>
  static T loadFromResources(std::string filename, const std::string &token);

  static Image loadImage(const std::string &fileName);

  // ... save
  template <typename T>
  static void save(std::string path, std::string filename, const std::string &token, const T &value);

  template <typename T>
  static void saveArray(std::string path, const std::string &token, const std::vector<T> &data);

  template <typename T>
  static void saveToData(std::string filename, const std::string &token, T value);

  template <typename T>
  static void saveToResources(std::string filename, const std::string &token, T value);

  static std::vector<std::string> loadFileLines(const char *path);
  static void saveFileLines(const std::string& path, const std::vector<std::string> & lines);

  // ... other
  static bool exists(const char *str);
};


// ================= Helpers =================
template<typename T>
T convert(const std::string &s) {
  std::stringstream ss(s);
  T value;
  ss >> value;
  return value;
}

template<> inline std::string convert<std::string>(const std::string &s) {
  return s;
}


// ================= Load (single) =================
template<typename T>
T Serializer::load(std::string path, std::string filename, const std::string &token) {

  filename = createOrGetDirectory(path) + filename;

  if (!std::filesystem::exists(filename)) {
    return T{};
  }

  std::ifstream file(filename);
  std::string line;

  while (std::getline(file, line)) {
    if (line.rfind(token + ":", 0) == 0) {
      std::string valuePart = line.substr(token.size() + 2);
      // trim
      valuePart.erase(0, valuePart.find_first_not_of(' '));
      valuePart.erase(valuePart.find_last_not_of(' ') + 1);
      return convert<T>(valuePart);
    }
  }
  return T{};
}

// ================= LoadArray =================
template<typename T>
std::vector<T> Serializer::loadArray(std::string path, const std::string &token) {
  path = createOrGetDirectory(PATH_DATA) + path;
  std::vector<T> result;

  if (!std::filesystem::exists(path)) {
    return result;
  }

  std::ifstream file(path);
  std::string line;

  while (std::getline(file, line)) {
    if (line.rfind(token + ":", 0) == 0) {
      std::string numbersPart = line.substr(token.size() + 2);
      std::stringstream ss(numbersPart);
      std::string tokenStr;

      while (std::getline(ss, tokenStr, ',')) {
        // trim
        tokenStr.erase(0, tokenStr.find_first_not_of(' '));
        tokenStr.erase(tokenStr.find_last_not_of(' ') + 1);
        if (!tokenStr.empty()) {
          result.push_back(convert<T>(tokenStr));
        }
      }
      break;
    }
  }
  return result;
}

// ================= Save (single) =================
template<typename T>
void Serializer::save(std::string path, std::string filename, const std::string &token, const T &value) {
  path = createOrGetDirectory(path) + filename;

  std::ostringstream oss;
  oss << token << ": " << value;
  std::string newLine = oss.str();

  if (!std::filesystem::exists(path)) {
    std::ofstream file(path);
    file << newLine << "\n";
    return;
  }

  std::ifstream inFile(path);
  std::vector<std::string> lines;
  bool tokenFound = false;
  std::string line;

  while (std::getline(inFile, line)) {
    if (line.rfind(token + ":", 0) == 0) {
      lines.push_back(newLine);
      tokenFound = true;
    } else {
      lines.push_back(line);
    }
  }
  inFile.close();

  if (!tokenFound) {
    lines.push_back(newLine);
  }

  std::ofstream outFile(path);
  for (const auto &l : lines) {
    outFile << l << "\n";
  }
}

// ================= SaveArray =================
template <typename T>
void Serializer::saveArray(std::string path, const std::string &token,
                           const std::vector<T> &data) {
  path = createOrGetDirectory(PATH_DATA) + path;

  std::ostringstream oss;
  oss << token << ": ";
  for (size_t i = 0; i < data.size(); i++) {
    oss << data[i];
    if (i < data.size() - 1)
      oss << ", ";
  }
  std::string newLine = oss.str();

  if (!std::filesystem::exists(path)) {
    std::ofstream file(path);
    file << newLine << "\n";
    return;
  }

  std::ifstream inFile(path);
  std::vector<std::string> lines;
  bool tokenFound = false;
  std::string line;

  while (std::getline(inFile, line)) {
    if (line.rfind(token + ":", 0) == 0) {
      lines.push_back(newLine);
      tokenFound = true;
    } else {
      lines.push_back(line);
    }
  }
  inFile.close();

  if (!tokenFound) {
    lines.push_back(newLine);
  }

  std::ofstream outFile(path);
  for (const auto &l : lines) {
    outFile << l << "\n";
  }
}

template <typename T>
T Serializer::loadFromData(std::string filename, const std::string &token) {
  return load<T>(PATH_DATA, filename, token);
}

template <typename T>
T Serializer::loadFromResources(std::string filename,
                                const std::string &token) {
  return load<T>(PATH_RESOURCES, filename, token);
}

template <typename T>
void Serializer::saveToData(std::string filename, const std::string &token, T value) {
  save<T>(PATH_DATA, filename, token, value);
}

template <typename T>
void Serializer::saveToResources(std::string filename, const std::string &token, T value) {
  save<T>(PATH_RESOURCES, filename, token, value);
}


