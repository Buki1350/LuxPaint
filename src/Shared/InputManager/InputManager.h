#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../Singleton/Singleton.h"

class InputManager final : public Updatable, public Singleton<InputManager> {
  public:
    void Update() override;

#ifdef DEBUG
    void UpdateDebug();
#endif
};