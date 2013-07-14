// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
#ifndef BNET_ATOMIC_H
#define BNET_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace bnet
{

template<typename T>
class AtomicIntegerT : boost::noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  // uncomment if you need copying and assignment
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  T Get()
  {
    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  T GetAndAdd(T x)
  {
    return __sync_fetch_and_add(&value_, x);
  }

  T AddAndGet(T x)
  {
    return GetAndAdd(x) + x;
  }

  T IncrementAndGet()
  {
    return AddAndGet(1);
  }

  T DecrementAndGet()
  {
    return AddAndGet(-1);
  }

  void Add(T x)
  {
    GetAndAdd(x);
  }

  void Increment()
  {
    IncrementAndGet();
  }

  void decrement()
  {
    DecrementAndGet();
  }

  T GetAndSet(T newValue)
  {
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  volatile T value_;
};

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // BNET_ATOMIC_H
