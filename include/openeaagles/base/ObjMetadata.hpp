
#ifndef __oe_base_ObjMetadata_H__
#define __oe_base_ObjMetadata_H__

namespace oe {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: ObjMetadata
// Description: Metadata about an object instance.  This includes its name,
//              slot table, and even the number of them in existance
//------------------------------------------------------------------------------
class ObjMetadata
{
public:
   ObjMetadata() = default;
   ObjMetadata(const char* const, const char* const, const SlotTable* const, const ObjMetadata* const);
   ObjMetadata(const ObjMetadata&) = delete;
   ObjMetadata& operator=(const ObjMetadata&) = delete;

   const char* getClassName() const;
   const char* getFactoryName() const;

   const SlotTable* const slottable;         // pointer to the SlotTable
   const ObjMetadata* const baseMetadata;    // pointer to the base object's metadata
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

