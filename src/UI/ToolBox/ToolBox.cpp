#include "ToolBox.h"
#include "ToolBox.h"

#include "../../App.h"
#include "../../Tools/PaintTools/PenTool.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/DelayedAction/DelayedAction.h"
#include "../../StaticShared/Utils/Utils.h"
#include "../../Tools/PaintTools/BrushTool.h"

void ToolBox::Init() {
  _oBackground = UIObjectsManager::Create();
  _oBackground->color = Utils::LoadColor("toolBox");
  _oBackground->roundness = 0.25f;
  _oBackground->zLayer = 1;

  toolSetListColor = Utils::LoadColor("toolSet");

  Tool* pen_tool = new PenTool("pen_tool");
  Tool* brush_tool = new BrushTool("brush_tool");
  ToolSet* paint_toolset     = new ToolSet("paint_toolset", std::vector{pen_tool, brush_tool});

  ToolSet* shapes_toolset    = new ToolSet("shapes_toolset", std::vector<Tool*>{});

  ToolSet* fill_toolset      = new ToolSet("fill_toolset", std::vector<Tool*>{});

  ToolSet* selection_toolset = new ToolSet("selection_toolset", std::vector<Tool*>{});

  ToolSet* effects_toolset   = new ToolSet("effects_toolset", std::vector<Tool*>{});

  toolSets.push_back(paint_toolset);
  toolSets.push_back(shapes_toolset);
  toolSets.push_back(fill_toolset);
  toolSets.push_back(selection_toolset);
  toolSets.push_back(effects_toolset);

  paint_toolset->button->OnClick([this, paint_toolset](){ _ExpandTools(paint_toolset); });
}

void ToolBox::Update() {
  Vec2f monitorSize = Utils::GetCurrentMonitorSize().CastTo<float>();

  int pixTileHeight      = (int)monitorSize.y * tilesScale;
  int pixTileWidth       = (int)pixTileHeight;
  int pixTileSeparation  = (int)monitorSize.y * tilesSeparationScale;
  int pixTileMargin      = (int)monitorSize.y * marginScale;
  float pixToolBoxHeight = pixTileSeparation + toolSets.size() * (pixTileHeight + pixTileSeparation);
  float pixToolBoxWidth  = pixTileSeparation + pixTileWidth + pixTileSeparation;
  float pixToolBoxMargin = monitorSize.x * uiObjectBorderSeparation;

  _oBackground->position = {
      pixToolBoxMargin,
      pixToolBoxMargin
    };

  _oBackground->size = {pixToolBoxWidth, pixToolBoxHeight};

  Vec2f tileStartPosition = {
    (float)pixToolBoxMargin + pixTileSeparation,
    (float)pixToolBoxMargin + pixTileSeparation,
  };

  for (int i = 0; i < toolSets.size(); i++) {
    toolSets[i]->button->position = tileStartPosition;
    toolSets[i]->button->size = {(float)pixTileWidth, (float)pixTileWidth};

    tileStartPosition.y += pixTileHeight + pixTileMargin;
  }
}

void ToolBox::_ExpandTools(ToolSet* toolSet) {
  new ToolsSetList(this, *toolSet);
}



ToolBox::ToolsSetList::ToolsSetList(ToolBox* toolBox, ToolSet& toolSet )
: _toolBox(toolBox), _toolSet(toolSet) {
  toolBox->_toolsSetList_Instances.push_back(this);
  _oToolSetListBackground = UIObjectsManager::Create();
  for (auto tool : toolSet.tools) {
    Button* newButton = UIObjectsManager::CreateButton();
    newButton->SetImage(tool->icon);
    newButton->OnClick([tool] {App::Instance->canvas.SetCurrentTool(tool); });
    _toolsButtons.push_back(newButton);
  }

  _oToolSetListBackground->color = toolBox->toolSetListColor;
  _oToolSetListBackground->position = toolBox->_oBackground->position;

  auto [targetPosition, targetSize] = _CalculateTransforms();
  Animator::AnimatePosition(_oToolSetListBackground, targetPosition, ANIMATION_POPUP_DURATION, GAUSSIAN);
  Animator::AnimateSize(_oToolSetListBackground, targetSize, ANIMATION_POPUP_DURATION, GAUSSIAN);
}

std::pair<Vec2f, Vec2f> ToolBox::ToolsSetList::_CalculateTransforms() {
  float smallerMonitorEdge = Utils::GetSmallerMonitorEdge();
  Vec2f tileSize = Vec2f(_toolBox->tilesScale * smallerMonitorEdge);
  float separator = _toolBox->tilesSeparationScale * smallerMonitorEdge;

  int cols = _toolSet.tools.size() / _toolBox->toolSets.size() + 1;
  float targetWidth = separator + cols * (tileSize.x + separator);
  float targetHeight = _toolSet.tools.size() > _toolBox->toolSets.size()?
    separator + _toolBox->toolSets.size() * (tileSize.y + separator) :
    separator + _toolSet.tools.size() * (tileSize.y + separator);

  Vec2f targetSize = Vec2f(targetWidth, targetHeight);
  Vec2f targetPosition = _oToolSetListBackground->position + Vec2f(targetWidth, 0.0f);

  return std::make_pair(targetPosition, targetSize);
}

void ToolBox::ToolsSetList::Update() {
  // Zamykanie ToolSetList
  if (!_oToolSetListBackground->Clicked() && Utils::MouseClicked()) {
    Animator::Reset(_oToolSetListBackground, ANIMATION_POPUP_DURATION);
    new DelayedAction(ANIMATION_POPUP_DURATION, [this](){
        auto& vec = _toolBox->_toolsSetList_Instances;
        auto it = std::find(vec.begin(), vec.end(), this);
        if (it != vec.end()) vec.erase(it);
        UIObjectsManager::Destroy(_oToolSetListBackground);
        delete this;
    });
    return;
  }

  // Aktualne wymiary tła
  Vec2f bgPos = _oToolSetListBackground->position;
  Vec2f bgSize = _oToolSetListBackground->size;

  int numButtons = _toolsButtons.size();

  // Określamy kafelek i odstęp w tle
  float separator = _toolBox->tilesSeparationScale * Utils::GetSmallerMonitorEdge();
  float tileWidth = (bgSize.x - separator * (numButtons + 1)) / numButtons; // rozkład w poziomie
  float tileHeight = bgSize.y - 2 * separator; // dopasowanie do wysokości tła

  // Układamy przyciski
  for (int i = 0; i < numButtons; i++) {
    _toolsButtons[i]->size = Vec2f(tileWidth, tileHeight);
    _toolsButtons[i]->position = Vec2f(
        bgPos.x + separator + i * (tileWidth + separator),
        bgPos.y + separator
    );
  }
}







