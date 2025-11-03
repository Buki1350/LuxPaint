#pragma once
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
  bool _hasCanvas = false;
  UIObject* _oBackground = nullptr;
  Tool* _currentTool = nullptr;
  Color _currentColor = BLACK;
  Rectangle _selection{0,0,0,0};
  UIObject* _selectionTarget = nullptr;

  bool _CanDrag();
  void _HandleZoomAndDrag();
  void _CreateBackground();
  void _UpdateBackground();

  void _HandleOutline();
  void _HandlePainting();

public:
  void Init();
  void AddImage(Image image);
  void AddTexture(Texture2D texture);
  void Update() override;
  void SetSelection(const Rectangle& rect);
  void ClearSelection();

  std::vector<UIObject*>& GetImages() { return _oLayers; }
  void SetCurrentColor(Color color)   { _currentColor = color; }
  Color GetCurrentColor()             { return _currentColor; }
  void SetCurrentTool(Tool* tool)     { _currentTool = tool; }
  Rectangle GetSelection() const      { return _selection; }
  bool HasCanvas() const              { return _hasCanvas; }
};
