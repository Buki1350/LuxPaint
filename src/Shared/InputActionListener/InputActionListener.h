#pragma once
#include "../../Automatition/Updatables/Updatable.h"
#include "../Singleton/Singleton.h"
#include "Automatition/Initializables/Initializable.h"

class InputActionListener final : public Updatable, public Initializable, public Singleton<InputActionListener> {
  public:
    void init() override;
    void update() override;

#ifdef DEBUG
    void updateDebug();
#endif
};