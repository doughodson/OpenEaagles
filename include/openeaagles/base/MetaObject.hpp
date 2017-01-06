
#ifndef __oe_base_MetaObject_H__
#define __oe_base_MetaObject_H__

namespace oe {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: MetaObject
//------------------------------------------------------------------------------
class MetaObject
{
public:
   MetaObject(const unsigned int, const char* const, const char* const, const SlotTable* const, const MetaObject* const);

   const unsigned int classIndex;     // Registered class index
   const char* const cname;           // class name from 'type_info'
   const char* const fname;           // class factory name
   const SlotTable* const st;         // Pointer to the SlotTable
   const MetaObject* const bstatic;   // Pointer to the base class MetaObject object
   int count;                         // NCurrent of instances
   int mc;                            // Max number of instances
   int tc;                            // Total number of instances

   private:MetaObject& operator=(const MetaObject&);

};

}
}

#endif

