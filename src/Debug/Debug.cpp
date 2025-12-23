#include "Debug.h"

#include "../Shared/Keybindings/Keybindings.h"

#include <iostream>

void Debug::Print(std::string text) {
  std::cout << text << "\n";
}
void Debug::DrawPoint(Vec2f position, Color color) {
  _debugPoints.push_back({position, color});
}

void Debug::DrawAll() {
  for (auto point : _debugPoints) {
    DrawRectangleRounded( {point.position.x - 5, point.position.y - 5, 10, 10},
      1,
      5,
      point.color );
  }
  _debugPoints.clear();
}

void Debug::PrintPushedKeys() {
  auto pushedKeys = Keybindings::getAllPushedKeys();

  std::cout << "[ ";
  for (int i = 0; i < pushedKeys.size(); i++) {
    std::cout << pushedKeys[i];
    if (i == pushedKeys.size() - 1) {
      std::cout << " ";
    } else {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl << std::endl;
}