#pragma once
#include "../Automatition/Updatables/Updatable.h"
#include "../Shared/UIObjects/UIObject.h"
#include "raylib.h"

#include <vector>

#include "../Tools/Shared/Tool.h"
#include "Selectors/SelectionMask.h"

class Canvas final : public Updatable {
  // ... first layer works as base for other layers
  std::vector<UIObject*> _oLayers;
  // ... active layer
  UIObject* _oActiveImage = nullptr;
  // ... for moving around
  Vec2f _dragOffset {0,0};
  bool _isDragging = false;
  // ... regular app background
  UIObject* _oBackground = nullptr;
  // ... drawing utility
  Color _currentColor = BLACK;
  Tool* _currentTool = nullptr;
  // ... resizing image
  int cropGrabTolerance = 5; // ... pixels
  bool _isCropping = false;
  enum class UIObjEdge { NONE, LEFT, RIGHT, TOP, BOTTOM };
  UIObjEdge _activeEdge = UIObjEdge::NONE;
  UIObject* _oResizePreview = nullptr; // TODO: make it better

  const float MIN_CANVAS_SIZE = 8.0f;

  void _UpdateBackground();
    void _HandleOutline();
    void _HandlePainting();
    void _HandleCropping();
  UIObjEdge _GetHoveredEdge(UIObject *layer, Vec2f mp);

  bool _CanDrag();
  void _HandleZoomAndDrag();
  void _CreateBackground();

public:
  void Init();
  void AddImage(Image image);
  void AddTexture(Texture2D texture);
  void Update() override;

  std::vector<UIObject*>& GetImages();
  void SetCurrentColor(Color color);
  Color GetCurrentColor();
  void SetCurrentTool(Tool *tool);
  const Tool *GetCurrentToolRO();
  Tool* GetCurrentTool();
};
