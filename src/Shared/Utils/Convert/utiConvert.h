#pragma once
#include "Math/Matrx.h"
#include "Shared/Utils/Colors/Colors.h"

namespace uti::convert {
  Matrx<Color> textureToMatrix(Texture& texture);
  Matrx<Color> imageToMatrix(Image& texture);
  Texture2D matrixToTexture(Matrx<Color>& matrix);
  Image matrixToImage(Matrx<Color>& matrix);
}
