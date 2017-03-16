
#ifndef __oe_base_SingleTask_H__
#define __oe_base_SingleTask_H__

#include "openeaagles/base/concurrent/Thread.hpp"

namespace oe {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: SingleTask
// Description: Abstract single task thread class
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is only called once.
//------------------------------------------------------------------------------
class SingleTask : public Thread
{
   DECLARE_SUBCLASS(SingleTask, Thread)

public:
   SingleTask(Component* const parent, const double priority);

   // User defined work function
   private: virtual unsigned long userFunc() =0;

protected:
   SingleTask();

private:
   // Thread class function
   virtual unsigned long mainThreadFunc() override;
};

}
}

#endif

