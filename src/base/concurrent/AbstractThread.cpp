
#include "openeaagles/base/concurrent/AbstractThread.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractThread, "AbstractThread")
EMPTY_SLOTTABLE(AbstractThread)
EMPTY_SERIALIZER(AbstractThread)

AbstractThread::AbstractThread(Component* const p, const double pri) : parent(p), priority(pri)
{
   STANDARD_CONSTRUCTOR()
}

AbstractThread::AbstractThread()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "AbstractThread(" << this << ")::AbstractThread() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "AbstractThread(" << this << ")::AbstractThread() -- ERROR: Did NOT create the thread!" << std::endl;
}

void AbstractThread::copyData(const AbstractThread& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "AbstractThread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a AbstractThread!" << std::endl;
}

void AbstractThread::deleteData()
{
   if (!isTerminated()) {
      terminate();
   }
   closeThread();
}

//------------------------------------------------------------------------------
// Create the thread
//------------------------------------------------------------------------------
bool AbstractThread::create()
{
   bool ok = true;

   // Check for a parent pointer
   if (parent == nullptr) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Parent pointer is not set!" << std::endl;
      ok = false;
   }

   // Check for valid priority
   if (priority < 0.0f || priority > 1.0f) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Invalid priority! Use [ 0.0 ... 1.0 ]" << std::endl;
      ok = false;
   }

   // Create the thread
   if (ok) ok = createThread();

   if (!ok) {
      std::cerr << "Thread(" << this << ")::create() -- ERROR: Did NOT create the thread!" << std::endl;
   }

   killed = !ok;

   return ok;
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------
bool AbstractThread::isTerminated() const
{
   return killed;
}

double AbstractThread::getPriority() const
{
   return priority;
}

Component* AbstractThread::getParent()
{
   return parent;
}

const void* AbstractThread::getThreadHandle() const
{
   return theThread;
}

// thread stack size in bytes (or zero if using the default stack size)
size_t AbstractThread::getStackSize() const
{
   return stackSize;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Set the thread's stack size in bytes (or zero to use the system default size)
bool AbstractThread::setStackSize(const size_t size)
{
   stackSize = size;
   return true;
}

// Set the terminated flag
void AbstractThread::setTerminated()
{
   killed = true;
}

}
}

