
#include "openeaagles/base/concurrent/Thread.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Thread, "Thread")
EMPTY_SLOTTABLE(Thread)
EMPTY_SERIALIZER(Thread)

Thread::Thread(Component* const p, const double pri) : parent(p), priority(pri)
{
   STANDARD_CONSTRUCTOR()
}

Thread::Thread()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "Thread(" << this << ")::Thread() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "Thread(" << this << ")::Thread() -- ERROR: Did NOT create the thread!" << std::endl;
}

void Thread::copyData(const Thread& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "Thread(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

void Thread::deleteData()
{
   if (!isTerminated()) {
      terminate();
   }
   closeThread();
}

//------------------------------------------------------------------------------
// Create the thread
//------------------------------------------------------------------------------
bool Thread::create()
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
bool Thread::isTerminated() const
{
   return killed;
}

double Thread::getPriority() const
{
   return priority;
}

Component* Thread::getParent()
{
   return parent;
}

const void* Thread::getThreadHandle() const
{
   return theThread;
}

// thread stack size in bytes (or zero if using the default stack size)
size_t Thread::getStackSize() const
{
   return stackSize;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Set the thread's stack size in bytes (or zero to use the system default size)
bool Thread::setStackSize(const size_t size)
{
   stackSize = size;
   return true;
}

// Set the terminated flag
void Thread::setTerminated()
{
   killed = true;
}

}
}
