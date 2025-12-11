#include "ToolBox.h"
#include "../../App.h"
#include "../../Shared/Animator/Animator.h"
#include "../../Shared/DelayedAction/DelayedActions.h"
#include "../../Shared/Utils/Utils.h"
#include "../../Tools/PaintTools/BrushTool.h"
#include "../../Tools/PaintTools/BucketTool.h"
#include "../../Tools/PaintTools/PenTool.h"
#include "../../Tools/SelectionTools/RectangleSelection.h"
#include "../../Tools/ShapesTools/CircleTool.h"
#include "../../Tools/ShapesTools/RectangleTool.h"
#include "../../Shared/UIObjects/UIObjectsManager.h"

void ToolBox::Init() {
  this->color = Utils::Files::LoadColor("toolbox", "uiGlobal");
  this->SetZLayer(LAYER_WIDGETS);

  _toolSetListColor = Utils::Files::LoadColor("toolSet");

  // ... paint
  ToolSet *paint_toolset =
      new ToolSet("paint_toolset", {
        new PenTool("pen_tool"),
        new BrushTool("brush_tool"),
        new BucketTool("bucket_tool")
      });
  // ... shapes
  ToolSet *shapes_toolset =
      new ToolSet("shapes_toolset", {
        new RectangleTool("rectangle_tool"),
        new CircleTool("circle_tool"),
      });
  // ... selection
  Tool *selection_tool = new RectangleSelection("selection_tool");
  ToolSet *selection_toolset =
      new ToolSet("selection_toolset", std::vector{selection_tool});


  _toolSets.push_back(paint_toolset);
  _toolSets.push_back(shapes_toolset);
  _toolSets.push_back(selection_toolset);

  for (auto ts : _toolSets) {
    ts->button->OnClick([this, ts]() { _ExpandTools(ts); });
  }

  this->_initialized = true;
}

bool ToolBox::Initialized() { return this->_initialized; }

void ToolBox::SetBackgroundColor(Color color) { this->color = color; }

void ToolBox::Update() {
  Vec2f monitorSize = Utils::View::GetCurrentMonitorSize().CastTo<float>();

  int pixTileHeight = static_cast<int>(monitorSize.y * _tilesScale);
  int pixTileWidth = pixTileHeight;
  int pixTileSeparation = static_cast<int>(monitorSize.y * _tilesSeparationScale);
  int pixTileMargin = static_cast<int>(monitorSize.y * _marginScale);
  float pixToolBoxHeight = static_cast<float>(
      pixTileSeparation +
      _toolSets.size() * (pixTileHeight + pixTileSeparation));
  float pixToolBoxWidth = static_cast<float>(pixTileSeparation + pixTileWidth + pixTileSeparation);
  float pixToolBoxMargin = monitorSize.x * _uiObjectBorderSeparation;

  this->roundness = UI_WIDGETS_ROUNDNESS * Utils::View::GetSmallerMonitorEdge();

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
}

void ToolBox::_ExpandTools(ToolSet* toolSet) {
  new ToolsSetList(this, *toolSet);
}

  ToolBox::ToolsSetList::ToolsSetList(ToolBox* toolBox, ToolSet& toolSet )
  : _toolBox(toolBox), _toolSet(toolSet) {
    toolBox->_toolsSetList_Instances.push_back(this);
    _oToolSetListBackground = new UIObject();
    _oToolSetListBackground->roundness = toolBox->roundness;

    // ... tools
    for (auto tool : toolSet.tools) {
      Button* newButton = new Button();
      newButton->SetImage(tool->icon);
      newButton->OnClick([tool] {App::Instance->canvas.SetCurrentTool(tool); });
      newButton->SetZLayer(_oToolSetListBackground->GetZLayer() + 1);
      newButton->roundness = 1;
      newButton->color = WHITE;
      newButton->imageMarginScale = UIOBJECT_ICON_MARGIN * 2;
      _toolsButtons.push_back(newButton);
    }

    _oToolSetListBackground->color = toolBox->_toolSetListColor;
    _oToolSetListBackground->position = toolBox->position;

    auto [targetPosition, targetSize] = _CalculateTransforms();
    Animator::AnimatePosition(_oToolSetListBackground, targetPosition, ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_FORWARD);
    Animator::AnimateSize(_oToolSetListBackground, targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_FORWARD);
  }

  std::pair<Vec2f, Vec2f> ToolBox::ToolsSetList::_CalculateTransforms() {
    float smallerMonitorEdge = Utils::View::GetSmallerMonitorEdge();
    Vec2f tileSize = Vec2f(_toolBox->_tilesScale * smallerMonitorEdge);
    float separator = _toolBox->_tilesSeparationScale * smallerMonitorEdge;

    int cols = static_cast<int>(1 + _toolSet.tools.size() / (_toolBox->_toolSets.size() + 1));
    float targetWidth = separator + cols * (tileSize.x + separator);
    float targetHeight = _toolSet.tools.size() > _toolBox->_toolSets.size()?
      separator + _toolBox->_toolSets.size() * (tileSize.y + separator) :
      separator + _toolSet.tools.size() * (tileSize.y + separator);

    Vec2f targetSize = Vec2f(targetWidth, targetHeight);
    Vec2f targetPosition = _oToolSetListBackground->position + Vec2f(targetWidth, 0.0f);

    return std::make_pair(targetPosition, targetSize);
  }

  void ToolBox::ToolsSetList::Update() {
    // ... closing toolSetList
    if (!_oToolSetListBackground->Clicked() && Utils::Input::MouseReleased()) {
      Animator::Reset(_oToolSetListBackground, ANIMATION_POPUP_DURATION);
      Animator::AnimateColor(_oToolSetListBackground, BLACK ,ANIMATION_POPUP_DURATION, GAUSSIAN_SHIFTED_BACKWARD);
      new DelayedAction_inSeconds(ANIMATION_POPUP_DURATION, [this](){
          auto& vec = _toolBox->_toolsSetList_Instances;
          auto it = std::find(vec.begin(), vec.end(), this);
          if (it != vec.end()) vec.erase(it);
          _oToolSetListBackground->Destroy();
          for (auto button : _toolsButtons) button->Destroy();

          delete this;
      });
      return;
    }

    float smallerMonitorEdge = Utils::View::GetSmallerMonitorEdge();
    Vec2f tileSize = Vec2f(_toolBox->_tilesScale * smallerMonitorEdge);
    float separator = _toolBox->_tilesSeparationScale * smallerMonitorEdge;

    for (int i = 0; i < _toolsButtons.size(); i++) {
      _toolsButtons[i]->size = tileSize;
      _toolsButtons[i]->position = Vec2f(
        _oToolSetListBackground->position.x + separator,
        _oToolSetListBackground->position.y + separator + (tileSize.y + separator) * i
        );
    }
  }







