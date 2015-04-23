
#ifndef __Eaagles_Basic_ref_ptr_H__
#define __Eaagles_Basic_ref_ptr_H__

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Template ref_ptr<T>
//
// Description: Shared pointer to an object of type T.
//              Provides automatic ref() and unref() of the object.
//
// Example #1
//
//    Object* p = new Object();  // New object; ref cnt is one
//
//    ref_ptr<Object> sp1( p );  // ref cnt is two
//
//    ref_ptr<Object> sp2();     // 'sp2' is null
//    sp2 = p;                   // ref cnt is three
//
//    ref_ptr<Object> sp3(sp2);  // ref cnt is four
//
//    p->unref();                // ref cnt is three
//    sp3 = 0;                   // ref cnt is two
//    sp2 = 0;                   // ref cnt is one
//    sp1 = 0;                   // ref cnt is zero; object is deleted
//
//
// Example #2
//
//    ref_ptr<Object> sp1( new Object(), false );  // new object; ref cnt stays at one
//
//
// Example #3
//
//    ref_ptr<Object> sp1();           // 'sp1' is null
//    sp1.set( new Object(), false );  // new object; ref cnt stays at one
//
//------------------------------------------------------------------------------
template <class T> class ref_ptr {
public:

   // constructors
   ref_ptr() : p(nullptr)                               {}
   ref_ptr(T* p_, const bool refThis = true) : p(p_)    { if (p != nullptr && refThis) p->ref(); }
   ref_ptr(ref_ptr<T>& p_) : p(p_.getRefPtr())          {}
   // descructor
   ~ref_ptr() { if (p != nullptr) p->unref(); }

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
      T* p0 = p;
      if (p0 != nullptr) p0->ref();
      return p0;
   }

   // Returns a pre-ref()'d const pointer to the object
   const T* getRefPtr() const {
      const T* p0 = p;
      if (p0 != nullptr) p0->ref();
      return p0;
   }

   // Operator: =
   ref_ptr<T>& operator=(T* p_) {
      if (p != nullptr) p->unref();
      p = p_;
      if (p != nullptr) p->ref();
      return *this;
   }
   ref_ptr<T>& operator=(ref_ptr<T>& p_) {
      operator=(p_.p);
      return *this;
   }

   // set() -- set the pointer with an optional reference
   void set(T* const p_, const bool refThis = true) {
      if (p != nullptr) p->unref();
      p = p_;
      if (p != nullptr && refThis) p->ref();
   }

private:

   T*  p;    // the pointer being managed
};

}
}

#endif

