#include "ToolBox.h"

#include "../../PaintTools/PenTool.h"
#include "../../Render/UIObjectsManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/Utils/Utils.h"

void ToolBox::Init() {
  _oBackground = UIObjectsManager::Create();
  _oBackground->color = Utils::LoadColor("toolBoxColor");
  _oBackground->roundness = 0.25f;
  _oBackground->zLayer = 1;

  activeToolsBackgroundColor = Utils::LoadColor("toolSetColor");

  Tool pen_tool = PenTool("pen_tool");
  ToolSet paint_toolset     = ToolSet("paint_toolset", std::vector{pen_tool});
  paint_toolset.button->OnClick([this, paint_toolset](){ _ExpandTools(paint_toolset); });

  ToolSet shapes_toolset    = ToolSet("shapes_toolset", std::vector<Tool>{});
  ToolSet fill_toolset      = ToolSet("fill_toolset", std::vector<Tool>{});
  ToolSet selection_toolset = ToolSet("selection_toolset", std::vector<Tool>{});
  ToolSet effects_toolset   = ToolSet("effects_toolset", std::vector<Tool>{});

  toolSets.push_back(paint_toolset);
  toolSets.push_back(shapes_toolset);
  toolSets.push_back(fill_toolset);
  toolSets.push_back(selection_toolset);
  toolSets.push_back(effects_toolset);
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
    toolSets[i].button->position = tileStartPosition;
    toolSets[i].button->size = {(float)pixTileWidth, (float)pixTileWidth};

    if (toolSets[i].button->CursorAbove()) {
      Animator::SizeUp(toolSets[i].button);
    }
    else {
      Animator::Reset(toolSets[i].button);
    }
    tileStartPosition.y += pixTileHeight + pixTileMargin;
  }

  if (_currentToolSet != nullptr && _activeToolsBackground != nullptr) {
    if (!_activeToolsBackground->CursorAbove() && Utils::MouseClicked()) {
      UIObjectsManager::Destroy(_activeToolsBackground);
      _activeToolsBackground = nullptr;
      _currentToolSet = nullptr;
    }
  }

}

void ToolBox::_ExpandTools(const ToolSet& toolSet) {
  if (_currentToolSet != nullptr) return;
  _currentToolSet = &toolSet;

  Vec2f monitorSize = Utils::GetCurrentMonitorSize().CastTo<float>();
  int pixTileSeparation = (int)(monitorSize.y * tilesSeparationScale);
  int pixTileWidth = (int)(monitorSize.y * tilesScale);
  int pixTileHeight = pixTileWidth;

  _activeToolsBackground = UIObjectsManager::Create();
  _activeToolsBackground->position = _oBackground->position * 1.2f;
  _activeToolsBackground->size = Vec2f(pixTileWidth + 2 * pixTileSeparation,
                                     toolSet.tools.size() * (pixTileHeight + pixTileSeparation) + pixTileSeparation) * 0.9f;
  _activeToolsBackground->color = activeToolsBackgroundColor;
  _activeToolsBackground->roundness = _oBackground->roundness;
  _activeToolsBackground->zLayer = _oBackground->zLayer - 1;

  // animacja tła
  Animator::AnimatePosition(
      _activeToolsBackground,
      Vec2f(_oBackground->position.x + _oBackground->size.x, _oBackground->position.y * 1.2f),
      ANIMATION_POPUP_DURATION,
      GAUSSIAN
  );

  Vec2f startPos = _oBackground->position;
  startPos.x += _oBackground->size.x;

  for (int i = 0; i < toolSet.tools.size(); i++) {
    Tool tool = toolSet.tools[i];
    tool.button.position = startPos;
    tool.button.size = Vec2f(pixTileWidth, pixTileHeight);
    tool.button.color = GRAY;
    tool.button.zLayer = _oBackground->zLayer + 1;

    // docelowa pozycja po wysunięciu
    Vec2f targetPos = Vec2f(
        _oBackground->position.x + _oBackground->size.x + pixTileSeparation,
        _oBackground->position.y + pixTileSeparation + i * (pixTileHeight + pixTileSeparation)
    );

    Animator::AnimatePosition(&tool.button, targetPos, ANIMATION_POPUP_DURATION, GAUSSIAN);
  }
}

