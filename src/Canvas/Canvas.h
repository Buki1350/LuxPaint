#pragma once
#include "../Defines.h"
#include "../Render/UIObject.h"
#include "../Automatition/Updatables/Updatable.h"
#include "raylib.h"

#include <vector>

#include "../Tools/Shared/Tool.h"

class Canvas final : public Updatable {
  std::vector<UIObject*> _oLayers;
  UIObject* _oActiveImage = nullptr;
  Vec2f _dragOffset {0,0};
  bool _isDragging = false;
  UIObject* _oBackground = nullptr;
  Tool* _currentTool = nullptr;
  bool _CanDrag();
  void _HandleZoomAndDrag();
  void _CreateBackground();
  void _UpdateBackground();

  void _HandleOutline();

public:
  void Init();
  void AddImage(Image image);
  void AddTexture(Texture2D texture);
  void Update() override;
  std::vector<UIObject*>& GetImages() { return _oLayers; }
  void SetCurrentTool(Tool* tool);
};
