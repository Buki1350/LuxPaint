#pragma once
#include "Utils.h"

Matrx<Color> Utils::Convert::TextureToMatrix(Texture &texture) {
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

Matrx<Color> Utils::Convert::ImageToMatrix(Image &texture) {
  Texture2D tex = LoadTextureFromImage(texture);
  return TextureToMatrix(tex);
}

Texture2D Utils::Convert::MatrixToTexture(Matrx<Color> &matrix) {
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


Image Utils::Convert::MatrixToImage(Matrx<Color> &matrix) {
  return LoadImageFromTexture(MatrixToTexture(matrix));
}
