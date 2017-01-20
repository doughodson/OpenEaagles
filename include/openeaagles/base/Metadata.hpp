
#ifndef __oe_base_Metadata_H__
#define __oe_base_Metadata_H__

namespace oe {
namespace base {
class SlotTable;

//------------------------------------------------------------------------------
// Class: Metadata
// Description: Metadata about class attributes and object instances.  This includes its name,
//              slot table, and even the number of them in existance
//------------------------------------------------------------------------------
class Metadata
{
public:
   Metadata() = default;
   Metadata(const char* const, const char* const, const SlotTable* const, const Metadata* const);
   Metadata(const Metadata&) = delete;
   Metadata& operator=(const Metadata&) = delete;

   const char* getClassName() const;
   const char* getFactoryName() const;

   const SlotTable* const slottable;         // pointer to the SlotTable
   const Metadata* const baseMetadata;       // pointer to the base object's metadata
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

