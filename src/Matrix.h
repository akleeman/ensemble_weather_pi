/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Ensemble Weather Plugin
 * Author:   Alex Kleeman
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Alex Kleeman                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *
 */

#include <vector>
#include <iostream>
#include <cassert>

#include "VectorUtilities.h"

#ifndef _MATRIX_H_
#define _MATRIX_H_

/*
 * The Matrix class provides a lightweight wrapper around std::vector
 * which allows indexing into the array as if it were layed out in
 * rows and columns.
 */
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

      /*
       * Return the shape of the matrix in rows by columns.
       */
      std::vector<int> shape() const { return {_m, _n}; }

      /*
       * Extract a single row from the matrix.
       */
      std::vector<T> row(const int i) {
          std::vector<T> out(_n);
          for (int j = 0; j < _n; j++)
              out[j] = get(i, j);
          return out;
      }

      /*
       * Extract a single column from the matrix.
       */
      std::vector<T> col(const int j) {
          std::vector<T> out(_m);
          for (int i = 0; i < _m; i++)
              out[i] = get(i, j);
          return out;
      }

      /*
       * Get an element from row i, column j.
       */
      T get(const int i, const int j) {
          return _values[i + _m * j];
      }

      /*
       * Set element in row i, columns j.
       */
      void set(const int i, const int j, T v) {
          _values[i + _m * j] = v;
      }

      /*
       * Change the shape of the array.
       */
      void reshape(const int m, const int n) {
        _values.resize(m * n);
        // TODO: for now this also wipes the data clean
        //       which isn't really ideal.
        _values.erase(_values.begin(), _values.end());
        _m = m;
        _n = n;
      }

      /*
       * Returns the minimum value in the array.
       */
      T min() {
        T cur_val;
        T min_val = get(0, 0);
        for(int i = 0; i < _m; i++) {
          for (int j = 0; j < _n; j++) {
            cur_val = get(i, j);
            if (cur_val < min_val) min_val = cur_val;
          }
        }
        return min_val;
      }

      /*
       * Returns the maximum value in the array.
       */
      T max() {
        T cur_val;
        T max_val = get(0, 0);
        for(int i = 0; i < _m; i++) {
          for (int j = 0; j < _n; j++) {
            cur_val = get(i, j);
            if (cur_val > max_val){
              max_val = cur_val;
            }
          }
        }
        return max_val;
      }

      /*
       * Apply a function which takes i, j as arguments and
       * returns the value which should be stored in element
       * i, j.
       */
      void apply ( T (*f)(int, int) ) {
        for(int i = 0; i < _m; i++) {
          for (int j = 0; j < _n; j++) {
            set(i, j, (*f)(i, j));
          }
        }
      }

      /*
       * Returns the vector which holds the flattened version
       * of the matrix.
       */
      std::vector<T> *flat() {
        return &_values;
      }

  protected:
      std::vector<T> _values;
      int _m;
      int _n;
};

#endif
