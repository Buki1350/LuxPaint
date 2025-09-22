#pragma once
#include "../../Automatition/Initializables/Initializable.h"
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Math/Matx.h"
#include "../../Math/Vec2.h"
#include "../Singleton/Singleton.h"
#include "raylib.h"

#include <string>

class Utils final : public Singleton<Utils>, public Updatable, public Initializable {
  static Vec2i _GetCurrentMonitorSize();
  static float _GetSmallerMonitorEdge();
  static Vec2i _GetWindowSize();
  static Vec2i _monitorSize;
  static Vec2i _windowSize;
  static float _smallerMonitorEdge;
  static Font _defaultFont;
  static Vec2f _mousePosition;
  static float _deltaTime;

public:
  static float GetSmallerMonitorEdge();
  static float GetDeltaTime();
  static Vec2i GetCurrentMonitorSize();
  static Vec2i GetWindowSize();
  static Color HexToColor(std::string hex);
  static Color LoadColor(std::string token);
  static Font GetDefaultFont();
  static bool VectorsEqual(Vector2 start, Vector2 end);
  static Vec2f GetMousePosition();
  static Matx<Color> TextureToMatrix(Texture& texture);
  static Matx<Color> ImageToMatrix(Image& texture);
  static Texture2D MatrixToTexture(Matx<Color>& matrix);
  static Image MatrixToImage(Matx<Color>& matrix);

  void Init() override;
  void Update() override;

};