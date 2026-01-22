#include "ToolBox.h"
#include "../../App/App.h"
#include "../../Shared/Animator/Animator.h"
#include "../../Shared/DelayedAction/DelayedActions.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"
#include "../../Tools/PaintTools/BrushTool.h"
#include "../../Tools/PaintTools/BucketTool.h"
#include "../../Tools/PaintTools/PenTool.h"
#include "../../Tools/ShapesTools/CircleTool.h"
#include "../../Tools/ShapesTools/RectangleTool.h"
#include "Shared/Utils/Files/utiFiles.h"
#include "Shared/Utils/View/utiView.h"
#include "Tools/PaintTools/EraserTool.h"
#include "Tools/ShapesTools/LineTool.h"

void ToolBox::init() {
  this->color = uti::files::loadColor("toolbox", "uiGlobal");
  this->setZLayer(LAYER_WIDGETS);

  _toolSetListColor = uti::files::loadColor("toolSet");

  // ... paint
  ToolSet *paint_toolset =
      new ToolSet("paint_toolset", {
        new PenTool("pen_tool"),
        new BrushTool("brush_tool"),
        new BucketTool("bucket_tool"),
        new EraserTool("eraser_tool"),
      });
  // ... shapes
  ToolSet *shapes_toolset =
      new ToolSet("shapes_toolset", {
        new RectangleTool("rectangle_tool"),
        new CircleTool("circle_tool"),
        new LineTool("line_tool"),
      });


  _toolSets.push_back(paint_toolset);
  _toolSets.push_back(shapes_toolset);

  for (auto ts : _toolSets) {
    ts->button->setZLayer(this->getZLayer() + 1);
    ts->button->onClick([this, ts]() { _expandTools(ts); });
  }

  this->_initialized = true;
}

bool ToolBox::initialized() { return this->_initialized; }

void ToolBox::setBackgroundColor(Color color) { this->color = color; }

void ToolBox::update() {
  Vec2f monitorSize = uti::view::getCurrentMonitorSize().CastTo<float>();

  int pixTileHeight = static_cast<int>(monitorSize.y * _tilesScale);
  int pixTileWidth = pixTileHeight;
  int pixTileSeparation = static_cast<int>(monitorSize.y * _tilesSeparationScale);
  int pixTileMargin = static_cast<int>(monitorSize.y * _marginScale);
  float pixToolBoxHeight = static_cast<float>(pixTileSeparation + _toolSets.size() * (pixTileHeight + pixTileSeparation));
  float pixToolBoxWidth = static_cast<float>(pixTileSeparation + pixTileWidth + pixTileSeparation);
  float pixToolBoxMargin = monitorSize.x * _uiObjectBorderSeparation;

  this->roundness = UI_WIDGETS_ROUNDNESS * uti::view::getSmallerMonitorEdge();

  this->position = {pixToolBoxMargin, pixToolBoxMargin};

  this->size = {pixToolBoxWidth, pixToolBoxHeight};

  Vec2f tileStartPosition = {
      (float)pixToolBoxMargin + pixTileSeparation,
      (float)pixToolBoxMargin + pixTileSeparation,
  };

  for (int i = 0; i < _toolSets.size(); i++) {
    _toolSets[i]->button->position = tileStartPosition;
    _toolSets[i]->button->size = {(float)pixTileWidth, (float)pixTileWidth};
    _toolSets[i]->button->roundness = roundness;

    tileStartPosition.y += pixTileHeight + pixTileMargin;
  }

  for (auto ts : _toolsSetList_Instances) {
    if (ts != nullptr && ts->isForDelete()) {
      delete ts;
    }
  }
}

void ToolBox::_expandTools(ToolSet *toolSet) {
  new ToolsSetList(this, *toolSet);
}

void ToolBox::ToolsSetList::destroy() {
  _forDelete = true;
}

bool ToolBox::ToolsSetList::isForDelete() const {
  return _forDelete;
}

ToolBox::ToolsSetList::ToolsSetList(ToolBox* toolBox, ToolSet& toolSet )
: _toolBox(toolBox), _toolSet(toolSet) {
  toolBox->_toolsSetList_Instances.push_back(this);
  _oToolSetListBackground = new UIObject();
  _oToolSetListBackground->setZLayer(toolBox->getZLayer() - 1);
  _oToolSetListBackground->roundness = toolBox->roundness;

  // ... tools
  for (auto tool : toolSet.tools) {
    Button* newButton = new Button();
    newButton->setImage(tool->icon);
    newButton->onClick([tool] {App::instance().canvas.setCurrentTool(tool); });
    newButton->setZLayer(toolBox->getZLayer() + 1);
    newButton->roundness = 1;
    newButton->color = WHITE;
    newButton->imageMarginScale = UIOBJECT_ICON_MARGIN * 2;
    _toolsButtons.push_back(newButton);
  }

  _oToolSetListBackground->color = toolBox->_toolSetListColor;
  _oToolSetListBackground->position = toolBox->position;

  auto [targetPosition, targetSize] = _calculateTransforms();
  Animator::animatePosition(_oToolSetListBackground, targetPosition, ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_FORWARD);
  Animator::animateSize(_oToolSetListBackground, targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_FORWARD);
}

std::pair<Vec2f, Vec2f> ToolBox::ToolsSetList::_calculateTransforms() {
  float edge = uti::view::getSmallerMonitorEdge();
  Vec2f tileSize(_toolBox->_tilesScale * edge);
  float sep = _toolBox->_tilesSeparationScale * edge;

  int maxRows = static_cast<int>(_toolBox->_toolSets.size());
  int toolCount = static_cast<int>(_toolSet.tools.size());
  int cols = (toolCount + maxRows - 1) / maxRows;

  float width  = sep + cols * (tileSize.x + sep);
  float height = sep + maxRows * (tileSize.y + sep);

  Vec2f targetSize(width, height);

  Vec2f targetPosition = {
    _toolBox->position.x + _toolBox->size.x,
    _toolBox->position.y
  };

  return {targetPosition, targetSize};
}



void ToolBox::ToolsSetList::update() {
  // ... closing toolSetList
  if (!_oToolSetListBackground->clicked() && uti::input::mouseReleased()) {
    Animator::reset(_oToolSetListBackground, ANIMATION_POPUP_DURATION);
    Animator::animateColor(_oToolSetListBackground, BLACK ,ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_BACKWARD);
    new DelayedAction_inSeconds(ANIMATION_POPUP_DURATION, [this](){
        auto& vec = _toolBox->_toolsSetList_Instances;
        auto it = std::find(vec.begin(), vec.end(), this);
        if (it != vec.end()) vec.erase(it);
        _oToolSetListBackground->destroy();
        for (auto button : _toolsButtons) button->destroy();

        _forDelete = true;
    });
    return;
  }

  float edge = uti::view::getSmallerMonitorEdge();
  Vec2f tileSize(_toolBox->_tilesScale * edge);
  float sep = _toolBox->_tilesSeparationScale * edge;

  int maxRows = static_cast<int>(_toolBox->_toolSets.size());

  for (int i = 0; i < _toolsButtons.size(); i++) {
    int row = i % maxRows;
    int col = i / maxRows;

    _toolsButtons[i]->size = tileSize;
    _toolsButtons[i]->position = Vec2f(
      _oToolSetListBackground->position.x + sep + col * (tileSize.x + sep),
      _oToolSetListBackground->position.y + sep + row * (tileSize.y + sep)
    );
  }
}







