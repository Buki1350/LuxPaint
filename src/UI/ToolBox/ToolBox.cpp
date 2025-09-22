#include "ToolBox.h"

#include "../../PaintTools/PenTool.h"
#include "../../Render/UIObjectManager.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/Utils/Utils.h"

void ToolBox::Init() {

  uiObject = UIObjectManager::CreateUIObject();
  uiObject->color = Utils::LoadColor("toolBoxColor");
  uiObject->roundness = 0.25f;
  uiObject->zLayer = 1;

  Tool pen_tool = PenTool("pen_tool");
  ToolSet paint_toolset     = ToolSet("paint_toolset", std::vector{pen_tool});
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

  uiObject->position = {
      pixToolBoxMargin,
      pixToolBoxMargin
    };

  uiObject->size = {pixToolBoxWidth, pixToolBoxHeight};

  Vec2f tileStartPosition = {
    (float)pixToolBoxMargin + pixTileSeparation,
    (float)pixToolBoxMargin + pixTileSeparation,
  };

  for (int i = 0; i < toolSets.size(); i++) {
    toolSets[i].uiObject->position = tileStartPosition;
    toolSets[i].uiObject->size = {(float)pixTileWidth, (float)pixTileWidth};

    if (toolSets[i].uiObject->CursorAbove()) {
      Animator::SizeUp(toolSets[i].uiObject);
    }
    else {
      Animator::Reset(toolSets[i].uiObject);
    }
    tileStartPosition.y += pixTileHeight + pixTileMargin;
  }
}