#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../Singleton/Singleton.h"

class InputActionListener final : public Updatable, public Singleton<InputActionListener> {
  public:
    void update() override;

#ifdef DEBUG
    void updateDebug();
#endif
};