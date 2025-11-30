#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Math/Matrx.h"
#include "../../Math/Vec2.h"
#include "../KeyBindings/Keybindings.h"
#include "../Singleton/Singleton.h"
#include "raylib.h"

#include <iosfwd>
#include <map>
#include <string>

class Utils final : public Singleton<Utils>, public Updatable {
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

  class Files final {
    public:
    static Color LoadColor(std::string token, std::string defaultToken = "");
    static void SetDefaultFont();
    static void SaveColor(const char *token, const Color &color);
    static std::map<InputAction, std::vector<KeyboardKey>> LoadActions();
    static void SaveActions(const std::map<InputAction, std::vector<KeyboardKey>>& bindings);
    static std::map<std::string, Color> LoadAllColors();
    static void SaveAllColors(std::pmr::map<std::string, Color> colorMap);
  };
  class Colors final {
    public:
    static Color HexToColor(std::string hex);
    static std::string ColorToHex(const Color &color, bool includeAlpha = false);
    static Color DarkenColor(const Color& color, float factor);
    static Color LightenColor(Color color, float x);
  };
  class Input final {
    public:
    static bool MouseReleased();
    static bool MousePressed();
    static bool MouseDown();
    static Vec2f GetMousePosition();
  };
  class View final {
    public:
    static Vec2i GetCurrentMonitorSize();
    static Vec2i GetWindowSize();
    static float GetSmallerMonitorEdge();
  };
  class Time final {
    public:
    static float GetDeltaTime();
  };
  class AppData final {
  public:
    static Font GetDefaultFont();
  };
  class Convert final {
    public:
    static Matrx<Color> TextureToMatrix(Texture& texture);
    static Matrx<Color> ImageToMatrix(Image& texture);
    static Texture2D MatrixToTexture(Matrx<Color>& matrix);
    static Image MatrixToImage(Matrx<Color>& matrix);
  };
  class RaylibSpecific final {
    public:
    static bool VectorsEqual(Vector2 start, Vector2 end);
  };
  class Keybindings final {
    public:
    std::vector<std::vector<InputAction>> LoadActions();
  };

  void Update() override;
};
