
#include "openeaagles/ubf/lua/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/ubf/lua/LuaBehavior.h"
#include "openeaagles/ubf/lua/LuaState.h"

namespace oe {
namespace lua {

base::Object* factory(const std::string& name)
{
   base::Object* obj = nullptr;

   if (name == LuaBehavior::getFactoryName()) {
      obj = new LuaBehavior();
   }

   else if (name == LuaState::getFactoryName()) {
      obj = new LuaState();
   }

   return obj;
}

}
}

