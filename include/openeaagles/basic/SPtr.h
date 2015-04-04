//------------------------------------------------------------------------------
// Template SPtr<T>
//      and Eaagles::Basic::Object::SPtr<T>
//
// Description: "Smart" pointer to an object of type T.
//              Provides automatic ref() and unref() of the object.
//
// Example #1
//
//    Object* p = new Object();  // New object; ref cnt is one
//
//    SPtr<Object> sp1( p );     // ref cnt is two
//
//    SPtr<Object> sp2();        // 'sp2' is null
//    sp2 = p;                   // ref cnt is three
//
//    SPtr<Object> sp3(sp2);     // ref cnt is four
//
//    p->unref();                // ref cnt is three
//    sp3 = 0;                   // ref cnt is two
//    sp2 = 0;                   // ref cnt is one
//    sp1 = 0;                   // ref cnt is zero; object is deleted
//
//
// Example #2
//
//    SPtr<Object> sp1( new Object(), false );  // new object; ref cnt stays at one
//
//
// Example #3
//
//    SPtr<Object> sp1();              // 'sp1' is null
//    sp1.set( new Object(), false );  // new object; ref cnt stays at one
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Template_SPtr_H__
#define __Eaagles_Basic_Template_SPtr_H__

template <class T> class SPtr {
public:

   // Constructors and destructor
   SPtr() : p(0), semaphore(0) {}
   SPtr(T* p_, const bool refThis = true) : p(p_), semaphore(0) { if (p != 0 && refThis) p->ref(); }
   SPtr(SPtr<T>& p_) : p(p_.getRefPtr()), semaphore(0) {}
   ~SPtr() { if (p != 0) p->unref(); }

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
      if (p0 != 0) p0->ref();
      unlock();
      return p0;
   }

   // Returns a pre-ref()'d const pointer to the object
   const T* getRefPtr() const {
      lock();
      const T* p0 = p;
      if (p0 != 0) p0->ref();
      unlock();
      return p0;
   }

   // Operator: =
   SPtr<T>& operator=(T* p_) {
      lock();
      if (p != 0) p->unref();
      p = p_;
      if (p != 0) p->ref();
      unlock();
      return *this;
   }
   SPtr<T>& operator=(SPtr<T>& p_) {
      p_.lock();
      operator=(p_.p);
      p_.unlock();
      return *this;
   }

   // set() -- set the pointer with an optional reference
   void set(T* const p_, const bool refThis = true) {
      lock();
      if (p != 0) p->unref();
      p = p_;
      if (p != 0 && refThis) p->ref();
      unlock();
   }

private:
   void lock() const {
      //SPtr<T>* which = (SPtr<T>*) this;
      lcLock( semaphore );
   }

   void unlock() const {
      //SPtr<T>* which = (SPtr<T>*) this;
      lcUnlock( semaphore );
   }

   T*  p;          // The pointer
   mutable long semaphore;  // spin-lock semaphore
};

#endif

