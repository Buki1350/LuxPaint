//
// Created by lukas on 30.09.2025.
//

#include "Button.h"
void Button::Update()  {
  if (CursorAbove()) Animator::SizeUp(this);
}