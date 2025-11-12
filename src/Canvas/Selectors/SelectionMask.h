#pragma once
#include "raylib.h"
#include <vector>

class SelectionMask {
public:
  enum Type { Selection_None, Selection_Rectangle, Selection_Lasso };

  Type type = Selection_None;
  Rectangle rect{};
  std::vector<Vector2> lassoPoints;

  Image maskImage{};
  bool hasMask = false;

  void Clear();
  void CreateFromRect(const Rectangle& rect, Vector2 canvasSize);
  void CreateFromLasso(const std::vector<Vector2>& points, Vector2 canvasSize);
  bool Contains(Vector2 point) const;
};
