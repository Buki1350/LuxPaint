#pragma once
#include "Math/Matrx.h"
#include "raylib.h"

namespace uti::convert {
  Matrx<Color> textureToMatrix(Texture &texture) {
    Image image = LoadImageFromTexture(texture);

    Matrx<Color> matrix(image.height, image.width);

    Color *pixels = LoadImageColors(image);

    for (int y = 0; y < image.height; y++) {
      for (int x = 0; x < image.width; x++) {
        matrix[y][x] = pixels[y * image.width + x];
      }
    }

    UnloadImageColors(pixels);
    return matrix;
  }

  Matrx<Color> imageToMatrix(Image &texture) {
    Texture2D tex = LoadTextureFromImage(texture);
    return textureToMatrix(tex);
  }

  Texture2D matrixToTexture(Matrx<Color> &matrix) {
    int height = static_cast<int>(matrix.rowCount());
    int width  = static_cast<int>(matrix.colCount());

    Image img = GenImageColor(width, height, BLANK);

    Color* pixels = (Color*)img.data;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        pixels[y * width + x] = matrix[y][x];
      }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    return tex;
  }

  Image matrixToImage(Matrx<Color> &matrix) {
    return LoadImageFromTexture(matrixToTexture(matrix));
  }
}