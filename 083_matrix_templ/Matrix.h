#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

template<typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  std::vector<std::vector<T> > rows;

 public:
  Matrix() : numRows(0), numColumns(0) {}
  Matrix(int r, int c) : numRows(r), numColumns(c), rows(r, std::vector<T>(c)) {}
  Matrix(const Matrix & rhs) :
      numRows(rhs.numRows), numColumns(rhs.numColumns), rows(rhs.rows) {}
  Matrix & operator=(Matrix rhs) {
    std::swap(numRows, rhs.numRows);
    std::swap(numColumns, rhs.numColumns);
    std::swap(rows, rhs.rows);
    return *this;
  }
  ~Matrix(){};

  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }

  const std::vector<T> & operator[](int index) const {
    assert(index >= 0 && index < numRows);
    return rows[index];
  }

  std::vector<T> & operator[](int index) {
    assert(index >= 0 && index < numRows);
    return rows[index];
  }

  bool operator==(const Matrix & rhs) const {
    return numRows == rhs.numRows && numColumns == rhs.numColumns && rows == rhs.rows;
  }

  Matrix operator+(const Matrix & rhs) const {
    assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
    Matrix result(numRows, numColumns);
    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numColumns; j++) {
        result[i][j] = rows[i][j] + rhs[i][j];
      }
    }
    return result;
  }
};

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); i++) {
    s << (i > 0 ? ",\n" : "") << rhs[i];
  }
  s << " ]";
  return s;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs) {
  s << "{";
  for (size_t i = 0; i < rhs.size(); i++) {
    if (i > 0) {
      s << ", ";
    }
    s << rhs[i];
  }
  s << "}";
  return s;
}

#endif  // __T_MATRIX_H___
