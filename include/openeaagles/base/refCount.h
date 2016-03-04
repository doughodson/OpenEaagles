//------------------------------------------------------------------------------
// ref(), unref() and getRefCount() functions
//------------------------------------------------------------------------------
#ifndef __oe_base_RefCount_H__
#define __oe_base_RefCount_H__

   // ---
   // If the following is defined then ref() will print a message to standard
   // output, cerr, if the reference count exceeds this number.
   //
   //    If you start seeing the ref count exceed message then a simple
   //    debugging technique is to attach a break point to the std::cout
   //    statement below and see who's calling ref().
   // ---
   //#define MAX_REF_COUNT_ERROR 256

   // ---
   // getRefCount() --
   // ---
   unsigned int getRefCount() const { return refCount; }

   // ---
   // ref() --
   //    Increments the number of references to this object.  An object
   //    is pre-referenced at creation and therefore does not need to be
   //    referenced by the creator (i.e., the reference count is
   //    initialized to one (1) by the constructor).  ExpInvalidRefCount
   //    is thrown if the reference count is invalid.
   // ---
   void ref() const
   {
      lcLock(semaphore);
      if (++(refCount) <= 1) throw new ExpInvalidRefCount();
      else lcUnlock(semaphore);

      #ifdef MAX_REF_COUNT_ERROR
      static int maxRefCount = MAX_REF_COUNT_ERROR;
      if (which->refCount > maxRefCount) {
         std::cout << "ref(" << this << "): refCount(" << which->refCount << ") exceeded max refCount(" << maxRefCount << ")." << std::endl;
      }
      #endif
   }

   // ---
   // unref() --
   //    Decrements the number of references to this object.
   //    And, when the number of references becomes zero, deletes this object.
   // ---
   void unref() const
   {
      lcLock(semaphore);
      if (--refCount == 0) delete this;
      else lcUnlock(semaphore);
   }

#endif
