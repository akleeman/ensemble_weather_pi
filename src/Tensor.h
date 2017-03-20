#include <vector>
#include <iostream>
#include <cassert>

#ifndef _TENSOR_H_
#define _TENSOR_H_

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

      int get_index(std::vector<int> indexer) {
        int idx = 0;
        for (typename std::vector<int>::size_type i = 0; i < _shape.size(); i++){
            idx += _index_multiplier[i] * indexer[i];
        }
        return idx;
      }

      T get(std::vector<int> indexer) {
          return _values[get_index(indexer)];
      }

      void set(std::vector<int> indexer, T v) {
        _values[get_index(indexer)] = v;
      }

      std::vector<T> _values;

  protected:
      std::vector<int> _shape;
      std::vector<int> _index_multiplier;
};

#endif
