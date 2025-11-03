#include "ToolBox.h"
#include "../../App.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/DelayedAction/DelayedAction.h"
#include "../../StaticShared/Utils/Utils.h"
#include "../../Tools/PaintTools/BrushTool.h"
#include "../../Tools/PaintTools/PenTool.h"
#include "../../Tools/SelectionTools/RectangleSelection.h"

void ToolBox::Init() {
  _oBackground = new UIObject();
  _oBackground->color = Utils::LoadColor("toolBox");
  _oBackground->roundness = 0.25f;
  _oBackground->zLayer = 3;

  _toolSetListColor = Utils::LoadColor("toolSet");

  // ... paint
  Tool* pen_tool = new PenTool("pen_tool");
  Tool* brush_tool = new BrushTool("brush_tool");
  ToolSet* paint_toolset      = new ToolSet("paint_toolset", std::vector{pen_tool, brush_tool});
  // ... shapes
  ToolSet* shapes_toolset     = new ToolSet("shapes_toolset", std::vector<Tool*>{});
  // ... fill
  ToolSet* fill_toolset       = new ToolSet("fill_toolset", std::vector<Tool*>{});
  // ... selection
  Tool* selection_tool        = new RectangleSelection("selection_tool");
  ToolSet* selection_toolset  = new ToolSet("selection_toolset", std::vector{selection_tool});
  // ... effects
  ToolSet* effects_toolset    = new ToolSet("effects_toolset", std::vector<Tool*>{});

  _toolSets.push_back(paint_toolset);
  _toolSets.push_back(shapes_toolset);
  _toolSets.push_back(fill_toolset);
  _toolSets.push_back(selection_toolset);
  _toolSets.push_back(effects_toolset);

  for (auto ts : _toolSets) {
    ts->button->OnClick([this, ts](){_ExpandTools(ts); });
  }

}

void ToolBox::Update() {
  Vec2f monitorSize = Utils::GetCurrentMonitorSize().CastTo<float>();

  int pixTileHeight      = (int)monitorSize.y * _tilesScale;
  int pixTileWidth       = (int)pixTileHeight;
  int pixTileSeparation  = (int)monitorSize.y * _tilesSeparationScale;
  int pixTileMargin      = (int)monitorSize.y * _marginScale;
  float pixToolBoxHeight = pixTileSeparation + _toolSets.size() * (pixTileHeight + pixTileSeparation);
  float pixToolBoxWidth  = pixTileSeparation + pixTileWidth + pixTileSeparation;
  float pixToolBoxMargin = monitorSize.x * _uiObjectBorderSeparation;

  _oBackground->position = {
      pixToolBoxMargin,
      pixToolBoxMargin
    };

  _oBackground->size = {pixToolBoxWidth, pixToolBoxHeight};

  Vec2f tileStartPosition = {
    (float)pixToolBoxMargin + pixTileSeparation,
    (float)pixToolBoxMargin + pixTileSeparation,
  };

  for (int i = 0; i < _toolSets.size(); i++) {
    _toolSets[i]->button->position = tileStartPosition;
    _toolSets[i]->button->size = {(float)pixTileWidth, (float)pixTileWidth};

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

  for (auto tool : toolSet.tools) {
    Button* newButton = new Button();
    newButton->SetImage(tool->icon);
    newButton->OnClick([tool] {App::Instance->canvas.SetCurrentTool(tool); });
    newButton->color = WHITE;
    newButton->zLayer = _oToolSetListBackground->zLayer + 1;
    newButton->roundness = 1;
    newButton->imageMarginScale = UIOBJECT_ICON_MARGIN * 2;
    _toolsButtons.push_back(newButton);
  }

  _oToolSetListBackground->color = toolBox->_toolSetListColor;
  _oToolSetListBackground->position = toolBox->_oBackground->position;

  auto [targetPosition, targetSize] = _CalculateTransforms();
  Animator::AnimatePosition(_oToolSetListBackground, targetPosition, ANIMATION_POPUP_DURATION, GAUSSIAN);
  Animator::AnimateSize(_oToolSetListBackground, targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN);
}

std::pair<Vec2f, Vec2f> ToolBox::ToolsSetList::_CalculateTransforms() {
  float smallerMonitorEdge = Utils::GetSmallerMonitorEdge();
  Vec2f tileSize = Vec2f(_toolBox->_tilesScale * smallerMonitorEdge);
  float separator = _toolBox->_tilesSeparationScale * smallerMonitorEdge;

  int cols = _toolSet.tools.size() / _toolBox->_toolSets.size() + 1;
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
  if (!_oToolSetListBackground->Clicked() && Utils::MouseReleased()) {
    Animator::Reset(_oToolSetListBackground, ANIMATION_POPUP_DURATION);
    new DelayedAction(ANIMATION_POPUP_DURATION, [this](){
        auto& vec = _toolBox->_toolsSetList_Instances;
        auto it = std::find(vec.begin(), vec.end(), this);
        if (it != vec.end()) vec.erase(it);
        _oToolSetListBackground->Destroy();
        for (auto button : _toolsButtons) button->Destroy();

        delete this;
    });
    return;
  }

  float smallerMonitorEdge = Utils::GetSmallerMonitorEdge();
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







