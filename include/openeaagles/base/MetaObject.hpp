
#ifndef __oe_base_MetaObject_H__
#define __oe_base_MetaObject_H__

namespace oe {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: MetaObject
// Description: MetaObject about class attributes and object instances.  This includes its name,
//              slot table, and even the number of them in existance
//------------------------------------------------------------------------------
class MetaObject
{
public:
   MetaObject() = default;
   MetaObject(const char* const, const char* const, const SlotTable* const, const MetaObject* const);
   MetaObject(const MetaObject&) = delete;
   MetaObject& operator=(const MetaObject&) = delete;

   const char* getClassName() const;
   const char* getFactoryName() const;

   const SlotTable* const slottable;         // pointer to the SlotTable
   const MetaObject* const baseMetaObject;   // pointer to the base object's MetaObject
   int count;                                // current number of object instances
   int mc;                                   // max number of instances in existance at the same time
   int tc;                                   // total number of instances created

private:
   const char* const cname;                  // class name from 'type_info'
   const char* const fname;                  // class factory name
};

}
}

#endif

