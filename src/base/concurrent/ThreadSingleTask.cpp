
#include "openeaagles/base/concurrent/ThreadSingleTask.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadSingleTask, "ThreadSingleTask")
EMPTY_SLOTTABLE(ThreadSingleTask)
EMPTY_SERIALIZER(ThreadSingleTask)
EMPTY_DELETEDATA(ThreadSingleTask)

ThreadSingleTask::ThreadSingleTask(Component* const p, const double pri) : Thread(p, pri)
{
   STANDARD_CONSTRUCTOR()
}

ThreadSingleTask::ThreadSingleTask()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "ThreadSingleTask(" << this << ")::ThreadSingleTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "ThreadSingleTask(" << this << ")::ThreadSingleTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void ThreadSingleTask::copyData(const ThreadSingleTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "ThreadSingleTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadSingleTask::mainThreadFunc()
{
   unsigned long rtn {};

   // Configure this thread
   configThread();

   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "ThreadSingleTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Call the user function only once
   rtn = this->userFunc();

   return rtn;
}

}
}

