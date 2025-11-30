#pragma once
#include "../../Shared/Utils/Utils.h"
#include "MiniMenu.h"

// ... === CRTP (Curiously Recurring Template Pattern) ===
// ... Wymuszenie implementacji statycznych metod w klasach pochodnych
// ... poprzez rzutowanie pustej pamięci na typ i wywoływanie na nim konkretnej metody

template <typename T>
class MiniMenuBuilderBase {
protected:
  inline static MiniMenu* menu = nullptr;
  inline static MiniMenuBuilderBase* baseInstance = nullptr;
  static void _Build() {
    if (menu != nullptr) return;
    menu = MiniMenu::CreateInstance();
    menu->OnDestroy([](){menu = nullptr;});
    menu->_oBackground->roundness = Utils::View::GetSmallerMonitorEdge() * 0.00001f;

    static_cast<T*>(nullptr)->BuildContext();
  }

public:
  static void Build() { _Build(); }
};
