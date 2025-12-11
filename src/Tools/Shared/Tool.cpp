#include "Tool.h"

#include "Tool.h"
#include <iostream>

void Tool::SetSize(int val) { size = val; }

Tool::Tool(const std::string &name) {
  this->_name = name;
  icon = {};

  // Wczytanie obrazu
  Image img = FilesManager::LoadImage(name + ".png");
  if (!img.data) {
    std::cerr << "Failed to load image: " << name << ".png\n";
    // Tworzymy placeholder (1x1 białe)
    img = GenImageColor(1, 1, WHITE);
  }

  // Upload do GPU
  icon = LoadTextureFromImage(img);

  // Zwolnienie pamięci CPU
  UnloadImage(img);

  // Dodatkowa kontrola
  if (icon.id == 0) {
    std::cerr << "Texture upload failed for: " << name << "\n";
  }
}

