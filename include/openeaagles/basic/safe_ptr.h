
#include "openeaagles/basic/support.h"

#ifndef __Eaagles_Basic_safe_ptr_H__
#define __Eaagles_Basic_safe_ptr_H__

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Template safe_ptr<T>
//
// Description: Thread-safe shared pointer to an object of type T.
//              Provides automatic ref() and unref() of the object.
//
// Example #1
//
//    Object* p = new Object();         // New object; ref cnt is one
//
//    Basic::safe_ptr<Object> sp1( p ); // ref cnt is two
//
//    Basic::safe_ptr<Object> sp2();    // 'sp2' is null
//    sp2 = p;                          // ref cnt is three
//
//    Basic::safe_ptr<Object> sp3(sp2); // ref cnt is four
//
//    p->unref();                // ref cnt is three
//    sp3 = 0;                   // ref cnt is two
//    sp2 = 0;                   // ref cnt is one
//    sp1 = 0;                   // ref cnt is zero; object is deleted
//
//
// Example #2
//
//    Basic::safe_ptr<Object> sp1( new Object(), false );  // new object; ref cnt stays at one
//
//
// Example #3
//
//    Basic::safe_ptr<Object> sp1();   // 'sp1' is null
//    sp1.set( new Object(), false );  // new object; ref cnt stays at one
//
//------------------------------------------------------------------------------
template <class T> class safe_ptr {
public:

   // constructors
   safe_ptr() : p(nullptr), semaphore(0) {}
   safe_ptr(T* p_, const bool refThis = true) : p(p_), semaphore(0)  { if (p != nullptr && refThis) p->ref(); }
   safe_ptr(safe_ptr<T>& p_) : p(p_.getRefPtr()), semaphore(0)       {}
   // destructor
   ~safe_ptr()                                                       { if (p != nullptr) p->unref(); }

   // Conversion operator to return raw pointer (T*)
   operator T*()                               { return p; }
   operator const T*() const                   { return p; }
   // Operators: -> == !=
   bool operator==(const T* t) const           { return (p == t); }
   bool operator!=(const T* t) const           { return (p != t); }
   T* operator->()                             { return p; }
   const T* operator->() const                 { return p; }

   // Returns a pre-ref()'d pointer to the object
   T* getRefPtr() {
      lock();
      T* p0 = p;
      if (p0 != nullptr) p0->ref();
      unlock();
      return p0;
   }

   // Returns a pre-ref()'d const pointer to the object
   const T* getRefPtr() const {
      lock();
      const T* p0 = p;
      if (p0 != nullptr) p0->ref();
      unlock();
      return p0;
   }

   // Operator: =
   safe_ptr<T>& operator=(T* p_) {
      lock();
      if (p != nullptr) p->unref();
      p = p_;
      if (p != nullptr) p->ref();
      unlock();
      return *this;
   }
   safe_ptr<T>& operator=(safe_ptr<T>& p_) {
      p_.lock();
      operator=(p_.p);
      p_.unlock();
      return *this;
   }

   // set() -- set the pointer with an optional reference
   void set(T* const p_, const bool refThis = true) {
      lock();
      if (p != nullptr) p->unref();
      p = p_;
      if (p != nullptr && refThis) p->ref();
      unlock();
   }

private:
   void lock() const {
      lcLock( semaphore );
   }

   void unlock() const {
      lcUnlock( semaphore );
   }

   T*  p;                   // the pointer being managed
   mutable long semaphore;  // spin-lock semaphore
};

}
}

#endif

