
#ifndef __oe_base_Referenced_H__
#define __oe_base_Referenced_H__

// framework configuration file
#include "openeaagles/config.hpp"
// lock/unlock, etc - reference system needs it
#include "openeaagles/base/util/atomics.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Referenced
// Description: Base class to enable reference counting mechanism for objects
//------------------------------------------------------------------------------
class Referenced
{
public:
   Referenced() =default;
   Referenced(const Referenced&) = delete;
   Referenced& operator=(const Referenced&) = delete;
   virtual ~Referenced() =0;

   unsigned int getRefCount() const { return refCount; }

   // ---
   // ref() --
   //    Increments the number of references to this object.  An object
   //    is pre-referenced at creation and therefore does not need to be
   //    referenced by the creator (i.e., the reference count is
   //    initialized to one (1) by the constructor).  ExpInvalidRefCount
   //    is thrown if the reference count is invalid.
   // ---
   void ref() const;

   // ---
   // unref() --
   //    Decrements the number of references to this object.
   //    And, when the number of references becomes zero, deletes this object.
   // ---
   void unref() const;

   // general exception class
   class Exception {
   public:
      Exception()                                { }
      virtual ~Exception()                       { }
      virtual const char* getDescription() const {
         return "Unknown";
      }
   };

   class ExpInvalidRefCount : public Exception {
   public:
      ExpInvalidRefCount() : Exception() {}
      virtual const char* getDescription() const override {
         return "invalid reference count";
      }
   };

   class ExpInvalidRefCountDelete : public Exception {
   public:
      ExpInvalidRefCountDelete() : Exception() {}
      virtual const char* getDescription() const override {
         return "deleting object with positive reference count";
      }
   };

private:
   mutable long semaphore {};           // ref(), unref() semaphore
   mutable unsigned int refCount {1};   // reference count
};

inline Referenced::~Referenced() {}

inline void Referenced::ref() const
{
   lock(semaphore);
   if (++(refCount) <= 1) throw new ExpInvalidRefCount();
   else unlock(semaphore);

   #ifdef MAX_REF_COUNT_ERROR
   static int maxRefCount = MAX_REF_COUNT_ERROR;
   if (which->refCount > maxRefCount) {
      std::cout << "ref(" << this << "): refCount(" << which->refCount << ") exceeded max refCount(" << maxRefCount << ")." << std::endl;
   }
   #endif
}

inline void Referenced::unref() const
{
   lock(semaphore);
   if (--refCount == 0) delete this;
   else unlock(semaphore);
}

}
}

#endif
