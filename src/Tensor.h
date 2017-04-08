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

#ifndef _TENSOR_H_
#define _TENSOR_H_

/*
 * The Tensor class is used to hold multidimensional data, at its core
 * it's simply a vector wrapped with more complicated indexing that
 * allow you to access elements using offsets along each dimension.
 *
 * See test_tensor.cc for examples.
 */
template <typename T>
class Tensor
{
  public:

      Tensor() {};

      Tensor(const std::vector<int> shape, const std::vector<T> *data = nullptr){

        typename std::vector<T>::size_type expected_size = 1;
        for (typename std::vector<T>::size_type i = 0; i < shape.size(); i++)
            expected_size *= shape[i];

        if (data != NULL){
          // data is an optional argument, if it's provided we use and
          // and make sure its size agrees with the dimensions m and n.
          _values = *data;
          assert(_values.size() == expected_size);
        } else {
          // otherwise we just create an empty vector and assume the user
          // is going to fill it in later.
          _values = std::vector<T>(expected_size);
        }
        _shape = shape;

        build_index_multiplier();
      }

      void build_index_multiplier() {
        //  Here we build the index multiplier which is used when we ask for
        //  the value at some index.  If we're dealing with an (m, n, p)
        //  shaped tensor the index for a value {i, j, k} would be:
        //    (n * p) * i + p * j + k
        _index_multiplier.resize(_shape.size());
        int mult = 1;
        for (uint j = _shape.size(); j > 0; j--){
          _index_multiplier[j - 1] = mult;
          mult *= _shape[j - 1];
        }
      }

      std::vector<int> shape() const { return _shape; }

      T get(std::vector<int> indexer) {
          return _values[get_index(indexer)];
      }

      void set(std::vector<int> indexer, T v) {
        _values[get_index(indexer)] = v;
      }

      std::vector<T> _values;

  protected:
      /*
       * Given a vector indexer, this returns the corresponding index
       * into the internal flat vector.
       */
      int get_index(std::vector<int> indexer) {
        assert(indexer.size() == _shape.size());
        int idx = 0;
        for (typename std::vector<int>::size_type i = 0; i < _shape.size(); i++){
            idx += _index_multiplier[i] * indexer[i];
        }
        return idx;
      }

      std::vector<int> _shape;
      std::vector<int> _index_multiplier;
};

#endif
