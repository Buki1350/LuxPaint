#include "../../Automatition/Updatables/Updatable.h"
#include "../../StaticShared/Animator/Animator.h"
#include "../UIObject.h"
class Button : public UIObject, public Updatable {
public:
  void Update() override;
};