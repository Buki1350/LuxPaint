#ifndef UTILS_H
#define UTILS_H
#include "raylib.h"
#include <string>

struct Vector2Int { int x; int y; };

class Utils {
  static Vector2Int _GetCurrentMonitorSize();
  static float _GetSmallerMonitorEdge();
  static Vector2Int _GetWindowSize();
  static Vector2Int _monitorSize;
  static Vector2Int _windowSize;
  static float _smallerMonitorEdge;
  static Font _defaultFont;

public:
  static Vector2Int GetCurrentMonitorSize();
  static float GetSmallerMonitorEdge();
  static Vector2Int GetWindowSize();
  static Color HexToColor(std::string hex);
  static Color LoadColor(std::string token);
  static Font GetDefaultFont();
  static bool VectorsEqual(Vector2 start, Vector2 end);

  static void Init();
  static void Update();

};


#endif //UTILS_H
