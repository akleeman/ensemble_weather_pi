#include <vector>
#include <iostream>
#include <cassert>

#include "VectorUtilities.h"

#ifndef _MATRIX_H_
#define _MATRIX_H_

template <typename T>
class Matrix
{
  public:

      Matrix(const int m = 0, const int n = 0, const std::vector<T> *data = nullptr){
        if (data != NULL){
          // data is an optional argument, if it's provided we use and
          // and make sure its size agrees with the dimensions m and n.
          _values = *data;
        } else {
          // otherwise we just create an empty vector and assume the user
          // is going to fill it in later.
          _values = std::vector<T>(m * n);
        }
        _m = m;
        _n = n;
        assert((int) _values.size() == _m * _n);
      }

      std::vector<int> shape() const { return {_m, _n}; }

      T get(const int i, const int j) {
          return _values[i + _m * j];
      }

      void set(const int i, const int j, T v) { _values[i + _m * j] = v; }

      void reshape(const int m, const int n) {
        _values.resize(m * n);
        _values.erase(_values.begin(), _values.end());
        _m = m;
        _n = n;
      }

      void apply ( T (*f)(int, int) ) {
        for(int i = 0; i < _m; i++) {
          // std::vector<double>::size_type j = 0;
          for (int j = 0; j < _n; j++) {
            _values[i + _m * j] = (*f)(i, j);
          }
        }
      }

  protected:
      std::vector<T> _values;
      int _m;
      int _n;
};

#endif
