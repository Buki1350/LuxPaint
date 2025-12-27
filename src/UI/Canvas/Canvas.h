#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "raylib.h"

#include <vector>

#include "../../Tools/Shared/Tool.h"

class Canvas final : public UIObject, public Updatable {
  // ... first layer works as base for other layers
  std::vector<UIObject*> _oLayers;
  // ... active layer
  UIObject* _oActiveImage = nullptr;
  // ... for moving around
  Vec2f _dragOffset {0,0};
  bool _isDragging = false;
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

  void _updateBackground();
    void _handleOutline();
    void _handlePainting();
    void _handleCropping();
  UIObjEdge _getHoveredEdge(UIObject *layer, Vec2f mp);

  bool _canDrag();
  void _handleZoomAndDrag();
  void _createBackground();

public:
  void init();
  void addImage(Image image);
  void addTexture(Texture2D texture);
  void update() override;

  const std::vector<UIObject*>& getLayersInfo();
  void setCurrentColor(Color color);
  Color getCurrentColor();
  void setCurrentTool(Tool *tool);
  const Tool *getCurrentToolRO();
  Tool *getCurrentTool();
  Image exportAsImage();
};
