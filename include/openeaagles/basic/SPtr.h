//------------------------------------------------------------------------------
// Template SPtr<T>
//      and Eaagles::Basic::Object::SPtr<T>
//
// Description: "Smart" pointer to an object of type T.
//              Provides automatic ref() and unref() of the object.
//------------------------------------------------------------------------------
template <class T> class SPtr {
public:

   // Constructors and destructor
   SPtr(SPtr<T>& p_) : p(p_.getRefPtr()), semaphore(0)   { }
   SPtr(T* p_) : p(p_), semaphore(0)           { if (p != 0) p->ref(); }
   SPtr() : p(0), semaphore(0)                 { }
   ~SPtr()                                     { if (p != 0) p->unref(); }

   // Operators: * -> == !=
   operator T*()                               { return p; }
   operator const T*() const                   { return p; }
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
