
#include "openeaagles/base/concurrent/SingleTask.hpp"

#include "openeaagles/base/Component.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(SingleTask, "AbstractSingleTask")
EMPTY_SLOTTABLE(SingleTask)
EMPTY_SERIALIZER(SingleTask)
EMPTY_DELETEDATA(SingleTask)

SingleTask::SingleTask(Component* const p, const double pri) : AbstractThread(p, pri)
{
   STANDARD_CONSTRUCTOR()
}

SingleTask::SingleTask()
{
   STANDARD_CONSTRUCTOR()
   std::cerr << "SingleTask(" << this << ")::SingleTask() -- ERROR: Do not use the default constructor" << std::endl;
   std::cerr << "SingleTask(" << this << ")::SingleTask() -- ERROR: Did NOT create the thread!" << std::endl;
}

void SingleTask::copyData(const SingleTask& org, const bool)
{
   BaseClass::copyData(org);

   std::cerr << "SingleTask(" << this << ")::copyData() -- ERROR: Can NOT copy or clone a Thread!" << std::endl;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long SingleTask::mainThreadFunc()
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

