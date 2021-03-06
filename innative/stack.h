// Copyright (c)2021 Fundament Software
// For conditions of distribution and use, see copyright notice in innative.h

#ifndef IN__STACK_H
#define IN__STACK_H

#include <malloc.h>
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

namespace innative {
  // Implements a simple stack that can temporarily restrict how far it can be shrunk
  template<typename T> class Stack
  {
  public:
    Stack() : _array(0), _capacity(0), _size(0), _limit(0) {}
    ~Stack()
    {
      if(_array)
        free(_array);
    }
    inline void Reserve(size_t capacity)
    {
      if(_capacity < capacity)
      {
        _array = reinterpret_cast<T*>(realloc(_array, capacity * sizeof(T)));
        if(!_array) // Out of memory, abort
          abort();
        _capacity = capacity;
      }
    }
    inline void Push(const T& item)
    {
      if(_size >= _capacity)
        Reserve(!_capacity ? MINSIZE : (_capacity * 2));
      _array[_size++] = item;
    }
    inline T Pop()
    {
      assert(_size > _limit);
      return _array[--_size];
    } // This only works with trivial types
    inline T& Peek()
    {
      assert(_size > _limit);
      return _array[_size - 1];
    }
    inline const T& Peek() const
    {
      assert(_size > _limit);
      return _array[_size - 1];
    }
    inline const T& Back() const { return _array[_limit]; }
    inline T& Back() { return _array[_limit]; }
    inline size_t Capacity() const { return _capacity; }
    inline size_t Size() const { return _size - _limit; }
    inline size_t Limit() const { return _limit; }
    inline T* Get() const { return _array; }
    inline void SetLimit(size_t limit)
    {
      assert(limit <= _size);
      _limit = limit;
    }
    inline void Resize(size_t size)
    {
      assert(_limit + size <= _size);
      _size = _limit + size;
    }

    static const int MINSIZE = 8;

    const T& operator[](size_t i) const
    {
      assert(i < _size);
      return _array[_size - i - 1];
    }
    T& operator[](size_t i)
    {
      assert(i < _size);
      return _array[_size - i - 1];
    }

  protected:
    T* _array;
    size_t _capacity;
    size_t _size;
    size_t _limit;
  };
}

#endif