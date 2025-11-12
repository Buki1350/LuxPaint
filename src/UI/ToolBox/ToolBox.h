#pragma once
#include "../../Tools/ToolSet/ToolSet.h"
#include "../../Automatition/Updatables/Updatable.h"

class ToolBox final : public Updatable {
  bool _initialized = false;

  UIObject* _oBackground = nullptr;
  Color _toolSetListColor {};
  const ToolSet* _currentToolSet = nullptr;
  std::vector<ToolSet*> _toolSets;

  float _uiObjectBorderSeparation = 0.01f;
  float _tilesScale = 0.05f;
  float _widthScale = 0.04f;
  float _marginScale = 0.01f;
  float _tilesSeparationScale = 0.01f;

  void _ExpandTools(ToolSet* toolSet);

  // ==== internal helper class
  friend class ToolsSetList;
  class ToolsSetList final : public Updatable
  {
    ToolBox* _toolBox = nullptr;
    ToolSet&  _toolSet;
    UIObject* _oToolSetListBackground = nullptr;
    std::vector<Button*> _toolsButtons;
    std::pair<Vec2f, Vec2f> _CalculateTransforms();

  public:
    ToolsSetList( ToolBox* toolBox, ToolSet& toolSet );
    void Update() override;
  };
  // ====

  std::vector<ToolsSetList*> _toolsSetList_Instances;

  ToolsSetList* _activeTools = nullptr;

public:
  void Init();
  bool Initialized();
  void SetBackgroundColor(Color color);
  void Update() override;
};
