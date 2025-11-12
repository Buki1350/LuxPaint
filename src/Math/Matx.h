#pragma once
#include "Vec2.h"

#include <stdexcept>
#include <vector>

template<typename T>
class Matx {
  size_t _rows;
  size_t _cols;
  std::vector<std::vector<T>> _data;
public:
  Matx() : _rows(0), _cols(0) {}

  Matx(size_t rows, size_t cols, const T& defaultValue = T())
      : _rows(rows), _cols(cols), _data(rows, std::vector<T>(cols, defaultValue)) {}

  Matx(Vec2i size)
    : _rows(size.y), _cols(size.x), _data(size.y, std::vector<T>(size.x)) {}


  T& at(size_t row, size_t col) {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("Matrix::at() - index out of range");
    return _data[row][col];
  }

  const T& at(size_t row, size_t col) const {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("Matrix::at() - index out of range");
    return _data[row][col];
  }

  size_t rowCount() const { return _rows; }
  size_t colCount() const { return _cols; }

  void fill(const T& value) {
    for (auto& row : _data) {
      std::fill(row.begin(), row.end(), value);
    }
  }

  const std::vector<std::vector<T>>& raw() const { return _data; }
  std::vector<std::vector<T>>& raw() { return _data; }

  std::vector<T>& operator[](size_t row) { return _data[row]; }
  const std::vector<T>& operator[](size_t row) const { return _data[row]; }

  Vec2i size() const { return Vec2i(_cols, _rows); }
};
