#include "FrontPanel.h"
#include "../../Render/UIObjectManager.h"
#include "../Utils/Utils.h"
#include "../Animator/Animator.h"

std::vector<FrontPanel*> FrontPanel::Instances = {};

FrontPanel * FrontPanel::CreateInstance() {
  FrontPanel *newFrontPanel = new FrontPanel();
  Instances.push_back(newFrontPanel);
  return newFrontPanel;
}

FrontPanel* FrontPanel::Pack(UIObject *object) {
  if (_oBackground == nullptr) {
    _oBackground = UIObjectManager::CreateUIObject();
    float borderSize = FRONTPANEL_BORDER_SIZE_SCALE * Utils::GetSmallerMonitorEdge();
    Vec2f targetSize = _oBackground->size;

    _oBackground->size = object->size + Vec2(borderSize, borderSize) * 2;
    _oBackground->color = Utils::LoadColor("frontPanel");
    _oBackground->position = Utils::GetWindowSize().CastTo<float>() / 2;
    _oBackground->size = {10, 10};
    _oBackground->imageAlpha = 0.0f;
    _oBackground->name = "frontPanel";
    _oBackground->roundness = object->roundness;

    float time = .1f;
    Animator::AnimateSize(_oBackground, object->size, time, GAUSSIAN);
    Animator::AnimatePosition(_oBackground, object->position, time, GAUSSIAN);
  }

  return this;
}
