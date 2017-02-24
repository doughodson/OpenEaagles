
#ifndef __oe_base_safe_ptr_H__
#define __oe_base_safe_ptr_H__

#include "openeaagles/base/util/atomics.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Template: safe_ptr<T>
//
// Description: Thread-safe shared pointer to an object of type T.
//              Provides automatic ref() and unref() of the object.
//
// Example #1
//
//    const auto ptr = new Object();      // New object; ref cnt is one
//
//    base::safe_ptr<Object> sp1( ptr );  // ref cnt is two
//
//    base::safe_ptr<Object> sp2();       // 'sp2' is null
//    sp2 = ptr;                          // ref cnt is three
//
//    base::safe_ptr<Object> sp3(sp2);    // ref cnt is four
//
//    ptr->unref();              // ref cnt is three
//    sp3 = 0;                   // ref cnt is two
//    sp2 = 0;                   // ref cnt is one
//    sp1 = 0;                   // ref cnt is zero; object is deleted
//
//
// Example #2
//
//    base::safe_ptr<Object> sp1( new Object(), false );  // new object; ref cnt stays at one
//
//
// Example #3
//
//    base::safe_ptr<Object> sp1();    // 'sp1' is null
//    sp1.set( new Object(), false );  // new object; ref cnt stays at one
//
//------------------------------------------------------------------------------
template <class T> class safe_ptr
{
public:
   safe_ptr() = default;
   safe_ptr(T* x, const bool refThis = true) : ptr(x)    { if (ptr != nullptr && refThis) ptr->ref(); }
   safe_ptr(safe_ptr<T>& x) : ptr(x.getRefPtr())         {}
   ~safe_ptr()                                           { if (ptr != nullptr) ptr->unref(); }

   // Conversion operator to return raw pointer (T*)
   operator T*()                               { return ptr; }
   operator const T*() const                   { return ptr; }
   // Operators: -> == !=
   bool operator==(const T* x) const           { return (ptr == x); }
   bool operator!=(const T* x) const           { return (ptr != x); }
   T* operator->()                             { return ptr; }
   const T* operator->() const                 { return ptr; }

   // Returns a pre-ref()'d pointer to the object
   T* getRefPtr() {
      lock();
      T* x = ptr;
      if (x != nullptr) x->ref();
      unlock();
      return x;
   }

   // Returns a pre-ref()'d const pointer to the object
   const T* getRefPtr() const {
      lock();
      const T* x = ptr;
      if (x != nullptr) x->ref();
      unlock();
      return x;
   }

   // Operator: =
   safe_ptr<T>& operator=(T* x) {
      if (ptr != x) {
         lock();
         if (ptr != nullptr) ptr->unref();
         ptr = x;
         if (ptr != nullptr) ptr->ref();
         unlock();
      }
      return *this;
   }
   safe_ptr<T>& operator=(safe_ptr<T>& x) {
      if (this != &x) { 
         x.lock();
         operator=(x.ptr);
         x.unlock();
      }
      return *this;
   }

   // set() -- set the pointer with an optional reference
   void set(T* const x, const bool refThis = true) {
      lock();
      if (ptr != nullptr) ptr->unref();
      ptr = x;
      if (ptr != nullptr && refThis) ptr->ref();
      unlock();
   }

private:
   void lock() const {
      base::lock( semaphore );
   }

   void unlock() const {
      base::unlock( semaphore );
   }

   T* ptr {};                  // the pointer being managed
   mutable long semaphore {};  // spin-lock semaphore
};

}
}

#endif

