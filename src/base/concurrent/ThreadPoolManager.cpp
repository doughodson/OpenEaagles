
#include "openeaagles/base/concurrent/ThreadPoolManager.hpp"

namespace oe {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadPoolManager, "ThreadPoolManager")
EMPTY_SLOTTABLE(ThreadPoolManager)
EMPTY_CONSTRUCTOR(ThreadPoolManager)
EMPTY_COPYDATA(ThreadPoolManager)
EMPTY_DELETEDATA(ThreadPoolManager)
EMPTY_SERIALIZER(ThreadPoolManager)

//------------------------------------------------------------------------------
// Default callback implementations (do nothing)
//------------------------------------------------------------------------------

Object* ThreadPoolManager::initialize()
{
   return nullptr;
}

void ThreadPoolManager::prepare( Object* const obj )
{
}

void ThreadPoolManager::execute( Object* const obj )
{
}

void ThreadPoolManager::execute( Object* const obj, Object* cur )
{
}

void ThreadPoolManager::destroy( Object* const obj )
{
}

}
}
