#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../../Tools/ToolSet/ToolSet.h"
#include "Automatition/Initializables/Initializable.h"

class ToolBox final : public UIObject, public Updatable, public Initializable {
  bool _initialized = false;

  Color _toolSetListColor {};
  const ToolSet* _currentToolSet = nullptr;
  std::vector<ToolSet*> _toolSets;

  float _uiObjectBorderSeparation = 0.01f;
  float _tilesScale = 0.05f;
  float _widthScale = 0.04f;
  float _marginScale = 0.01f;
  float _tilesSeparationScale = 0.01f;

  void _expandTools(ToolSet* toolSet);

  // ==== internal helper class
  friend class ToolsSetList;
  class ToolsSetList final : public Updatable
  {
    ToolBox* _toolBox = nullptr;
    ToolSet&  _toolSet;
    UIObject* _oToolSetListBackground = nullptr;
    std::vector<Button*> _toolsButtons;
    std::pair<Vec2f, Vec2f> _calculateTransforms();
    bool _forDelete = false;

  public:
    void destroy();
    bool isForDelete() const;
    ToolsSetList( ToolBox* toolBox, ToolSet& toolSet );
    void update() override;
  };
  // ====

  std::vector<ToolsSetList*> _toolsSetList_Instances;

  ToolsSetList* _activeTools = nullptr;

public:
  void init() override;
  bool initialized();
  void setBackgroundColor(Color color);
  void update() override;
};
