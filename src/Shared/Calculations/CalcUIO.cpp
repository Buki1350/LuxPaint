#include "../../Math/Vec2.h"
#include "../../Shared/UIObjects/UIObject.h"
#include "Calc.h"

Vec2f Calc::UIO::LeftCorner(const UIObject* uio) {
  return uio->position + Vec2f (0, uio->size.y);
}