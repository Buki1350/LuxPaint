#pragma once
#include "MiniMenu.h"
#include "Shared/Utils/View/utiView.h"

// ... === CRTP (Curiously Recurring Template Pattern) ===
// ... Wymuszenie implementacji statycznych metod w klasach pochodnych
// ... poprzez rzutowanie pustej pamięci na typ i wywoływanie na nim konkretnej metody

template <typename T>
class MiniMenuBuilderBase {
protected:
  inline static MiniMenu* menu = nullptr;
  static void _Build() {
    if (menu != nullptr) return;
    menu = MiniMenu::createInstance();
    menu->onDestroy([](){menu = nullptr;});
    menu->_oBackground->roundness = uti::view::getSmallerMonitorEdge() * 0.00001f;

    static_cast<T*>(nullptr)->BuildContext();
  }

public:
  static void Build() { _Build(); }
};
